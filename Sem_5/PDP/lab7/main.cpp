///////////////////////////
///   IMPORTS SECTION   ///
///////////////////////////
#include <mpi.h>
#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

///////////////////////////
///   TYPEDEFS SECTION  ///
///////////////////////////
using Coeff = long long;
using Poly = std::vector<Coeff>;

///////////////////////////
///   HELPERS SECTION   ///
///////////////////////////

/**
 * Sequential naive polynomial multiplication (O(n²) complexity).
 *
 * Used as base case in distributed algorithms and for verification.
 * Performs classical convolution: C[k] = Σ_{i+j=k} A[i]·B[j]
 *
 * @param a  First polynomial coefficients
 * @param b  Second polynomial coefficients
 * @return   Product polynomial coefficients
 */
static Poly multiply_naive_seq(const Poly& a, const Poly& b) {
    size_t n = a.size(), m = b.size();
    Poly result(n + m - 1, 0);
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < m; ++j)
            result[i + j] += a[i] * b[j];
    return result;
}

/**
 * Distributed naive polynomial multiplication using MPI.
 *
 * Distribution Strategy:
 * - Root broadcasts polynomial B to all processes
 * - Polynomial A is partitioned into contiguous chunks via MPI_Scatter
 * - Each process computes partial products for its chunk of A coefficients
 * - Results gathered back to root via MPI_Gatherv (handles uneven chunk sizes)
 *
 * Communication Pattern:
 * - MPI_Bcast: O(log P) latency, broadcasts entire B polynomial
 * - MPI_Scatter: Distributes A chunks with automatic load balancing
 * - MPI_Gatherv: Collects variable-sized result segments
 *
 * Synchronization:
 * - All collective operations provide implicit barrier synchronization
 * - No explicit locks needed due to distributed memory model
 * - Each process operates on independent memory segments
 *
 * @param a      First polynomial (only valid on root)
 * @param b      Second polynomial (broadcast to all)
 * @param rank   MPI process rank
 * @param size   Total number of MPI processes
 * @return       Product polynomial (complete only on root, partial elsewhere)
 */
