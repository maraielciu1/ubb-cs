#include "bigint_polynomial.h"
#include <algorithm>
#include <stdexcept>
#include <thread>
#include <future>

// Karatsuba base case threshold for BigInt
static const int KARATSUBA_THRESHOLD_BIGINT = 100;
static const int MAX_PARALLEL_DEPTH_BIGINT = 4;

// Helper function to find next power of 2
static int find_next_power(int n) {
    int next_power = 1;
    while (next_power < n) {
        next_power *= 2;
    }
    return next_power;
}

// Constructor from BigInt coefficients
BigIntPolynomial::BigIntPolynomial(const std::vector<BigInt>& coefficients) 
    : coefficients_(coefficients) {
    trim();
}

// Constructor for polynomial of given degree
BigIntPolynomial::BigIntPolynomial(int degree) {
    if (degree < 0) {
        coefficients_ = {BigInt(0)};
    } else {
        coefficients_.resize(degree + 1, BigInt(0));
    }
}

// Constructor from int coefficients
BigIntPolynomial::BigIntPolynomial(const std::vector<int>& coefficients) {
    coefficients_.reserve(coefficients.size());
    for (int coeff : coefficients) {
        coefficients_.push_back(BigInt(coeff));
    }
    trim();
}

int BigIntPolynomial::getDegree() const {
    if (coefficients_.empty()) return -1;
    return static_cast<int>(coefficients_.size()) - 1;
}

BigInt BigIntPolynomial::getCoefficient(int index) const {
    if (index < 0 || index >= static_cast<int>(coefficients_.size())) {
        return BigInt(0);
    }
    return coefficients_[index];
}

void BigIntPolynomial::setCoefficient(int index, const BigInt& value) {
    if (index < 0) {
        throw std::invalid_argument("Index must be non-negative");
    }
    if (index >= static_cast<int>(coefficients_.size())) {
        coefficients_.resize(index + 1, BigInt(0));
    }
    coefficients_[index] = value;
}

const std::vector<BigInt>& BigIntPolynomial::getCoefficients() const {
    return coefficients_;
}

BigIntPolynomial BigIntPolynomial::operator+(const BigIntPolynomial& other) const {
    int maxSize = std::max(coefficients_.size(), other.coefficients_.size());
    BigIntPolynomial result(maxSize - 1);
    
    for (int i = 0; i < maxSize; ++i) {
        result.setCoefficient(i, getCoefficient(i) + other.getCoefficient(i));
    }
    
    result.trim();
    return result;
}

BigIntPolynomial BigIntPolynomial::operator-(const BigIntPolynomial& other) const {
    int maxSize = std::max(coefficients_.size(), other.coefficients_.size());
    BigIntPolynomial result(maxSize - 1);
    
    for (int i = 0; i < maxSize; ++i) {
        result.setCoefficient(i, getCoefficient(i) - other.getCoefficient(i));
    }
    
    result.trim();
    return result;
}

BigIntPolynomial BigIntPolynomial::shiftLeft(int positions) const {
    if (positions <= 0) {
        return *this;
    }
    
    BigIntPolynomial result;
    result.coefficients_.resize(coefficients_.size() + positions, BigInt(0));
    std::copy(coefficients_.begin(), coefficients_.end(), 
              result.coefficients_.begin() + positions);
    return result;
}

void BigIntPolynomial::resize(int newSize) {
    coefficients_.resize(newSize, BigInt(0));
}

void BigIntPolynomial::trim() {
    while (!coefficients_.empty() && coefficients_.back() == BigInt(0)) {
        coefficients_.pop_back();
    }
    if (coefficients_.empty()) {
        coefficients_.push_back(BigInt(0));
    }
}

