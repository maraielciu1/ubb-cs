#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <chrono>
#include <cstdint>
#include <algorithm>

using namespace std;

struct BoundedQueue {
    explicit BoundedQueue(int capacity_) : capacity(max<int>(1, capacity_)) {}

    void push(long long x) {
        unique_lock<mutex> lock(mtx);
        cv_not_full.wait(lock, [&]{ return q.size() < capacity; });
        q.push(x);
        lock.unlock();
        cv_not_empty.notify_one();
    }

    long long pop() {
        unique_lock<mutex> lock(mtx);
        cv_not_empty.wait(lock, [&]{ return !q.empty(); });
        long long x = q.front();
        q.pop();
        lock.unlock();
        cv_not_full.notify_one();
        return x;
    }

    queue<long long> q;
    const int capacity;
    mutex mtx;
    condition_variable cv_not_full, cv_not_empty;
};

void producer(const vector<int>& a, const vector<int>& b, BoundedQueue& bq) {
    const int n = min(a.size(), b.size());
    for (int i = 0; i < n; ++i) {
        long long prod = 1LL * a[i] * b[i];
        bq.push(prod);
    }
}

void consumer(BoundedQueue& bq, int count, long long& result) {
    long long sum = 0;
    for (int i = 0; i < count; ++i) {
        sum += bq.pop();
    }
    result = sum;
}

int main() {
    const int N = 100000;
    vector<int> a, b;
    a.reserve(N);
    b.reserve(N);
    for (int i = 1; i <= N; ++i) {
        a.push_back(i % 10);
        b.push_back(i % 10);
    }

    cout << "Vector size: " << N << "\n";
    cout << "Testing different queue sizes\n";

    vector<int> capacities = {1, 8, 64, 512, 4096};

    for (int capacity : capacities) {
        long long result = 0;
        BoundedQueue bq(capacity);

        auto t0 = chrono::high_resolution_clock::now();

        thread tp(producer, cref(a), cref(b), ref(bq));
        thread tc(consumer, ref(bq), min(a.size(), b.size()), ref(result));

        tp.join();
        tc.join();

        auto t1 = chrono::high_resolution_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();

        cout << "Queue size: " << capacity
             << " | Result: " << result
             << " | Time: " << ms << " ms" << endl;
    }

    cout << "----------------------------------------\n";
    return 0;
}
