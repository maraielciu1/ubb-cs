#include "polynomial_mpi.h"
#include <algorithm>
#include <vector>
#include <cmath>

using Coeff = long long;
using Poly = std::vector<Coeff>;

// Sequential naive polynomial multiplication (O(n^2) complexity)
static Poly multiply_naive_seq(const Poly& a, const Poly& b) {
    size_t n = a.size(), m = b.size();
    Poly result(n + m - 1, 0);
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < m; ++j)
            result[i + j] += a[i] * b[j];
    return result;
}

// Sequential Karatsuba multiplication
static Poly multiply_karatsuba_seq(const Poly& a, const Poly& b) {
    size_t n = a.size(), m = b.size();

    if (n <= 128 || m <= 128)
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

// Distributed naive polynomial multiplication using MPI
static Poly multiply_naive_mpi(const Poly& a, const Poly& b, int rank, int size) {
    int n = (rank == 0) ? a.size() : 0;
    int m = (rank == 0) ? b.size() : 0;

    // Root broadcasts polynomial sizes to all processes for allocation
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (n == 0 || m == 0) {
        return Poly();
    }

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
    // Broadcast recvcounts and recvdispls to all processes (MPI_Gatherv needs them on all ranks)
    MPI_Bcast(recvcounts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(recvdispls.data(), size, MPI_INT, 0, MPI_COMM_WORLD);

    Poly result;
    if (rank == 0) result.resize(n + m - 1, 0);

    // Gather with custom reduction (sum overlapping coefficient regions)
    std::vector<Coeff> gathered;
    if (rank == 0) gathered.resize(n + m - 1, 0);

    MPI_Gatherv(
            local_result.data(),
            local_result.size(),
            MPI_LONG_LONG,
            rank == 0 ? gathered.data() : nullptr,
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
                if (offset + j < n + m - 1) {
                    result[offset + j] += gathered[recvdispls[i] + j];
                }
            }
        }
    }

    return result;
}

// Distributed Karatsuba multiplication using master-worker pattern
static Poly multiply_karatsuba_mpi(const Poly& a, const Poly& b, int rank, int size) {
    int n = (rank == 0) ? a.size() : 0;
    int m = (rank == 0) ? b.size() : 0;

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Base case: small polynomials computed locally without distribution overhead
    if (n <= 128 || m <= 128 || size == 1) {
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

// Wrapper functions to convert between Polynomial and Poly
Polynomial multiplyRegularMPI(const Polynomial& a, const Polynomial& b) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Convert Polynomial to Poly - only rank 0 has valid data
    Poly a_poly, b_poly;
    if (rank == 0) {
        // Safely get coefficients only on rank 0
        const std::vector<int>& a_coeffs = a.getCoefficients();
        const std::vector<int>& b_coeffs = b.getCoefficients();
        a_poly.resize(a_coeffs.size());
        b_poly.resize(b_coeffs.size());
        for (size_t i = 0; i < a_coeffs.size(); ++i) a_poly[i] = a_coeffs[i];
        for (size_t i = 0; i < b_coeffs.size(); ++i) b_poly[i] = b_coeffs[i];
    }
    // For rank != 0, a_poly and b_poly remain empty (size 0)
    // This is correct - multiply_naive_mpi will broadcast sizes first
    
    // All processes participate - multiply_naive_mpi handles rank != 0 correctly
    Poly result_poly = multiply_naive_mpi(a_poly, b_poly, rank, size);
    
    if (rank == 0 && !result_poly.empty()) {
        std::vector<int> result_coeffs(result_poly.begin(), result_poly.end());
        return Polynomial(result_coeffs);
    }
    
    return Polynomial({0});
}

Polynomial multiplyKaratsubaMPI(const Polynomial& a, const Polynomial& b) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Only rank 0 has valid polynomials - create empty ones for others
    // The multiply_karatsuba_mpi function will broadcast sizes, so empty vectors are fine
    Poly a_poly, b_poly;
    if (rank == 0) {
        const std::vector<int>& a_coeffs = a.getCoefficients();
        const std::vector<int>& b_coeffs = b.getCoefficients();
        a_poly.resize(a_coeffs.size());
        b_poly.resize(b_coeffs.size());
        for (size_t i = 0; i < a_coeffs.size(); ++i) a_poly[i] = a_coeffs[i];
        for (size_t i = 0; i < b_coeffs.size(); ++i) b_poly[i] = b_coeffs[i];
    }
    // For rank != 0, a_poly and b_poly remain empty, which is fine
    
    // All processes participate - multiply_karatsuba_mpi handles rank != 0 correctly
    Poly result_poly = multiply_karatsuba_mpi(a_poly, b_poly, rank, size);
    
    if (rank == 0 && !result_poly.empty()) {
        std::vector<int> result_coeffs(result_poly.begin(), result_poly.end());
        return Polynomial(result_coeffs);
    }
    
    return Polynomial({0});
}
