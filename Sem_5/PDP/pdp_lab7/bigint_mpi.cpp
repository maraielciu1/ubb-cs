#include "bigint_mpi.h"
#include <algorithm>
#include <vector>

BigInt multiplyBigIntRegularMPI(const BigInt& a, const BigInt& b) {
    int rank, numProcs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    
    // Get sizes - only rank 0 has valid BigInt objects
    size_t n = (rank == 0) ? a.getSize() : 0;
    size_t m = (rank == 0) ? b.getSize() : 0;
    
    // Broadcast sizes first - all processes must participate
    int sizes[2] = {static_cast<int>(n), static_cast<int>(m)};
    MPI_Bcast(sizes, 2, MPI_INT, 0, MPI_COMM_WORLD);
    n = sizes[0];
    m = sizes[1];
    
    // Check for zero sizes after broadcasting - all processes check
    if (n == 0 || m == 0) {
        return BigInt(0);
    }
    
    size_t resultSize = n + m;
    
    // Broadcast digits
    std::vector<int> a_digits(n);
    std::vector<int> b_digits(m);
    
    if (rank == 0) {
        for (size_t i = 0; i < n; ++i) {
            a_digits[i] = a.getDigit(i);
        }
        for (size_t i = 0; i < m; ++i) {
            b_digits[i] = b.getDigit(i);
        }
    }
    
    MPI_Bcast(a_digits.data(), n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b_digits.data(), m, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Broadcast sign information
    bool a_negative = rank == 0 ? a.isNegative() : false;
    bool b_negative = rank == 0 ? b.isNegative() : false;
    int signs[2] = {a_negative ? 1 : 0, b_negative ? 1 : 0};
    MPI_Bcast(signs, 2, MPI_INT, 0, MPI_COMM_WORLD);
    a_negative = (signs[0] == 1);
    b_negative = (signs[1] == 1);
    
    // Each process computes its assigned result digits
    // For BigInt multiplication: result[i+j] += a[i] * b[j]
    std::vector<long long> local_result(resultSize, 0);
    
    // Distribute work: each process handles different result positions
    for (size_t resultPos = rank; resultPos < resultSize; resultPos += numProcs) {
        // Find all pairs (i, j) such that i + j = resultPos
        for (size_t i = 0; i <= resultPos && i < n; ++i) {
            size_t j = resultPos - i;
            if (j < m) {
                local_result[resultPos] += static_cast<long long>(a_digits[i]) * 
                                          static_cast<long long>(b_digits[j]);
            }
        }
    }
    
    // Gather results to master
    std::vector<long long> global_result;
    if (rank == 0) {
        global_result.resize(resultSize, 0);
    }
    
    MPI_Reduce(local_result.data(), 
               rank == 0 ? global_result.data() : nullptr,
               resultSize, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        // Convert to BigInt with proper carry propagation
        std::vector<int> result_digits;
        long long carry = 0;
        
        for (size_t i = 0; i < resultSize; ++i) {
            long long sum = global_result[i] + carry;
            result_digits.push_back(static_cast<int>(sum % 10));
            carry = sum / 10;
        }
        
        while (carry > 0) {
            result_digits.push_back(static_cast<int>(carry % 10));
            carry /= 10;
        }
        
        bool result_negative = (a_negative != b_negative);
        BigInt result(result_digits, result_negative);
        return result;
    }
    
    return BigInt(0); // Non-master processes return dummy
}

// Sequential Karatsuba multiplication for BigInt (helper function)
// Used as base case in MPI Karatsuba
static BigInt multiply_karatsuba_seq_bigint(const std::vector<int>& a_digits, 
                                            const std::vector<int>& b_digits,
                                            bool a_negative, bool b_negative) {
    size_t n = a_digits.size();
    size_t m = b_digits.size();
    
    // Base case: use regular multiplication for small numbers
    if (n <= 64 || m <= 64) {
        // Regular O(n²) multiplication
        size_t resultSize = n + m;
        std::vector<long long> temp_result(resultSize, 0);
        
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                temp_result[i + j] += static_cast<long long>(a_digits[i]) * 
                                     static_cast<long long>(b_digits[j]);
            }
        }
        
        // Convert to BigInt with carry propagation
        std::vector<int> result_digits;
        long long carry = 0;
        for (size_t i = 0; i < resultSize; ++i) {
            long long sum = temp_result[i] + carry;
            result_digits.push_back(static_cast<int>(sum % 10));
            carry = sum / 10;
        }
        while (carry > 0) {
            result_digits.push_back(static_cast<int>(carry % 10));
            carry /= 10;
        }
        
        bool result_negative = (a_negative != b_negative);
        return BigInt(result_digits, result_negative);
    }
    
    // Split into high and low parts
    size_t half = std::max(n, m) / 2;
    
    std::vector<int> a_low, a_high, b_low, b_high;
    for (size_t i = 0; i < n && i < half; ++i) {
        a_low.push_back(a_digits[i]);
    }
    for (size_t i = half; i < n; ++i) {
        a_high.push_back(a_digits[i]);
    }
    for (size_t i = 0; i < m && i < half; ++i) {
        b_low.push_back(b_digits[i]);
    }
    for (size_t i = half; i < m; ++i) {
        b_high.push_back(b_digits[i]);
    }
    
    // Ensure non-empty
    if (a_low.empty()) a_low.push_back(0);
    if (a_high.empty()) a_high.push_back(0);
    if (b_low.empty()) b_low.push_back(0);
    if (b_high.empty()) b_high.push_back(0);
    
    // Compute z0 = a_low * b_low
    BigInt z0 = multiply_karatsuba_seq_bigint(a_low, b_low, false, false);
    
    // Compute z2 = a_high * b_high
    BigInt z2 = multiply_karatsuba_seq_bigint(a_high, b_high, false, false);
    
    // Compute a_sum = a_low + a_high and b_sum = b_low + b_high
    size_t max_a = std::max(a_low.size(), a_high.size());
    size_t max_b = std::max(b_low.size(), b_high.size());
    std::vector<int> a_sum(max_a, 0);
    std::vector<int> b_sum(max_b, 0);
    
    for (size_t i = 0; i < a_low.size(); ++i) a_sum[i] += a_low[i];
    for (size_t i = 0; i < a_high.size(); ++i) a_sum[i] += a_high[i];
    for (size_t i = 0; i < b_low.size(); ++i) b_sum[i] += b_low[i];
    for (size_t i = 0; i < b_high.size(); ++i) b_sum[i] += b_high[i];
    
    // Normalize sums (handle carries)
    for (size_t i = 0; i < a_sum.size(); ++i) {
        if (a_sum[i] >= 10) {
            if (i + 1 >= a_sum.size()) a_sum.push_back(0);
            a_sum[i + 1] += a_sum[i] / 10;
            a_sum[i] %= 10;
        }
    }
    for (size_t i = 0; i < b_sum.size(); ++i) {
        if (b_sum[i] >= 10) {
            if (i + 1 >= b_sum.size()) b_sum.push_back(0);
            b_sum[i + 1] += b_sum[i] / 10;
            b_sum[i] %= 10;
        }
    }
    
    // Compute z1 = (a_low + a_high) * (b_low + b_high)
    BigInt z1 = multiply_karatsuba_seq_bigint(a_sum, b_sum, false, false);
    
    // Compute z1 = z1 - z0 - z2
    z1 = z1 - z0 - z2;
    
    // Reconstruct: result = z2 * 10^(2*half) + z1 * 10^half + z0
    // In little-endian: shifting by m means prepending m zeros
    
    // Get z0, z1, z2 as digit vectors
    std::vector<int> z0_digits, z1_digits, z2_digits;
    for (int i = 0; i < static_cast<int>(z0.getSize()); ++i) {
        z0_digits.push_back(z0.getDigit(i));
    }
    for (int i = 0; i < static_cast<int>(z1.getSize()); ++i) {
        z1_digits.push_back(z1.getDigit(i));
    }
    for (int i = 0; i < static_cast<int>(z2.getSize()); ++i) {
        z2_digits.push_back(z2.getDigit(i));
    }
    
    // Result size: max(z0.size, z1.size + half, z2.size + 2*half)
    size_t result_size = std::max({z0_digits.size(), 
                                   z1_digits.size() + half, 
                                   z2_digits.size() + 2 * half});
    std::vector<long long> result_temp(result_size, 0);
    
    // Add z0 (no shift)
    for (size_t i = 0; i < z0_digits.size(); ++i) {
        result_temp[i] += z0_digits[i];
    }
    
    // Add z1 shifted by half (prepend half zeros = shift indices by half)
    for (size_t i = 0; i < z1_digits.size(); ++i) {
        result_temp[i + half] += z1_digits[i];
    }
    
    // Add z2 shifted by 2*half
    for (size_t i = 0; i < z2_digits.size(); ++i) {
        result_temp[i + 2 * half] += z2_digits[i];
    }
    
    // Convert to BigInt with carry propagation
    std::vector<int> result_digits;
    long long carry = 0;
    for (size_t i = 0; i < result_size; ++i) {
        long long sum = result_temp[i] + carry;
        result_digits.push_back(static_cast<int>(sum % 10));
        carry = sum / 10;
    }
    while (carry > 0) {
        result_digits.push_back(static_cast<int>(carry % 10));
        carry /= 10;
    }
    
    bool result_negative = (a_negative != b_negative);
    return BigInt(result_digits, result_negative);
}

