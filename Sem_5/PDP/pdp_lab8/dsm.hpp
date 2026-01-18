#pragma once

#include <mpi.h>
#include <functional>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

/**
 * Distributed Shared Memory (DSM) Implementation — Replicated Model
 *
 * - Each process maintains a local array of NUM_VARIABLES integer variables.
 * - All operations (write, compare-and-swap, read) update or access local state,
 *   but changes are globally ordered across the relevant subscriber group.
 * - Lamport clocks and distributed ACK-based total-order multicast are used for
 *   uniform sequential consistency within each subscription group.
 * - Only subscribers of a variable may perform updates or receive change notifications.
 * - Callbacks always execute in the same order on all subscribers, preserving total order.
 */

const int NUM_PROCESSES = 4;
const int NUM_VARIABLES = 5;

enum class MessageType {
    WRITE = 1, // Simple update/set operation
    CAS = 2, // Atomic compare-and-swap operation
    ACK = 3 // Acknowledgement for total order multicast
};

class DistributedSharedMemory {
public:
    /**
     * Callback type:
     *  void(int variable_id, int old_value, int new_value, int lamport_time)
     * Called whenever a variable changes due to an ordered DSM event delivered.
     */
    using ChangeCallback = std::function<void(int, int, int, int)>;

private:
    // MPI and system context
    int rank_; // This process's unique ID (0 to world_size-1)
    int world_size_; // Total number of MPI processes
    bool verbose_; // If true, print debug logs

    // Local array of DSM variables (replicated model, no ownership)
    std::vector<int> variables_; // The actual variable storage
    mutable std::mutex variable_mutex_; // Protects variables_ from concurrent access

    // Static per-variable subscription sets
    std::map<int, std::set<int>> subscriptions_; // Maps var_id -> set of subscriber ranks
    mutable std::mutex subscriptions_mutex_; // Protects subscriptions_

    // Registered change notification callback
    ChangeCallback change_callback_; // User function called on variable changes
    std::mutex callback_mutex_; // Protects callback registration

    // Logical clock and per-sender monotonic message id (Lamport protocol)
    std::atomic<int> lamport_clock_; // Logical time for ordering events
    std::atomic<int> next_message_id_; // Unique ID generator for messages

    // Pending messages (not yet delivered: priority queue for total order)
    struct PendingMessage {
        int timestamp; // Lamport timestamp when message was sent
        int sender; // Rank of process that originated this message
        int msg_id; // Unique per-sender message ID
        MessageType type; // WRITE or CAS
        int var_id; // Which variable this affects
        int new_value; // Value to write
        int expected; // For CAS: value we expect to see (0 for WRITE)
    };

    // Comparator for priority queue: orders by (timestamp, sender, msg_id)
    struct PendingCompare {
        bool operator()(const PendingMessage& a, const PendingMessage& b) const {
            // Note: returns true if a > b because priority_queue is a max-heap by default
            // We want min-heap behavior (smallest timestamp first)
            if (a.timestamp != b.timestamp) return a.timestamp > b.timestamp;
            if (a.sender != b.sender) return a.sender > b.sender;
            return a.msg_id > b.msg_id;
        }
    };

    std::priority_queue<PendingMessage, std::vector<PendingMessage>, PendingCompare> pending_messages_;
    mutable std::mutex pending_mutex_; // Protects pending_messages_

    // Per-(sender, msg_id) set recording all who have ACKed the message
    std::map<std::pair<int,int>, std::set<int>> ack_sets_; // Tracks ACKs for each message
    mutable std::mutex ack_mutex_; // Protects ack_sets_

    // Per-CAS local result (for request originator to see the outcome)
    std::map<std::pair<int,int>, bool> cas_result_; // Maps (sender,msg_id) -> success/failure
    std::map<std::pair<int,int>, bool> cas_done_; // Maps (sender,msg_id) -> is result ready?
    std::mutex cas_mutex_; // Protects CAS result tracking
    std::condition_variable cas_cv_; // Wakes up threads waiting for CAS results

