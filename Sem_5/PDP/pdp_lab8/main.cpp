#include "dsm.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mpi.h>

// Event log for multiple-rank consistency verification
struct EventLog {
    int rank;
    int variable_id;
    int old_value;
    int new_value;
    int lamport_time;
};

std::vector<EventLog> event_sequence;

// Scenario 1: Concurrent writes to a globally shared variable (var 0)
void runScenario1(DistributedSharedMemory& dsm, int rank) {
    std::cout << "[Rank " << rank << "] Scenario 1: Sequential Consistency\n";
    if (rank == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        dsm.write(0, 250);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        dsm.write(0, 500);
    } else if (rank == 1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        dsm.write(0, 375);
    } else if (rank == 2) {
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
        dsm.write(0, 300);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

// Scenario 2: Write to a variable (var 2) subscribed ONLY by ranks {0,2,3}
void runPartialSubscriptionScenario(DistributedSharedMemory& dsm, int rank) {
    std::cout << "[Rank " << rank << "] Scenario 2: Partial Subscription (var2 local group)\n";
    if (rank == 0) {
        dsm.write(2, 111);
    } else if (rank == 2) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        dsm.write(2, 222);
    } else if (rank == 3) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        dsm.write(2, 333);
    }
    // Rank 1 is NOT subscribed, does not write var2!
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

// Scenario 3: var 1 is a global variable, all processes write in sequence
void runGlobalVarsScenario(DistributedSharedMemory& dsm, int rank) {
    if (rank == 1) {
        dsm.write(1, 555);
    } else if (rank == 2) {
        std::this_thread::sleep_for(std::chrono::milliseconds(70));
        dsm.write(1, 999);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

// Scenario 4: CAS atomicity demonstration on var 3
void runScenario3(DistributedSharedMemory& dsm, int rank) {
    std::cout << "[Rank " << rank << "] Scenario 3: CAS Atomicity\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(35 * rank));
    int candidate = 50 + 25 * rank;
    bool success = dsm.compareAndSwap(3, 0, candidate);
    std::cout << "[Rank " << rank << "] CAS(3, 0, " << candidate << ") = "
              << (success ? "SUCCESS" : "FAILED") << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    if (rank == 2) {
        int v = dsm.read(3);
        std::cout << "[Rank 2] After first CAS, var 3 = " << v << "\n";
        success = dsm.compareAndSwap(3, v, 1234);
        std::cout << "[Rank 2] CAS(3, " << v << ", 1234) = "
                  << (success ? "SUCCESS" : "FAILED") << "\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

// Scenario 5: Demonstrates Lamport happens-before on var 4
void runScenario4(DistributedSharedMemory& dsm, int rank) {
    std::cout << "[Rank " << rank << "] Scenario 4: Happens-Before\n";
    if (rank == 0) {
        dsm.write(4, 7);
        std::cout << "[Rank 0] Event A: write(4, 7) | Clock=" << dsm.getLamportClock() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
        dsm.write(4, 14);
        std::cout << "[Rank 0] Event B: write(4, 14) | Clock=" << dsm.getLamportClock() << "\n";
    } else if (rank == 1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        dsm.write(4, 21);
        std::cout << "[Rank 1] Event C: write(4, 21) | Clock=" << dsm.getLamportClock() << "\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

// Utility: Which variables are globally subscribed by all ranks
static bool is_globally_shared_var(int var_id) {
    return var_id == 0 || var_id == 1 || var_id == 3 || var_id == 4;
}

// Global consistency check for callback sequence (should match for all ranks)
void verifySequentialConsistency(int rank, int world_size) {
    int local_event_count = 0;
    for (const auto& e : event_sequence)
        if (is_globally_shared_var(e.variable_id)) local_event_count++;
    std::vector<int> all_counts(world_size);
    MPI_Gather(&local_event_count, 1, MPI_INT, all_counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        std::cout << "\n=== SEQUENTIAL CONSISTENCY CHECK ===\n";
        for (int i = 0; i < world_size; ++i)
            std::cout << "Rank " << i << ": " << all_counts[i] << " events\n";
        bool consistent = true;
        for (int i = 1; i < world_size; ++i)
            if (all_counts[i] != all_counts[0]) consistent = false;
        std::cout << (consistent ? "Consistency: PASSED\n" : "Consistency: FAILED\n");
        std::cout << "Event sequence on rank 0 (globals):\n";
        for (const auto& e : event_sequence)
            if (is_globally_shared_var(e.variable_id))
                std::cout << " [T=" << e.lamport_time << "] Var " << e.variable_id
                          << ": " << e.old_value << " -> " << e.new_value
                          << " (rank " << e.rank << ")\n";
        std::cout << "====================================\n";
    }
}

int main(int argc, char** argv) {
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Ensure required number of ranks
    if (world_size != NUM_PROCESSES) {
        if (rank == 0)
            std::cerr << "Error: Requires exactly " << NUM_PROCESSES << " MPI processes!\n";
        MPI_Finalize();
        return 1;
    }

    // DSM with local replication of all variables, Lamport consistency
    DistributedSharedMemory dsm(rank, world_size, false);

    // Build subscription groups
    std::set<int> all_ranks;
    for (int i = 0; i < world_size; ++i) all_ranks.insert(i);
    std::set<int> subs_var2 = {0, 2, 3};

    dsm.subscribe(0, all_ranks);
    dsm.subscribe(1, all_ranks);
    if (subs_var2.count(rank)) dsm.subscribe(2, subs_var2);
    dsm.subscribe(3, all_ranks);
    dsm.subscribe(4, all_ranks);

    // Logging callback for every variable update
    dsm.setChangeCallback([rank](int var_id, int old_val, int new_val, int lamport_time) {
        std::cout << "[Rank " << rank << "] CALLBACK: Var " << var_id
                  << ": " << old_val << " -> " << new_val
                  << " | T=" << lamport_time << "\n";
        event_sequence.push_back({rank, var_id, old_val, new_val, lamport_time});
    });

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        std::cout << "\n=============================================\n";
        std::cout << "DISTRIBUTED SHARED MEMORY - LAB 8\n";
        std::cout << "Lamport Total-Order Multicast, Replicated Model\n";
        std::cout << "Processes: " << world_size << "\n";
        std::cout << "Variables per process: " << NUM_VARIABLES << "\n";
        std::cout << "=============================================\n\n";
    }
    MPI_Barrier(MPI_COMM_WORLD);

    // Global concurrent write
    runScenario1(dsm, rank);
    MPI_Barrier(MPI_COMM_WORLD);

    // Local group write
    runPartialSubscriptionScenario(dsm, rank);
    MPI_Barrier(MPI_COMM_WORLD);

    // All ranks may write to a global variable
    runGlobalVarsScenario(dsm, rank);
    MPI_Barrier(MPI_COMM_WORLD);

    // CAS atomicity
    runScenario3(dsm, rank);
    MPI_Barrier(MPI_COMM_WORLD);

    // Happens-before
    runScenario4(dsm, rank);
    MPI_Barrier(MPI_COMM_WORLD);

    verifySequentialConsistency(rank, world_size);

    if (rank == 0) {
        std::cout << "\n=== Final Variable Values (Rank 0 view): ===\n";
        for (int i = 0; i < NUM_VARIABLES; ++i)
            std::cout << "  Variable " << i << ": " << dsm.read(i) << "\n";
        std::cout << "  Lamport Clock: " << dsm.getLamportClock() << "\n";
        std::cout << "=============================================\n";
    }

    MPI_Finalize();
    return 0;
}