// Distributed Karatsuba multiplication using master-worker pattern
static BigInt multiply_karatsuba_mpi_bigint(const std::vector<int>& a_digits,
                                            const std::vector<int>& b_digits,
                                            bool a_negative, bool b_negative,
                                            int rank, int size) {
    size_t n = a_digits.size();
    size_t m = b_digits.size();
    
    // Base case: small numbers computed locally without distribution overhead
    if (n <= 256 || m <= 256 || size == 1) {
        return multiply_karatsuba_seq_bigint(a_digits, b_digits, a_negative, b_negative);
    }
    
    if (rank == 0) {
        // Master process: decompose and distribute subproblems
        size_t half = std::max(n, m) / 2;
        
        // Split into high and low parts
        std::vector<int> a_low, a_high, b_low, b_high;
        for (size_t i = 0; i < n && i < half; ++i) {
            a_low.push_back(a_digits[i]);
        }
        for (size_t i = half; i < n; ++i) {
            a_high.push_back(a_digits[i]);
        }
        for (size_t i = 0; i < m && i < half; ++i) {
            b_low.push_back(b_digits[i]);
        }
        for (size_t i = half; i < m; ++i) {
            b_high.push_back(b_digits[i]);
        }
        
        // Ensure non-empty
        if (a_low.empty()) a_low.push_back(0);
        if (a_high.empty()) a_high.push_back(0);
        if (b_low.empty()) b_low.push_back(0);
        if (b_high.empty()) b_high.push_back(0);
        
        // Compute sums
        size_t max_a = std::max(a_low.size(), a_high.size());
        size_t max_b = std::max(b_low.size(), b_high.size());
        std::vector<int> a_sum(max_a, 0);
        std::vector<int> b_sum(max_b, 0);
        
        for (size_t i = 0; i < a_low.size(); ++i) a_sum[i] += a_low[i];
        for (size_t i = 0; i < a_high.size(); ++i) a_sum[i] += a_high[i];
        for (size_t i = 0; i < b_low.size(); ++i) b_sum[i] += b_low[i];
        for (size_t i = 0; i < b_high.size(); ++i) b_sum[i] += b_high[i];
        
        // Normalize sums
        for (size_t i = 0; i < a_sum.size(); ++i) {
            if (a_sum[i] >= 10) {
                if (i + 1 >= a_sum.size()) a_sum.push_back(0);
                a_sum[i + 1] += a_sum[i] / 10;
                a_sum[i] %= 10;
            }
        }
        for (size_t i = 0; i < b_sum.size(); ++i) {
            if (b_sum[i] >= 10) {
                if (i + 1 >= b_sum.size()) b_sum.push_back(0);
                b_sum[i + 1] += b_sum[i] / 10;
                b_sum[i] %= 10;
            }
        }
        
        // Package subproblems for distribution
        struct Subproblem {
            int id;
            std::vector<int> digits_a, digits_b;
        };
        
        std::vector<Subproblem> tasks = {
            {1, a_low, b_low},
            {2, a_high, b_high},
            {3, a_sum, b_sum}
        };
        
        std::vector<BigInt> results(3);
        int next_worker = 1;
        
        // Distribute tasks to workers (round-robin)
        for (auto& task : tasks) {
            if (next_worker < size) {
                int task_id = task.id;
                int size_a = task.digits_a.size();
                int size_b = task.digits_b.size();
                
                MPI_Send(&task_id, 1, MPI_INT, next_worker, 0, MPI_COMM_WORLD);
                MPI_Send(&size_a, 1, MPI_INT, next_worker, 0, MPI_COMM_WORLD);
                MPI_Send(&size_b, 1, MPI_INT, next_worker, 0, MPI_COMM_WORLD);
                MPI_Send(task.digits_a.data(), size_a, MPI_INT, next_worker, 0, MPI_COMM_WORLD);
                MPI_Send(task.digits_b.data(), size_b, MPI_INT, next_worker, 0, MPI_COMM_WORLD);
                
                next_worker++;
            } else {
                // Master computes remaining tasks if fewer workers than subproblems
                results[task.id - 1] = multiply_karatsuba_seq_bigint(
                    task.digits_a, task.digits_b, false, false);
            }
        }
        
        // Collect results from workers
        for (int w = 1; w < std::min(size, 4); ++w) {
            int task_id;
            int result_size;
            MPI_Recv(&task_id, 1, MPI_INT, w, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&result_size, 1, MPI_INT, w, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            std::vector<int> result_digits(result_size);
            MPI_Recv(result_digits.data(), result_size, MPI_INT, w, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            results[task_id - 1] = BigInt(result_digits, false);
        }
        
        // Reconstruct final result from subproblem solutions
        BigInt z0 = results[0];
        BigInt z2 = results[1];
        BigInt z1 = results[2];
        
        // Compute z1 = z1 - z0 - z2
        z1 = z1 - z0 - z2;
        
        // Reconstruct: result = z2 * 10^(2*half) + z1 * 10^half + z0
        std::vector<int> z0_digits, z1_digits, z2_digits;
        for (int i = 0; i < static_cast<int>(z0.getSize()); ++i) {
            z0_digits.push_back(z0.getDigit(i));
        }
        for (int i = 0; i < static_cast<int>(z1.getSize()); ++i) {
            z1_digits.push_back(z1.getDigit(i));
        }
        for (int i = 0; i < static_cast<int>(z2.getSize()); ++i) {
            z2_digits.push_back(z2.getDigit(i));
        }
        
        size_t result_size = std::max({z0_digits.size(), 
                                       z1_digits.size() + half, 
                                       z2_digits.size() + 2 * half});
        std::vector<long long> result_temp(result_size, 0);
        
        // Add z0 (no shift)
        for (size_t i = 0; i < z0_digits.size(); ++i) {
            result_temp[i] += z0_digits[i];
        }
        
        // Add z1 shifted by half
        for (size_t i = 0; i < z1_digits.size(); ++i) {
            result_temp[i + half] += z1_digits[i];
        }
        
        // Add z2 shifted by 2*half
        for (size_t i = 0; i < z2_digits.size(); ++i) {
            result_temp[i + 2 * half] += z2_digits[i];
        }
        
        // Convert to BigInt with carry propagation
        std::vector<int> result_digits;
        long long carry = 0;
        for (size_t i = 0; i < result_size; ++i) {
            long long sum = result_temp[i] + carry;
            result_digits.push_back(static_cast<int>(sum % 10));
            carry = sum / 10;
        }
        while (carry > 0) {
            result_digits.push_back(static_cast<int>(carry % 10));
            carry /= 10;
        }
        
        bool result_negative = (a_negative != b_negative);
        
        // Signal workers to terminate
        int terminate = -1;
        for (int w = 1; w < size; ++w) {
            MPI_Send(&terminate, 1, MPI_INT, w, 0, MPI_COMM_WORLD);
        }
        
        return BigInt(result_digits, result_negative);
    } else {
        // Worker process: receive and compute subproblems
        while (true) {
            int task_id;
            MPI_Recv(&task_id, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            if (task_id == -1) break; // Termination signal
            
            int size_a, size_b;
            MPI_Recv(&size_a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&size_b, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            std::vector<int> digits_a(size_a), digits_b(size_b);
            MPI_Recv(digits_a.data(), size_a, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(digits_b.data(), size_b, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            BigInt result = multiply_karatsuba_seq_bigint(digits_a, digits_b, false, false);
            
            int result_size = result.getSize();
            std::vector<int> result_digits(result_size);
            for (int i = 0; i < result_size; ++i) {
                result_digits[i] = result.getDigit(i);
            }
            
            MPI_Send(&task_id, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(&result_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(result_digits.data(), result_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        
        return BigInt(0); // Workers return dummy
    }
}

// KARATSUBA MPI ALGORITHM for BigInt - O(n^1.585)
BigInt multiplyBigIntKaratsubaMPI(const BigInt& a, const BigInt& b) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Get sizes - only rank 0 has valid BigInt objects
    size_t n = (rank == 0) ? a.getSize() : 0;
    size_t m = (rank == 0) ? b.getSize() : 0;
    
    // Broadcast sizes first - all processes must participate
    int sizes[2] = {static_cast<int>(n), static_cast<int>(m)};
    MPI_Bcast(sizes, 2, MPI_INT, 0, MPI_COMM_WORLD);
    n = sizes[0];
    m = sizes[1];
    
    // Check for zero sizes after broadcasting
    if (n == 0 || m == 0) {
        return BigInt(0);
    }
    
    // Broadcast digits
    std::vector<int> a_digits(n);
    std::vector<int> b_digits(m);
    
    if (rank == 0) {
        for (size_t i = 0; i < n; ++i) {
            a_digits[i] = a.getDigit(i);
        }
        for (size_t i = 0; i < m; ++i) {
            b_digits[i] = b.getDigit(i);
        }
    }
    
    MPI_Bcast(a_digits.data(), n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b_digits.data(), m, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Broadcast sign information
    bool a_negative = rank == 0 ? a.isNegative() : false;
    bool b_negative = rank == 0 ? b.isNegative() : false;
    int signs[2] = {a_negative ? 1 : 0, b_negative ? 1 : 0};
    MPI_Bcast(signs, 2, MPI_INT, 0, MPI_COMM_WORLD);
    a_negative = (signs[0] == 1);
    b_negative = (signs[1] == 1);
    
    // All processes participate in Karatsuba MPI
    return multiply_karatsuba_mpi_bigint(a_digits, b_digits, a_negative, b_negative, rank, size);
}