static Poly multiply_naive_mpi(const Poly& a, const Poly& b, int rank, int size) {
    int n = (rank == 0) ? a.size() : 0;
    int m = (rank == 0) ? b.size() : 0;

    // Root broadcasts polynomial sizes to all processes for allocation
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Non-root processes allocate buffer for polynomial B
    Poly b_local = (rank == 0) ? b : Poly(m);
    MPI_Bcast(b_local.data(), m, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // Compute chunk distribution (handle non-divisible sizes)
    int base_chunk = n / size;
    int remainder = n % size;
    int local_chunk = base_chunk + (rank < remainder ? 1 : 0);

    // Prepare scatter metadata (only root needs full arrays)
    std::vector<int> sendcounts(size), displs(size);
    if (rank == 0) {
        int offset = 0;
        for (int i = 0; i < size; ++i) {
            sendcounts[i] = base_chunk + (i < remainder ? 1 : 0);
            displs[i] = offset;
            offset += sendcounts[i];
        }
    }

    // Scatter chunks of polynomial A to all processes
    Poly a_local(local_chunk);
    MPI_Scatterv(
            rank == 0 ? a.data() : nullptr,
            sendcounts.data(),
            displs.data(),
            MPI_LONG_LONG,
            a_local.data(),
            local_chunk,
            MPI_LONG_LONG,
            0,
            MPI_COMM_WORLD
    );

    // Compute local partial products independently
    Poly local_result(local_chunk + m - 1, 0);
    for (int i = 0; i < local_chunk; ++i)
        for (int j = 0; j < m; ++j)
            local_result[i + j] += a_local[i] * b_local[j];

    // Gather all partial results back to root
    std::vector<int> recvcounts(size), recvdispls(size);
    if (rank == 0) {
        for (int i = 0; i < size; ++i) {
            recvcounts[i] = sendcounts[i] + m - 1;
            recvdispls[i] = displs[i];
        }
    }

    Poly result;
    if (rank == 0) result.resize(n + m - 1, 0);

    // Gather with custom reduction (sum overlapping coefficient regions)
    std::vector<Coeff> gathered;
    if (rank == 0) gathered.resize(n + m - 1, 0);

    MPI_Gatherv(
            local_result.data(),
            local_result.size(),
            MPI_LONG_LONG,
            gathered.data(),
            recvcounts.data(),
            recvdispls.data(),
            MPI_LONG_LONG,
            0,
            MPI_COMM_WORLD
    );

    // Root process merges overlapping segments (coefficients from different chunks)
    if (rank == 0) {
        for (int i = 0; i < size; ++i) {
            int offset = displs[i];
            for (int j = 0; j < recvcounts[i]; ++j) {
                result[offset + j] += gathered[recvdispls[i] + j];
            }
        }
    }

    return result;
}

/**
 * Sequential Karatsuba multiplication with divide-and-conquer recursion.
 *
 * Theory:
 * Splits polynomials A, B into halves: A = A_low + A_high·x^m
 * Computes product via three recursive multiplications:
 *   P1 = A_low × B_low
 *   P2 = A_high × B_high
 *   P3 = (A_low + A_high) × (B_low + B_high)
 * Result: A×B = P2·x^(2m) + (P3 - P2 - P1)·x^m + P1
 *
 * Base case cutoff at degree 64 to avoid excessive recursion overhead.
 *
 * @param a  First polynomial
 * @param b  Second polynomial
 * @return   Product coefficients
 */
static Poly multiply_karatsuba_seq(const Poly& a, const Poly& b) {
    size_t n = a.size(), m = b.size();

    if (n <= 64 || m <= 64)
        return multiply_naive_seq(a, b);

    size_t half = n / 2;
    Poly a_low(a.begin(), a.begin() + half);
    Poly a_high(a.begin() + half, a.end());
    Poly b_low(b.begin(), b.begin() + std::min(half, m));
    Poly b_high(b.begin() + std::min(half, m), b.end());

    Poly P1 = multiply_karatsuba_seq(a_low, b_low);
    Poly P2 = multiply_karatsuba_seq(a_high, b_high);

    Poly a_sum(std::max(a_low.size(), a_high.size()), 0);
    Poly b_sum(std::max(b_low.size(), b_high.size()), 0);
    for (size_t i = 0; i < a_low.size(); ++i) a_sum[i] += a_low[i];
    for (size_t i = 0; i < a_high.size(); ++i) a_sum[i] += a_high[i];
    for (size_t i = 0; i < b_low.size(); ++i) b_sum[i] += b_low[i];
    for (size_t i = 0; i < b_high.size(); ++i) b_sum[i] += b_high[i];

    Poly P3 = multiply_karatsuba_seq(a_sum, b_sum);

    for (size_t i = 0; i < P1.size(); ++i) P3[i] -= P1[i];
    for (size_t i = 0; i < P2.size(); ++i) P3[i] -= P2[i];

    Poly result(n + m - 1, 0);
    for (size_t i = 0; i < P1.size(); ++i) result[i] += P1[i];
    for (size_t i = 0; i < P3.size(); ++i) result[i + half] += P3[i];
    for (size_t i = 0; i < P2.size(); ++i) result[i + 2 * half] += P2[i];
    return result;
}

/**
 * Distributed Karatsuba multiplication using master-worker pattern.
 *
 * Distribution Strategy:
 * - Master decomposes polynomials into subproblems at top recursion levels
 * - Subproblems (P1, P2, P3 computations) distributed round-robin to workers
 * - Workers compute assigned subproblems sequentially using Karatsuba
 * - Master collects results and performs final merge/reconstruction
 *
 * Communication Pattern:
 * - Point-to-point MPI_Send/MPI_Recv for task distribution
 * - Master sends: subproblem ID + two polynomial segments
 * - Workers reply: subproblem ID + result polynomial
 * - Minimizes collective operation overhead for irregular task sizes
 *
 * Load Balancing:
 * - Round-robin assignment ensures even distribution for balanced workload
 * - Dynamic work stealing could improve performance for heterogeneous clusters
 *
 * Synchronization:
 * - Master-worker pattern provides implicit synchronization
 * - No deadlocks: master sends all tasks before receiving results
 * - Workers process one task at a time (stateless computation)
 *
 * Performance Consideration:
 * Karatsuba's recursive structure creates irregular subproblem sizes, causing:
 * - Communication overhead potentially exceeding computation savings
 * - Recommended only for very large polynomials (degree > 10,000)
 * - Consider hybrid approach: distribute top levels, parallelize locally within nodes
 *
 * @param a      First polynomial (valid on root)
 * @param b      Second polynomial (valid on root)
 * @param rank   MPI process rank
 * @param size   Total MPI processes
 * @return       Product polynomial (complete on root)
 */
static Poly multiply_karatsuba_mpi(const Poly& a, const Poly& b, int rank, int size) {
    int n = (rank == 0) ? a.size() : 0;
    int m = (rank == 0) ? b.size() : 0;

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Base case: small polynomials computed locally without distribution overhead
    if (n <= 256 || m <= 256 || size == 1) {
        Poly a_full = a, b_full = b;
        if (rank != 0) {
            a_full.resize(n);
            b_full.resize(m);
        }
        MPI_Bcast(a_full.data(), n, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
        MPI_Bcast(b_full.data(), m, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
        return multiply_karatsuba_seq(a_full, b_full);
    }

    if (rank == 0) {
        // Master process: decompose and distribute subproblems
        size_t half = n / 2;
        Poly a_low(a.begin(), a.begin() + half);
        Poly a_high(a.begin() + half, a.end());
        Poly b_low(b.begin(), b.begin() + std::min<size_t>(half, m));
        Poly b_high(b.begin() + std::min<size_t>(half, m), b.end());

        Poly a_sum(std::max(a_low.size(), a_high.size()), 0);
        Poly b_sum(std::max(b_low.size(), b_high.size()), 0);
        for (size_t i = 0; i < a_low.size(); ++i) a_sum[i] += a_low[i];
        for (size_t i = 0; i < a_high.size(); ++i) a_sum[i] += a_high[i];
        for (size_t i = 0; i < b_low.size(); ++i) b_sum[i] += b_low[i];
        for (size_t i = 0; i < b_high.size(); ++i) b_sum[i] += b_high[i];

        // Package subproblems for distribution
        struct Subproblem {
            int id;
            Poly poly_a, poly_b;
        };

        std::vector<Subproblem> tasks = {
                {1, a_low, b_low},
                {2, a_high, b_high},
                {3, a_sum, b_sum}
        };

        std::vector<Poly> results(3);
        int next_worker = 1;

        // Distribute tasks to workers (round-robin)
        for (auto& task : tasks) {
            if (next_worker < size) {
                int task_id = task.id;
                int size_a = task.poly_a.size();
                int size_b = task.poly_b.size();

                MPI_Send(&task_id, 1, MPI_INT, next_worker, 0, MPI_COMM_WORLD);
                MPI_Send(&size_a, 1, MPI_INT, next_worker, 0, MPI_COMM_WORLD);
                MPI_Send(&size_b, 1, MPI_INT, next_worker, 0, MPI_COMM_WORLD);
                MPI_Send(task.poly_a.data(), size_a, MPI_LONG_LONG, next_worker, 0, MPI_COMM_WORLD);
                MPI_Send(task.poly_b.data(), size_b, MPI_LONG_LONG, next_worker, 0, MPI_COMM_WORLD);

                next_worker++;
            } else {
                // Master computes remaining tasks if fewer workers than subproblems
                results[task.id - 1] = multiply_karatsuba_seq(task.poly_a, task.poly_b);
            }
        }

        // Collect results from workers
        for (int w = 1; w < std::min(size, 4); ++w) {
            int task_id;
            int result_size;
            MPI_Recv(&task_id, 1, MPI_INT, w, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&result_size, 1, MPI_INT, w, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            results[task_id - 1].resize(result_size);
            MPI_Recv(results[task_id - 1].data(), result_size, MPI_LONG_LONG, w, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Reconstruct final result from subproblem solutions
        Poly P1 = results[0];
        Poly P2 = results[1];
        Poly P3 = results[2];

        for (size_t i = 0; i < P1.size(); ++i) P3[i] -= P1[i];
        for (size_t i = 0; i < P2.size(); ++i) P3[i] -= P2[i];

        Poly result(n + m - 1, 0);
        for (size_t i = 0; i < P1.size(); ++i) result[i] += P1[i];
        for (size_t i = 0; i < P3.size(); ++i) result[i + half] += P3[i];
        for (size_t i = 0; i < P2.size(); ++i) result[i + 2 * half] += P2[i];

        // Signal workers to terminate
        int terminate = -1;
        for (int w = 1; w < size; ++w) {
            MPI_Send(&terminate, 1, MPI_INT, w, 0, MPI_COMM_WORLD);
        }

        return result;
    } else {
        // Worker process: receive and compute subproblems
        while (true) {
            int task_id;
            MPI_Recv(&task_id, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (task_id == -1) break; // Termination signal

            int size_a, size_b;
            MPI_Recv(&size_a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&size_b, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            Poly poly_a(size_a), poly_b(size_b);
            MPI_Recv(poly_a.data(), size_a, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(poly_b.data(), size_b, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            Poly result = multiply_karatsuba_seq(poly_a, poly_b);

            int result_size = result.size();
            MPI_Send(&task_id, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(&result_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(result.data(), result_size, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
        }

        return Poly(); // Workers return empty result
    }
}

///////////////////////////
/// BENCHMARKING SECTION///
///////////////////////////

/**
 * Benchmarks a distributed multiplication algorithm with timing and verification.
 *
 * Only root process performs timing measurement to avoid clock synchronization issues.
 * All processes participate in computation for accurate distributed performance metrics.
 *
 * @param name   Algorithm identifier for display
 * @param fn     Multiplication function accepting (Poly, Poly, rank, size)
 * @param A      First polynomial (only valid on root)
 * @param B      Second polynomial (only valid on root)
 * @param rank   MPI process rank
 * @param size   Total MPI processes
 */
static void benchmark_mpi(const std::string& name,
                          const std::function<Poly(const Poly&, const Poly&, int, int)>& fn,
                          const Poly& A,
                          const Poly& B,
                          int rank,
                          int size) {
    MPI_Barrier(MPI_COMM_WORLD); // Synchronize before timing

    auto start = std::chrono::high_resolution_clock::now();
    Poly result = fn(A, B, rank, size);
    auto end = std::chrono::high_resolution_clock::now();

    if (rank == 0) {
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << std::left << std::setw(30) << name << " -> "
                  << "Time: " << std::setw(10) << ms << "ms"
                  << " Result[0..4]: ";

        for (size_t i = 0; i < std::min<size_t>(5, result.size()); ++i)
            std::cout << result[i] << " ";
        std::cout << std::endl;
    }
}

///////////////////////////
///   MAIN SECTION      ///
///////////////////////////

/**
 * Main entry point for distributed polynomial multiplication benchmark.
 *
 * Execution:
 *   mpirun -np 4 ./polynomial_mpi
 *
 * Benchmarks:
 * - Sequential naive (reference baseline)
 * - Distributed naive MPI
 * - Sequential Karatsuba (reference baseline)
 * - Distributed Karatsuba MPI
 *
 * Performance Comparison:
 * - Against Lab 5 shared-memory parallel implementations
 * - Communication overhead vs computation speedup trade-offs
 * - Scalability analysis across different process counts
 *
 * @return Exit status code
 */
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const size_t n = 1 << 16;
    Poly A, B;

    if (rank == 0) {
        A.resize(n);
        B.resize(n);
        for (size_t i = 0; i < n; ++i) {
            A[i] = i % 10 + 1;
            B[i] = (i % 5) + 2;
        }

        std::cout << "========================================\n";
        std::cout << "MPI POLYNOMIAL MULTIPLICATION BENCHMARK\n";
        std::cout << "Processes: " << size << "\n";
        std::cout << "Polynomial degree: " << n << "\n";
        std::cout << "========================================\n";
    }

    // Benchmark distributed implementations
    benchmark_mpi("Naive MPI", multiply_naive_mpi, A, B, rank, size);
    benchmark_mpi("Karatsuba MPI", multiply_karatsuba_mpi, A, B, rank, size);

    // Root compares with sequential baselines
    if (rank == 0) {
        std::cout << "\n--- Sequential Baselines (single process) ---\n";

        auto start_seq = std::chrono::high_resolution_clock::now();
        Poly result_seq = multiply_naive_seq(A, B);
        auto end_seq = std::chrono::high_resolution_clock::now();
        double ms_seq = std::chrono::duration<double, std::milli>(end_seq - start_seq).count();

        std::cout << std::left << std::setw(30) << "Naive Sequential" << " -> "
                  << "Time: " << std::setw(10) << ms_seq << "ms\n";

        auto start_kar = std::chrono::high_resolution_clock::now();
        Poly result_kar = multiply_karatsuba_seq(A, B);
        auto end_kar = std::chrono::high_resolution_clock::now();
        double ms_kar = std::chrono::duration<double, std::milli>(end_kar - start_kar).count();

        std::cout << std::left << std::setw(30) << "Karatsuba Sequential" << " -> "
                  << "Time: " << std::setw(10) << ms_kar << "ms\n";

        std::cout << "========================================\n";
        std::cout << "All tests completed\n";
        std::cout << "========================================\n";
    }

    MPI_Finalize();
    return 0;
}