    // Thread for automatic MPI message polling/processing
    std::thread message_thread_; // Background thread that processes messages
    std::atomic<bool> running_; // Set to false to stop background thread

public:
    /**
     * Constructor: Create the DSM component and start background message processing.
     */
    DistributedSharedMemory(int rank, int world_size, bool verbose = false)
            : rank_(rank),
              world_size_(world_size),
              verbose_(verbose),
              variables_(NUM_VARIABLES, 0), // Initialize all variables to 0
              lamport_clock_(0), // Start logical clock at 0
              next_message_id_(0), // First message ID is 0
              running_(true) // Background thread should run
    {
        // Validate that we're running with the correct number of processes
        if (world_size != NUM_PROCESSES) {
            throw std::runtime_error("World size must be " + std::to_string(NUM_PROCESSES));
        }

        if (verbose_) {
            log("DSM initialized with " + std::to_string(NUM_VARIABLES) + " variables per process.");
        }

        // Start background thread that continuously processes incoming MPI messages
        message_thread_ = std::thread(&DistributedSharedMemory::messageProcessingLoop, this);
    }

    /**
     * Destructor: Stop background thread and clean up.
     */
    ~DistributedSharedMemory() {
        running_ = false; // Signal background thread to stop
        if (message_thread_.joinable()) {
            message_thread_.join(); // Wait for thread to finish
        }
    }

    // Prevent copying (DSM object has thread and mutexes, can't be safely copied)
    DistributedSharedMemory(const DistributedSharedMemory&) = delete;
    DistributedSharedMemory& operator=(const DistributedSharedMemory&) = delete;

    /**
     * Subscribe current rank to a DSM variable.
     * All subscribers must call this with the same subscriber_ranks set.
     */
    void subscribe(int variable_id, const std::set<int>& subscriber_ranks) {
        // Validate variable ID is in range [0, NUM_VARIABLES)
        if (variable_id < 0 || variable_id >= NUM_VARIABLES) {
            throw std::runtime_error("Variable ID out of range");
        }

        // Ensure this process is in the subscriber list (can't subscribe if not listed)
        if (subscriber_ranks.find(rank_) == subscriber_ranks.end()) {
            throw std::runtime_error("Process must be a subscriber to subscribe.");
        }

        {
            // Thread-safely update subscription map
            std::lock_guard<std::mutex> lock(subscriptions_mutex_);
            subscriptions_[variable_id] = subscriber_ranks;
        }

        if (verbose_) log("Subscribed to variable " + std::to_string(variable_id));

        incrementClock(); // Subscribing is a local event, advance clock
    }

    /**
     * Register a callback function to be invoked on every variable change.
     * Callback will be called in the same order on all subscribers.
     */
    void setChangeCallback(ChangeCallback callback) {
        std::lock_guard<std::mutex> lock(callback_mutex_);
        change_callback_ = callback; // Store the user's callback function
    }

    /**
     * Read the current value of a DSM variable (always local, no network).
     */
    int read(int variable_id) {
        // Validate variable ID
        if (variable_id < 0 || variable_id >= NUM_VARIABLES)
            throw std::runtime_error("Invalid variable id");

        incrementClock(); // Reading is a local event

        // Thread-safely read the variable
        std::lock_guard<std::mutex> lock(variable_mutex_);
        return variables_[variable_id];
    }

    /**
     * Write a value to a DSM variable.
     * Initiates total-order protocol, returns immediately (fire-and-forget).
     */
    void write(int variable_id, int new_value) {
        // Security: ensure we're allowed to write (must be subscribed)
        ensureSubscribed(variable_id);

        // Advance logical clock (starting a write is a local event)
        incrementClock();
        int timestamp = lamport_clock_.load(); // Capture current time
        int msg_id = next_message_id_++; // Get unique message ID

        // Package the write operation
        PendingMessage msg{timestamp, rank_, msg_id, MessageType::WRITE, variable_id, new_value, 0};

        {
            // Add to our own local queue (we queue our own writes!)
            std::lock_guard<std::mutex> lock(pending_mutex_);
            pending_messages_.push(msg);
        }

        // Create unique key for ACK tracking
        auto key = std::make_pair(rank_, msg_id);

        {
            // We implicitly ACK our own message
            std::lock_guard<std::mutex> lock(ack_mutex_);
            ack_sets_[key].insert(rank_);
        }

        // Get list of all subscribers for this variable
        std::set<int> subscribers = getSubscribers(variable_id);

        // Prepare MPI message buffer: [type, var_id, new_value, expected(0), sender, msg_id, timestamp]
        std::vector<int> buffer = {(int)MessageType::WRITE, variable_id, new_value, 0, rank_, msg_id, timestamp};

        // Send to all other subscribers (not to ourselves)
        for (int dest : subscribers) {
            if (dest == rank_) continue;  // Skip self
            MPI_Send(buffer.data(), buffer.size(), MPI_INT, dest, 0, MPI_COMM_WORLD);
        }

        if (verbose_) {
            log("WRITE var=" + std::to_string(variable_id) +
                " value=" + std::to_string(new_value) + " | T=" + std::to_string(timestamp));
        }
        // Note: Write doesn't wait for delivery, returns immediately
    }