std::string BigIntPolynomial::toString() const {
    if (coefficients_.empty() || (coefficients_.size() == 1 && coefficients_[0] == BigInt(0))) {
        return "0";
    }
    
    std::string result;
    bool first = true;
    
    for (int i = static_cast<int>(coefficients_.size()) - 1; i >= 0; --i) {
        BigInt coeff = coefficients_[i];
        if (coeff == BigInt(0)) continue;
        
        std::string coeffStr = coeff.toString();
        bool isNegative = coeff.isNegative();
        
        if (!first && !isNegative) {
            result += " + ";
        } else if (isNegative) {
            result += " - ";
            if (coeffStr[0] == '-') {
                coeffStr = coeffStr.substr(1);
            }
        }
        
        if (i == 0) {
            result += coeffStr;
        } else if (coeffStr == "1" || coeffStr == "-1") {
            if (coeffStr == "-1") {
                result += "-";
            }
            result += "x";
        } else {
            result += coeffStr + "x";
        }
        
        if (i > 1) {
            result += "^" + std::to_string(i);
        }
        
        first = false;
    }
    
    return result;
}

// Regular O(n^2) sequential multiplication with BigInt
BigIntPolynomial multiplyRegularSequentialBigInt(const BigIntPolynomial& a, const BigIntPolynomial& b) {
    int degreeA = a.getDegree();
    int degreeB = b.getDegree();
    
    if (degreeA < 0 || degreeB < 0) {
        return BigIntPolynomial({BigInt(0)});
    }
    
    int resultDegree = degreeA + degreeB;
    BigIntPolynomial result(resultDegree);
    
    for (int i = 0; i <= degreeA; ++i) {
        for (int j = 0; j <= degreeB; ++j) {
            BigInt current = result.getCoefficient(i + j);
            result.setCoefficient(i + j, current + a.getCoefficient(i) * b.getCoefficient(j));
        }
    }
    
    result.trim();
    return result;
}

// Worker function for regular parallel multiplication with BigInt
static void workerRegularParallelBigInt(const std::vector<BigInt>& a, const std::vector<BigInt>& b, 
                                        std::vector<BigInt>& c, int index, int numThreads) {
    int n = a.size();
    int m = b.size();
    
    for (int i = index; i < n + m - 1; i += numThreads) {
        for (int j = 0; j <= i; ++j) {
            if (j < n && i - j < m) {
                c[i] = c[i] + a[j] * b[i - j];
            }
        }
    }
}

// Regular O(n^2) parallel multiplication with BigInt using std::thread
BigIntPolynomial multiplyRegularParallelBigInt(const BigIntPolynomial& a, const BigIntPolynomial& b) {
    int n = a.getDegree() + 1;
    int m = b.getDegree() + 1;
    
    if (n == 0 || m == 0) {
        return BigIntPolynomial({BigInt(0)});
    }
    
    const std::vector<BigInt>& a_coeffs = a.getCoefficients();
    const std::vector<BigInt>& b_coeffs = b.getCoefficients();
    
    const int THREADS = std::thread::hardware_concurrency();
    const int numThreads = (THREADS > 0) ? THREADS : 8;
    
    std::vector<BigInt> c(n + m - 1, BigInt(0));
    std::vector<std::thread> threads;
    
    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(workerRegularParallelBigInt, 
                                     std::cref(a_coeffs), std::cref(b_coeffs), 
                                     std::ref(c), i, numThreads));
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    BigIntPolynomial result(c);
    result.trim();
    return result;
}

