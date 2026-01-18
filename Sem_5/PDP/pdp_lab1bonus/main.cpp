#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <memory>
#include <random>
#include <chrono>
#include <cassert>

using namespace std;

struct Node {
    int value;
    Node* prev;
    Node* next;
    mutex mtx;

    Node(int v = 0) : value(v), prev(nullptr), next(nullptr) {}
};

struct DLList {
    Node* head;
    Node* tail;

    DLList() {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }

    ~DLList() {
        Node* cur = head;
        while (cur) {
            Node* nx = cur->next;
            delete cur;
            cur = nx;
        }
    }

    Node* move_next(Node* cur) {
        // Optional: lock_guard<mutex> lk(cur->mtx);
        return cur->next;
    }

    Node* move_prev(Node* cur) {
        // Optional: lock_guard<mutex> lk(cur->mtx);
        return cur->prev;
    }

    // Lock two node mutexes in ascending address order (deadlock-free).
    static void lock_pair(Node* a, Node* b) {
        if (a == b) { a->mtx.lock(); return; }
        Node* first  = (a < b) ? a : b;
        Node* second = (a < b) ? b : a;
        first->mtx.lock();
        second->mtx.lock();
    }
    static void unlock_pair(Node* a, Node* b) {
        if (a == b) { a->mtx.unlock(); return; }
        Node* first  = (a < b) ? a : b;
        Node* second = (a < b) ? b : a;
        second->mtx.unlock();
        first->mtx.unlock();
    }

    Node* insert_after(Node* cur, int val) {
        while (true) {
            Node* right = cur->next;

            lock_pair(cur, right);

            if (cur->next == right && right->prev == cur) {
                Node* n = new Node(val);

                // Link n between cur and right
                n->prev = cur;
                n->next = right;
                cur->next = n;
                right->prev = n;

                unlock_pair(cur, right);
                return n;
            } else {
                // Adjacency changed: retry
                unlock_pair(cur, right);
                // Small backoff to reduce livelock probability
                std::this_thread::yield();
            }
        }
    }

    Node* insert_before(Node* cur, int val) {
        while (true) {
            Node* left = cur->prev;

            // Lock the adjacent pair (left, cur) in address order
            lock_pair(left, cur);

            // Validate adjacency still holds after locking
            if (left->next == cur && cur->prev == left) {
                Node* n = new Node(val);

                // Link n between left and cur
                n->prev = left;
                n->next = cur;
                left->next = n;
                cur->prev = n;

                unlock_pair(left, cur);
                return n;
            } else {
                unlock_pair(left, cur);
                std::this_thread::yield();
            }
        }
    }

    bool is_consistent() const {
        // Forward pass: prev/next pointers match
        const Node* f = head;
        const Node* last = nullptr;
        while (f) {
            if (f->prev != last) return false;
            last = f;
            f = f->next;
        }
        // Backward pass: next/prev pointers match
        const Node* b = tail;
        const Node* nxt = nullptr;
        while (b) {
            if (b->next != nxt) return false;
            nxt = b;
            b = b->prev;
        }
        return true;
    }

    // Count nodes (excluding sentinels) — call when quiescent or externally synchronized
    size_t size_st() const {
        size_t cnt = 0;
        const Node* cur = head->next;
        while (cur && cur != tail) { ++cnt; cur = cur->next; }
        return cnt;
    }
};


int main() {
//    ios::sync_with_stdio(false);
//    cin.tie(nullptr);

    DLList list;

    Node* a = list.insert_after(list.head, 1);
    Node* b = list.insert_after(a, 2);

    const int THREADS = 8;
    const int OPS_PER_THREAD = 20000;

    vector<thread> threads;

    auto start = chrono::steady_clock::now();

    for (int t = 0; t < THREADS; ++t) {
        threads.emplace_back([&list]() {
            Node* cur = list.head;

            for (int i = 0; i < OPS_PER_THREAD; ++i) {
                // Randomly move next or prev
                if (rand() % 2 == 0) {
                    Node* nxt = list.move_next(cur);
                    if (nxt) cur = nxt;
                } else {
                    Node* prv = list.move_prev(cur);
                    if (prv) cur = prv;
                }

                // Randomly choose a value and insert before/after
                int value = rand() % 1000;
                if (rand() % 2 == 0) {
                    if (cur != list.head)
                        list.insert_before(cur, value);
                    else
                        list.insert_after(cur, value);
                } else {
                    if (cur != list.tail)
                        list.insert_after(cur, value);
                    else
                        list.insert_before(cur, value);
                }
            }
        });
    }

    for (auto& th : threads) th.join();

    auto end = chrono::steady_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "Concurrent operations finished in " << ms << " ms\n";
    cout << "List consistent? " << (list.is_consistent() ? "YES" : "NO") << "\n";
    cout << "List size: " << list.size_st() << "\n";

    cout << "\nDocumentation: Mutexes & Invariants\n";
    cout << "Each Node has its own mutex protecting its links.\n";
    cout << "Inserts lock only two neighbors in ascending address order. \n";
    cout << "Independent inserts on distant parts of the list proceed concurrently.\n";

    return 0;
}