    /**
     * Atomic compare-and-swap: if variable == expected, set to new_value.
     * Blocks until operation is delivered and returns success/failure.
     */
    bool compareAndSwap(int variable_id, int expected, int new_value) {
        // Security: ensure we're subscribed
        ensureSubscribed(variable_id);

        // Advance clock and capture timestamp
        incrementClock();
        int timestamp = lamport_clock_.load();
        int msg_id = next_message_id_++;

        // Package CAS operation (note: expected field is meaningful for CAS)
        PendingMessage msg{timestamp, rank_, msg_id, MessageType::CAS, variable_id, new_value, expected};

        {
            // Add to local queue
            std::lock_guard<std::mutex> lock(pending_mutex_);
            pending_messages_.push(msg);
        }

        auto key = std::make_pair(rank_, msg_id);

        {
            // Self-ACK
            std::lock_guard<std::mutex> lock(ack_mutex_);
            ack_sets_[key].insert(rank_);
        }

        {
            // Initialize result tracking (we'll wait for this)
            std::lock_guard<std::mutex> lock(cas_mutex_);
            cas_result_[key] = false; // Assume failure initially
            cas_done_[key] = false; // Result not ready yet
        }

        // Get subscribers and prepare message
        std::set<int> subscribers = getSubscribers(variable_id);
        std::vector<int> buffer = {(int)MessageType::CAS, variable_id, new_value, expected, rank_, msg_id, timestamp};

        // Broadcast CAS to all subscribers
        for (int dest : subscribers) {
            if (dest == rank_) continue;
            MPI_Send(buffer.data(), buffer.size(), MPI_INT, dest, 0, MPI_COMM_WORLD);
        }

        if (verbose_) {
            log("CAS var=" + std::to_string(variable_id) +
                " expected=" + std::to_string(expected) +
                " new=" + std::to_string(new_value) + " | T=" + std::to_string(timestamp));
        }

        // BLOCK HERE: Wait until background thread delivers this CAS and sets result
        std::unique_lock<std::mutex> lock(cas_mutex_);
        cas_cv_.wait(lock, [this, key] { return cas_done_[key]; }); // Sleep until cas_done_[key] == true

        // Result is ready, read it
        bool success = cas_result_[key];

        // Cleanup tracking maps
        cas_done_.erase(key);
        cas_result_.erase(key);

        if (verbose_) {
            log("CAS result: " + std::string(success ? "SUCCESS" : "FAILED"));
        }

        return success; // Return whether swap happened
    }

    /**
     * Get current Lamport clock value (for debugging/testing).
     */
    int getLamportClock() const {
        return lamport_clock_.load();
    }

private:
    /**
     * Increment logical clock (called on every local event).
     */
    void incrementClock() {
        lamport_clock_++; // Atomic increment
    }

    /**
     * Get the set of subscribers for a variable (thread-safe).
     */
    std::set<int> getSubscribers(int variable_id) const {
        std::lock_guard<std::mutex> lock(subscriptions_mutex_);
        auto it = subscriptions_.find(variable_id);
        if (it == subscriptions_.end()) return {}; // No subscribers
        return it->second; // Return copy of subscriber set
    }