// Helper function for Karatsuba (sequential) with BigInt
BigIntPolynomial karatsubaSequentialHelperBigInt(const BigIntPolynomial& a, const BigIntPolynomial& b) {
    int degreeA = a.getDegree();
    int degreeB = b.getDegree();
    
    if (degreeA < 0 || degreeB < 0) {
        return BigIntPolynomial({BigInt(0)});
    }
    
    // Base case: use regular multiplication when size is below threshold
    // BigInt operations are more expensive, so threshold is typically smaller
    int sizeA = degreeA + 1;
    int sizeB = degreeB + 1;
    if (sizeA <= KARATSUBA_THRESHOLD_BIGINT && sizeB <= KARATSUBA_THRESHOLD_BIGINT) {
        return multiplyRegularSequentialBigInt(a, b);
    }
    
    int m = std::max(degreeA, degreeB) / 2;
    
    BigIntPolynomial a0, a1, b0, b1;
    
    for (int i = 0; i <= m && i <= degreeA; ++i) {
        a0.setCoefficient(i, a.getCoefficient(i));
    }
    for (int i = m + 1; i <= degreeA; ++i) {
        a1.setCoefficient(i - m - 1, a.getCoefficient(i));
    }
    
    for (int i = 0; i <= m && i <= degreeB; ++i) {
        b0.setCoefficient(i, b.getCoefficient(i));
    }
    for (int i = m + 1; i <= degreeB; ++i) {
        b1.setCoefficient(i - m - 1, b.getCoefficient(i));
    }
    
    BigIntPolynomial z0 = karatsubaSequentialHelperBigInt(a0, b0);
    BigIntPolynomial z2 = karatsubaSequentialHelperBigInt(a1, b1);
    BigIntPolynomial z1 = karatsubaSequentialHelperBigInt(a0 + a1, b0 + b1) - z0 - z2;
    
    return z2.shiftLeft(2 * m) + z1.shiftLeft(m) + z0;
}

// Karatsuba sequential multiplication with BigInt
BigIntPolynomial multiplyKaratsubaSequentialBigInt(const BigIntPolynomial& a, const BigIntPolynomial& b) {
    return karatsubaSequentialHelperBigInt(a, b);
}

// Worker function for parallel regular multiplication on BigInt vectors
// Used as base case in parallel Karatsuba
static void workerRegularParallelBigIntVectors(const std::vector<BigInt>& a, const std::vector<BigInt>& b, 
                                                std::vector<BigInt>& result, int threadId, int numThreads) {
    int n = a.size();
    int m = b.size();
    
    for (int coeffIndex = threadId; coeffIndex < n + m - 1; coeffIndex += numThreads) {
        for (int i = 0; i <= coeffIndex; ++i) {
            int j = coeffIndex - i;
            if (i < n && j < m) {
                result[coeffIndex] = result[coeffIndex] + a[i] * b[j];
            }
        }
    }
}

// Parallel regular multiplication for BigInt vectors (used in parallel Karatsuba base case)
static void multiplyRegularParallelBigIntVectors(const std::vector<BigInt>& a, const std::vector<BigInt>& b, 
                                                  std::vector<BigInt>& result) {
    int n = a.size();
    int m = b.size();
    
    if (n == 0 || m == 0) {
        return;
    }
    
    const int numThreads = std::max(1, static_cast<int>(std::thread::hardware_concurrency()));
    std::vector<std::thread> threads;
    
    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(workerRegularParallelBigIntVectors, 
                                     std::cref(a), std::cref(b), 
                                     std::ref(result), i, numThreads));
    }
    
    for (auto& t : threads) {
        t.join();
    }
}

