#include "benchmark.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "bigint_polynomial.h"

BenchmarkResult Benchmark::measure(
    const std::string& name,
    std::function<Polynomial(const Polynomial&, const Polynomial&)> func,
    const Polynomial& a,
    const Polynomial& b,
    const Polynomial& expected) {
    
    BenchmarkResult result;
    result.algorithm_name = name;

    func(a, b);
    
    // Measure time
    auto start = std::chrono::high_resolution_clock::now();
    result.result = func(a, b);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    result.time_seconds = duration.count() / 1000000.0;
    
    // Verify correctness if expected result provided
    if (expected.getDegree() >= 0) {
        result.correct = (result.result.getCoefficients() == expected.getCoefficients());
    } else {
        result.correct = true; // Assume correct if no expected result
    }
    
    return result;
}

void Benchmark::compareAll(
    const Polynomial& a,
    const Polynomial& b,
    const std::string& testName) {
    
    std::cout << "\n" << std::string(80, '=') << "\n";
    if (!testName.empty()) {
        std::cout << "Test: " << testName << "\n";
    }
//    std::cout << "Polynomial A (degree " << a.getDegree() << "): " << a.toString() << "\n";
//    std::cout << "Polynomial B (degree " << b.getDegree() << "): " << b.toString() << "\n";
    std::cout << std::string(80, '=') << "\n\n";
    
    // Compute expected result using sequential regular (as reference)
    Polynomial expected = multiplyRegularSequential(a, b);
    
    std::vector<BenchmarkResult> results;
    
    // Measure all variants
    results.push_back(Benchmark::measure(
        "Regular Sequential O(n^2)",
        multiplyRegularSequential,
        a, b, expected
    ));
    
    results.push_back(Benchmark::measure(
        "Regular Parallel O(n^2)",
        multiplyRegularParallel,
        a, b, expected
    ));
    
    results.push_back(Benchmark::measure(
        "Karatsuba Sequential",
        multiplyKaratsubaSequential,
        a, b, expected
    ));
    
    results.push_back(Benchmark::measure(
        "Karatsuba Parallel",
        multiplyKaratsubaParallel,
        a, b, expected
    ));
    
    printResults(results);
}

void Benchmark::printResults(const std::vector<BenchmarkResult>& results) {
    std::cout << std::left << std::setw(30) << "Algorithm"
              << std::right << std::setw(15) << "Time (s)"
              << std::setw(15) << "Time (ms)"
              << std::setw(10) << "Correct"
              << "\n";
    std::cout << std::string(70, '-') << "\n";
    
    // Find the fastest time for speedup calculation
    double fastest = std::min_element(results.begin(), results.end(),
        [](const BenchmarkResult& a, const BenchmarkResult& b) {
            return a.time_seconds < b.time_seconds;
        })->time_seconds;
    
    for (const auto& result : results) {
        double speedup = fastest / result.time_seconds;
        std::cout << std::left << std::setw(30) << result.algorithm_name
                  << std::right << std::fixed << std::setprecision(6)
                  << std::setw(15) << result.time_seconds
                  << std::setw(15) << (result.time_seconds * 1000.0)
                  << std::setw(10) << (result.correct ? "✓" : "✗");
        
        if (result.time_seconds > 0) {
            std::cout << " (speedup: " << std::setprecision(2) << speedup << "x)";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// BigInt versions
BigIntBenchmarkResult Benchmark::measureBigInt(
    const std::string& name,
    std::function<BigIntPolynomial(const BigIntPolynomial&, const BigIntPolynomial&)> func,
    const BigIntPolynomial& a,
    const BigIntPolynomial& b,
    const BigIntPolynomial& expected) {
    
    BigIntBenchmarkResult result;
    result.algorithm_name = name;
    
    // Warm-up run
    func(a, b);
    
    // Measure time
    auto start = std::chrono::high_resolution_clock::now();
    result.result = func(a, b);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    result.time_seconds = duration.count() / 1000000.0;
    
    // Verify correctness if expected result provided
    if (expected.getDegree() >= 0) {
        // Compare coefficients
        result.correct = true;
        int maxDegree = std::max(result.result.getDegree(), expected.getDegree());
        for (int i = 0; i <= maxDegree; ++i) {
            if (result.result.getCoefficient(i) != expected.getCoefficient(i)) {
                result.correct = false;
                break;
            }
        }
    } else {
        result.correct = true; // Assume correct if no expected result
    }
    
    return result;
}

void Benchmark::compareAllBigInt(
    const BigIntPolynomial& a,
    const BigIntPolynomial& b,
    const std::string& testName) {
    
    std::cout << "\n" << std::string(80, '=') << "\n";
    if (!testName.empty()) {
        std::cout << "BIG NUMBER Test: " << testName << "\n";
    }
//    std::cout << "Polynomial A (degree " << a.getDegree() << "): " << a.toString() << "\n";
//    std::cout << "Polynomial B (degree " << b.getDegree() << "): " << b.toString() << "\n";
    std::cout << std::string(80, '=') << "\n\n";
    
    // Compute expected result using sequential regular (as reference)
    BigIntPolynomial expected = multiplyRegularSequentialBigInt(a, b);
    
    std::vector<BigIntBenchmarkResult> results;
    
    // Measure all variants
    results.push_back(Benchmark::measureBigInt(
        "Regular Sequential O(n²) [BigInt]",
        multiplyRegularSequentialBigInt,
        a, b, expected
    ));
    
    results.push_back(Benchmark::measureBigInt(
        "Regular Parallel O(n²) [BigInt]",
        multiplyRegularParallelBigInt,
        a, b, expected
    ));
    
    results.push_back(Benchmark::measureBigInt(
        "Karatsuba Sequential [BigInt]",
        multiplyKaratsubaSequentialBigInt,
        a, b, expected
    ));
    
    results.push_back(Benchmark::measureBigInt(
        "Karatsuba Parallel [BigInt]",
        multiplyKaratsubaParallelBigInt,
        a, b, expected
    ));
    
    printResultsBigInt(results);
}

void Benchmark::printResultsBigInt(const std::vector<BigIntBenchmarkResult>& results) {
    std::cout << std::left << std::setw(40) << "Algorithm"
              << std::right << std::setw(15) << "Time (s)"
              << std::setw(15) << "Time (ms)"
              << std::setw(10) << "Correct"
              << "\n";
    std::cout << std::string(80, '-') << "\n";
    
    // Find the fastest time for speedup calculation
    double fastest = std::min_element(results.begin(), results.end(),
        [](const BigIntBenchmarkResult& a, const BigIntBenchmarkResult& b) {
            return a.time_seconds < b.time_seconds;
        })->time_seconds;
    
    for (const auto& result : results) {
        double speedup = fastest / result.time_seconds;
        std::cout << std::left << std::setw(40) << result.algorithm_name
                  << std::right << std::fixed << std::setprecision(6)
                  << std::setw(15) << result.time_seconds
                  << std::setw(15) << (result.time_seconds * 1000.0)
                  << std::setw(10) << (result.correct ? "✓" : "✗");
        
        if (result.time_seconds > 0) {
            std::cout << " (speedup: " << std::setprecision(2) << speedup << "x)";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