    /**
     * Verify this process is subscribed to a variable (throws if not).
     */
    void ensureSubscribed(int variable_id) const {
        std::lock_guard<std::mutex> lock(subscriptions_mutex_);
        auto it = subscriptions_.find(variable_id);

        // Check 1: Is variable in map?
        // Check 2: Is our rank in the subscriber set?
        if (it == subscriptions_.end() || it->second.find(rank_) == it->second.end()) {
            throw std::runtime_error("Process not subscribed to variable " + std::to_string(variable_id));
        }
    }

    /**
     * Background thread main loop: continuously poll and process messages.
     */
    void messageProcessingLoop() {
        while (running_) {
            processMessages(); // Do the actual work
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Sleep to avoid 100% CPU
        }
    }

    /**
     * Process all available MPI messages and attempt delivery.
     */
    void processMessages() {
        int flag = 0;
        MPI_Status status;

        // Non-blocking check: is there a message waiting?
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);

        // Process all available messages
        while (flag && running_) {
            // Get message size
            int count = 0;
            MPI_Get_count(&status, MPI_INT, &count);

            // Allocate buffer and receive message
            std::vector<int> buffer(count);
            MPI_Recv(buffer.data(), count, MPI_INT, status.MPI_SOURCE, status.MPI_TAG,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Unpack message fields
            MessageType type = (MessageType)buffer[0];
            int var_id = buffer[1];
            int new_value = buffer[2];
            int expected = buffer[3];
            int original_sender = buffer[4];
            int msg_id = buffer[5];
            int msg_timestamp = buffer[6];

            // Update Lamport clock: max(local, received) + 1
            // This is the key Lamport clock rule for receiving messages
            lamport_clock_ = std::max(lamport_clock_.load(), msg_timestamp) + 1;

            // Route to appropriate handler based on message type
            if (type == MessageType::WRITE || type == MessageType::CAS) {
                handleOperationMessage(type, var_id, new_value, expected,
                                       original_sender, msg_id, msg_timestamp);
            } else if (type == MessageType::ACK) {
                handleAckMessage(var_id, original_sender, msg_id, status.MPI_SOURCE);
            }

            // Check if more messages arrived while we were processing
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        }

        // After processing all incoming messages, try to deliver queued operations
        deliverPendingMessages();
    }

    /**
     * Handle incoming WRITE or CAS operation from another process.
     */
    void handleOperationMessage(MessageType type, int var_id, int new_value, int expected, int original_sender, int msg_id, int msg_timestamp) {
        // Get subscriber list for this variable
        std::set<int> subscribers = getSubscribers(var_id);

        // Ignore if we're not subscribed (shouldn't happen, but defensive)
        if (subscribers.empty() || subscribers.find(rank_) == subscribers.end()) return;

        // Package as pending message
        PendingMessage msg{msg_timestamp, original_sender, msg_id, type, var_id, new_value, expected};

        {
            // Add to priority queue (will be sorted by timestamp)
            std::lock_guard<std::mutex> lock(pending_mutex_);
            pending_messages_.push(msg);
        }

        // Track ACKs for this message
        auto key = std::make_pair(original_sender, msg_id);
        {
            std::lock_guard<std::mutex> lock(ack_mutex_);
            ack_sets_[key].insert(original_sender); // Original sender implicitly ACKed
            ack_sets_[key].insert(rank_); // We've now seen it
        }

        // Prepare ACK message to send to all other subscribers
        std::vector<int> ack_buffer = {(int)MessageType::ACK, var_id, 0, 0,
                                       original_sender, msg_id, lamport_clock_.load()};

        // Send ACK to all subscribers (except ourselves)
        for (int dest : subscribers) {
            if (dest == rank_) continue;
            MPI_Send(ack_buffer.data(), ack_buffer.size(), MPI_INT, dest, 0, MPI_COMM_WORLD);
        }

        if (verbose_) {
            log("RECEIVE " + std::string(type == MessageType::WRITE ? "WRITE" : "CAS") +
                " from " + std::to_string(original_sender) +
                " var=" + std::to_string(var_id) +
                " T=" + std::to_string(msg_timestamp));
        }
    }