// Helper function for Karatsuba (parallel) with BigInt
static void karatsubaParallelHelperBigInt(const std::vector<BigInt>& a, const std::vector<BigInt>& b, 
                                          std::vector<BigInt>& sol, int depth = 0) {
    if (depth > MAX_PARALLEL_DEPTH_BIGINT) {
        BigIntPolynomial poly_a(a);
        BigIntPolynomial poly_b(b);
        BigIntPolynomial result = karatsubaSequentialHelperBigInt(poly_a, poly_b);
        const std::vector<BigInt>& result_coeffs = result.getCoefficients();
        sol.resize(result_coeffs.size(), BigInt(0));
        for (size_t i = 0; i < result_coeffs.size(); ++i) {
            sol[i] = result_coeffs[i];
        }
        return;
    }
    
    // Base case: use parallel regular multiplication for small polynomials
    // This maintains parallelism even at the base case
    if (a.size() <= KARATSUBA_THRESHOLD_BIGINT && b.size() <= KARATSUBA_THRESHOLD_BIGINT) {
        multiplyRegularParallelBigIntVectors(a, b, sol);
        return;
    }
    
    int half = a.size() / 2;
    
    std::vector<BigInt> a_lo(a.begin(), a.begin() + half);
    std::vector<BigInt> a_hi(a.begin() + half, a.end());
    std::vector<BigInt> b_lo(b.begin(), b.begin() + half);
    std::vector<BigInt> b_hi(b.begin() + half, b.end());
    
    std::vector<BigInt> lo(a_lo.size() + b_lo.size() - 1, BigInt(0));
    std::vector<BigInt> hi(a_hi.size() + b_hi.size() - 1, BigInt(0));
    
    // Parallelize lo and hi multiplications using std::async
    auto future_lo = std::async(std::launch::async, karatsubaParallelHelperBigInt, 
                               std::cref(a_lo), std::cref(b_lo), std::ref(lo), depth + 1);
    auto future_hi = std::async(std::launch::async, karatsubaParallelHelperBigInt, 
                               std::cref(a_hi), std::cref(b_hi), std::ref(hi), depth + 1);
    
    // Prepare sum vectors while async tasks run
    std::vector<BigInt> sum_a = a_lo;
    std::vector<BigInt> sum_b = b_lo;
    
    // Resize to max size
    int max_size = std::max(sum_a.size(), a_hi.size());
    sum_a.resize(max_size, BigInt(0));
    max_size = std::max(sum_b.size(), b_hi.size());
    sum_b.resize(max_size, BigInt(0));
    
    for (int i = 0; i < a_hi.size(); ++i) {
        sum_a[i] = sum_a[i] + a_hi[i];
    }
    for (int i = 0; i < b_hi.size(); ++i) {
        sum_b[i] = sum_b[i] + b_hi[i];
    }
    
    // Wait for lo and hi to complete
    future_lo.get();
    future_hi.get();
    
    // Compute mid sequentially
    std::vector<BigInt> mid(sum_a.size() + sum_b.size() - 1, BigInt(0));
    karatsubaParallelHelperBigInt(sum_a, sum_b, mid, depth + 1);
    
    // Combine results: sol = x^(2*half) * hi + x^half * (mid - lo - hi) + lo
    for (int i = 0; i < lo.size(); ++i) {
        sol[i] = sol[i] + lo[i];
    }
    for (int i = 0; i < mid.size(); ++i) {
        sol[i + half] = sol[i + half] + mid[i] - (i < lo.size() ? lo[i] : BigInt(0)) - (i < hi.size() ? hi[i] : BigInt(0));
    }
    for (int i = 0; i < hi.size(); ++i) {
        sol[i + 2 * half] = sol[i + 2 * half] + hi[i];
    }
}

// Karatsuba parallel multiplication with BigInt using std::async
BigIntPolynomial multiplyKaratsubaParallelBigInt(const BigIntPolynomial& a, const BigIntPolynomial& b) {
    const std::vector<BigInt>& a_coeffs = a.getCoefficients();
    const std::vector<BigInt>& b_coeffs = b.getCoefficients();
    
    int n = a_coeffs.size();
    int m = b_coeffs.size();
    
    if (n == 0 || m == 0) {
        return BigIntPolynomial({BigInt(0)});
    }
    
    // Convert and pad to power of 2
    int new_size = find_next_power(std::max(n, m));
    
    std::vector<BigInt> a_padded(new_size, BigInt(0));
    std::vector<BigInt> b_padded(new_size, BigInt(0));
    
    for (int i = 0; i < n; ++i) {
        a_padded[i] = a_coeffs[i];
    }
    for (int i = 0; i < m; ++i) {
        b_padded[i] = b_coeffs[i];
    }
    
    std::vector<BigInt> sol(a_padded.size() + b_padded.size() - 1, BigInt(0));
    karatsubaParallelHelperBigInt(a_padded, b_padded, sol, 0);
    
    // Clean zeroes and convert back
    int result_size = a.getDegree() + b.getDegree() + 1;
    while (sol.size() > result_size && sol.back() == BigInt(0)) {
        sol.pop_back();
    }
    
    return BigIntPolynomial(sol);
}

