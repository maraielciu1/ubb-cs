#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <mutex>
#include <algorithm>

#define DEBUG_PRINT 0

const int DEFAULT_SIZE = 512;
const int DEFAULT_THREADS = 4;

std::vector<std::vector<double>> A;
std::vector<std::vector<double>> B;
std::vector<std::vector<double>> C;

int N;
int NUM_THREADS;

std::mutex print_mutex;

using namespace std::chrono;

void compute_element(int row, int col, int thread_id) {
    if (DEBUG_PRINT) {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "[T" << thread_id << "] Computing C[" << row << "][" << col << "]\n";
    }

    double sum = 0.0;
    for (int k = 0; k < N; ++k) {
        sum += A[row][k] * B[k][col];
    }
    C[row][col] = sum;
}

void thread_func_consecutive_elements_row_major(int thread_id) {
    long long total_elements = (long long)N * N;
    long long elements_per_thread = total_elements / NUM_THREADS;
    long long remainder = total_elements % NUM_THREADS;

    long long start_k = thread_id * elements_per_thread + std::min((long long)thread_id, remainder);
    long long end_k = start_k + elements_per_thread;
    if (thread_id < remainder) {
        end_k += 1;
    }

    for (long long k = start_k; k < end_k; ++k) {
        int row = k / N;
        int col = k % N;
        compute_element(row, col, thread_id);
    }
}

void thread_func_consecutive_elements_col_major(int thread_id) {
    long long total_elements = (long long)N * N;
    long long elements_per_thread = total_elements / NUM_THREADS;
    long long remainder = total_elements % NUM_THREADS;

    long long start_k = thread_id * elements_per_thread + std::min((long long)thread_id, remainder);
    long long end_k = start_k + elements_per_thread;
    if (thread_id < remainder) {
        end_k += 1;
    }

    for (long long k = start_k; k < end_k; ++k) {
        int col = k / N;
        int row = k % N;
        compute_element(row, col, thread_id);
    }
}

void thread_func_interleaved(int thread_id) {
    long long total_elements = (long long)N * N;

    for (long long k = thread_id; k < total_elements; k += NUM_THREADS) {
        int row = k / N;
        int col = k % N;
        compute_element(row, col, thread_id);
    }
}

void initialize_matrices() {
    A.assign(N, std::vector<double>(N));
    B.assign(N, std::vector<double>(N));
    C.assign(N, std::vector<double>(N, 0.0));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = (double)(i + j) / N;
            B[i][j] = (double)(i * 2 + j) / N;
        }
    }
}

bool verify_result() {
    std::vector<std::vector<double>> C_seq(N, std::vector<double>(N, 0.0));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (int k = 0; k < N; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C_seq[i][j] = sum;
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (std::abs(C[i][j] - C_seq[i][j]) > 1e-9) {
                std::cerr << "Verification failed at C[" << i << "][" << j << "]. Thread result: "
                          << C[i][j] << ", Expected: " << C_seq[i][j] << std::endl;
                return false;
            }
        }
    }
    return true;
}

long long run_strategy(void (*func)(int), const std::string& strategy_name, bool print_output = true) {
    initialize_matrices();
    std::vector<std::thread> threads;

    auto start_time = high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(func, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = high_resolution_clock::now();
    long long duration = duration_cast<milliseconds>(end_time - start_time).count();

    if (print_output) {
        std::cout << "\nStrategy: " << strategy_name << " (N=" << N << ", T=" << NUM_THREADS << ") ---\n";
        std::cout << "Computation Time: " << duration << " ms\n";
    }

    if (!verify_result()) {
        std::cerr << "ERROR: Result verification failed for this strategy.\n";
    }

    return duration;
}

int main() {
    std::cout << std::fixed << std::setprecision(2);

    std::vector<std::pair<int, int>> experiments = {
            {128, 4},
            {512, 4},
            {512, 8},
            {1024, 8},
            {1024, 16}
    };

    std::cout << std::setw(15) << "N | T"
              << std::setw(25) << "1. Row-Major (ms)"
              << std::setw(25) << "2. Col-Major (ms)"
              << std::setw(25) << "3. Interleaved (ms)"
              << std::endl;
    std::cout << "---------------------------------------------------------------------------------------\n";

    for (const auto& exp : experiments) {
        N = exp.first;
        NUM_THREADS = exp.second;

        std::string n_t_label = std::to_string(N) + " | " + std::to_string(NUM_THREADS);
        std::cout << std::setw(15) << n_t_label;
        std::cout.flush();

        long long time1 = run_strategy(thread_func_consecutive_elements_row_major, "1. Row-Major", false);
        std::cout << std::setw(25) << time1;
        std::cout.flush();

        long long time2 = run_strategy(thread_func_consecutive_elements_col_major, "2. Col-Major", false);
        std::cout << std::setw(25) << time2;
        std::cout.flush();

        long long time3 = run_strategy(thread_func_interleaved, "3. Interleaved", false);
        std::cout << std::setw(25) << time3 << std::endl;
    }

    return 0;
}