    /**
     * Handle incoming ACK message (someone acknowledged an operation).
     */
    void handleAckMessage(int var_id, int original_sender, int msg_id, int ack_sender) {
        // Check if we're subscribed (ignore if not)
        std::set<int> subscribers = getSubscribers(var_id);
        if (subscribers.empty() || subscribers.find(rank_) == subscribers.end()) return;

        // Update ACK set for this message
        auto key = std::make_pair(original_sender, msg_id);
        {
            std::lock_guard<std::mutex> lock(ack_mutex_);
            ack_sets_[key].insert(original_sender); // Ensure originator is marked
            ack_sets_[key].insert(ack_sender); // Add this ACK sender
        }
    }

    /**
     * Check if all subscribers have ACKed a message (ready for delivery).
     */
    bool haveAllAcks(const PendingMessage& msg) const {
        // Get subscriber list
        std::set<int> subscribers = getSubscribers(msg.var_id);
        if (subscribers.empty()) return false;

        // Look up ACK set for this message
        auto key = std::make_pair(msg.sender, msg.msg_id);
        std::lock_guard<std::mutex> lock(ack_mutex_);
        auto ack_it = ack_sets_.find(key);
        if (ack_it == ack_sets_.end()) return false; // No ACKs yet

        const auto& ack_set = ack_it->second;

        // Check if every subscriber has ACKed
        for (int subscriber : subscribers) {
            if (ack_set.find(subscriber) == ack_set.end()) {
                return false; // This subscriber hasn't ACKed yet
            }
        }

        return true; // All subscribers have ACKed, ready to deliver!
    }

    /**
     * Try to deliver all messages at head of queue that have complete ACK sets.
     */
    void deliverPendingMessages() {
        while (true) {
            PendingMessage msg;
            bool deliver = false;

            {
                std::lock_guard<std::mutex> lock(pending_mutex_);

                // Check if queue is empty
                if (pending_messages_.empty()) break;

                // Look at message with smallest timestamp (head of priority queue)
                const PendingMessage& top = pending_messages_.top();

                // Can we deliver it? (all subscribers ACKed?)
                if (!haveAllAcks(top)) break;  // Not ready, stop trying

                // Yes! Extract it for delivery
                msg = top;
                pending_messages_.pop();
                deliver = true;
            }

            // Deliver outside the lock (to avoid holding lock during callback)
            if (!deliver) break;
            deliverMessage(msg);
        }
    }

    /**
     * Actually apply a DSM operation: update variable and fire callback.
     */
    void deliverMessage(const PendingMessage& msg) {
        int old_value;

        {
            // Thread-safely access and modify the variable
            std::lock_guard<std::mutex> lock(variable_mutex_);
            old_value = variables_[msg.var_id]; // Save old value for callback

            bool success = true;

            if (msg.type == MessageType::WRITE) {
                // WRITE: always succeeds, just update the value
                variables_[msg.var_id] = msg.new_value;

            } else if (msg.type == MessageType::CAS) {
                // CAS: check if current value matches expected
                if (variables_[msg.var_id] == msg.expected) {
                    // Match! Do the swap
                    variables_[msg.var_id] = msg.new_value;
                    success = true;
                } else {
                    // No match, CAS fails, don't change value
                    success = false;
                }

                // If this process originated the CAS, record the result
                if (msg.sender == rank_) {
                    auto key = std::make_pair(msg.sender, msg.msg_id);
                    std::lock_guard<std::mutex> lock_cas(cas_mutex_);
                    cas_result_[key] = success; // Store result
                    cas_done_[key] = true; // Mark as ready
                    cas_cv_.notify_all(); // Wake up waiting thread
                }
            }
        }

        // Call user callback (outside variable lock to avoid deadlock)
        invokeCallback(msg.var_id, old_value, variables_[msg.var_id], msg.timestamp);
    }

    /**
     * Invoke user callback (thread-safe).
     */
    void invokeCallback(int var_id, int old_value, int new_value, int ts) {
        std::lock_guard<std::mutex> lock(callback_mutex_);
        if (change_callback_) {
            // Call user's registered callback function
            change_callback_(var_id, old_value, new_value, ts);
        }
    }

    /**
     * Print debug log message.
     */
    void log(const std::string& message) const {
        std::cout << "[Rank " << rank_ << "] " << message << std::endl << std::flush;
    }
};

