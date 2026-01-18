# PDP Lab 7: Polynomial and Big Number Multiplication using MPI

## Overview

This project implements distributed polynomial and big number multiplication using MPI (Message Passing Interface). It compares the performance of MPI-based implementations against CPU-based implementations from Lab 5, using both the regular O(n²) algorithm and the Karatsuba algorithm.

## Algorithms

### 1. Regular O(n²) Algorithm

#### Polynomial Multiplication
The regular algorithm multiplies two polynomials by computing:
```
result[i+j] = Σ(a[i] * b[j]) for all valid pairs (i,j)
```

**CPU Implementation:**
- Sequential: Single-threaded computation
- Parallel: Multi-threaded using `std::thread`, distributing result coefficients across threads

**MPI Implementation:**
- **Distribution Strategy:**
  - Master process (rank 0) broadcasts both polynomials A and B to all processes
  - Each process computes a subset of result coefficients
  - Process `i` computes coefficients: `i, i+numProcs, i+2*numProcs, ...`
  - Results are gathered using `MPI_Reduce` with `MPI_SUM` operation
  - Master combines all partial results

- **Communication Pattern:**
  ```
  Master: Broadcast A, B → All processes
  Each process: Compute local_result[coeffIndex] for assigned coefficients
  All processes: MPI_Reduce(local_result, global_result, MPI_SUM)
  Master: Return combined result
  ```

#### Big Number Multiplication
Similar approach but for BigInt numbers:
- Each digit position in the result is computed by summing products `a[i] * b[j]` where `i + j = resultPos`
- Processes compute different result digit positions
- Final result requires carry propagation

### 2. Karatsuba Algorithm (O(n^1.585))

#### Polynomial Multiplication
The Karatsuba algorithm reduces the number of multiplications from 4 to 3 by using:
```
A = A_high × x^m + A_low
B = B_high × x^m + B_low

z0 = A_low × B_low
z2 = A_high × B_high
z1 = (A_low + A_high) × (B_low + B_high) - z0 - z2

result = z2 × x^(2m) + z1 × x^m + z0
```

**CPU Implementation:**
- Sequential: Recursive implementation with base case threshold
- Parallel: Uses `std::async` to parallelize recursive calls

**MPI Implementation:**
- **Distribution Strategy:**
  - Master-worker pattern
  - For large polynomials, split into 3 recursive calls (z0, z1, z2)
  - Master distributes these calls to available worker processes
  - Workers compute assigned subproblems using sequential Karatsuba
  - Master receives results and combines them
  - For small polynomials or deep recursion, falls back to sequential computation

- **Communication Pattern:**
  ```
  Master: Split problem into z0, z1, z2
  Master → Worker i: Send subproblem data
  Worker i: Compute result, send back to master
  Master: Receive all results, combine
  ```

- **Work Distribution:**
  - Up to 3 workers can be assigned simultaneously (one per subproblem)
  - If more workers available, remaining subproblems computed locally by master
  - Workers wait for termination signal after all work is done

## Distribution and Communication

### Regular O(n²) MPI Algorithm

**Data Distribution:**
1. Master broadcasts polynomial sizes (2 integers)
2. Master broadcasts polynomial coefficients (n + m integers)
3. All processes receive the same data

**Computation Distribution:**
- Each process `rank` computes coefficients at positions: `rank, rank+numProcs, rank+2*numProcs, ...`
- This ensures load balancing when `numProcs << resultSize`

**Result Collection:**
- `MPI_Reduce` with `MPI_SUM` operation combines all partial results
- Each process contributes to different result positions, so summation is correct

**Communication Complexity:**
- Broadcast: O(n + m) data sent to all processes
- Reduce: O(n + m) data collected from all processes
- Total: O(P × (n + m)) where P is number of processes

### Karatsuba MPI Algorithm

**Data Distribution:**
1. Master splits polynomials into subproblems
2. Master sends subproblem data to workers (polynomial coefficients)
3. Workers receive data, compute, and send results back

**Computation Distribution:**
- Master coordinates work assignment
- Workers compute assigned subproblems independently
- Master combines results from workers and local computation

**Result Collection:**
- Each worker sends result size and coefficients back to master
- Master receives results and combines them according to Karatsuba formula

**Communication Complexity:**
- Per recursive level: O(subproblem_size) data sent/received
- Total depends on recursion depth and number of workers used
- Typically: O(P × n × log(n)) for P processes

### Big Number Multiplication

**Distribution Strategy:**
- Similar to polynomial regular O(n²) algorithm
- Each process computes different result digit positions
- Final result requires carry propagation (done on master)

**Communication:**
- Broadcast: BigInt digits and sign information
- Reduce: Partial results summed
- Master: Performs final carry propagation

## Performance Measurements

### Benchmarking Approach

The program measures:
1. **CPU Implementations:**
   - Regular Sequential O(n²)
   - Regular Parallel O(n²) 
   - Karatsuba Sequential
   - Karatsuba Parallel

2. **MPI Implementations:**
   - Regular O(n²) [MPI]
   - Karatsuba [MPI]

3. **Metrics:**
   - Execution time (seconds and milliseconds)
   - Correctness verification (comparing against CPU sequential regular)
   - Speedup relative to fastest CPU implementation

### Expected Performance Characteristics

**Small Problems:**
- MPI overhead may dominate
- CPU implementations likely faster due to communication costs

**Medium Problems:**
- MPI starts showing benefits with multiple processes
- Karatsuba may outperform regular for large enough inputs

**Large Problems:**
- MPI should show significant speedup with multiple processes
- Better scalability than CPU parallel (can use multiple nodes)
- Communication overhead becomes less significant relative to computation

**Factors Affecting Performance:**
1. **Problem Size:** Larger problems benefit more from MPI
2. **Number of Processes:** More processes = more parallelism, but also more communication
3. **Network Bandwidth:** Affects communication time
4. **Load Balancing:** Regular algorithm has better load balance than Karatsuba

## Building and Running

### Prerequisites
- MPI implementation (OpenMPI or MPICH)
- CMake 3.15 or higher
- C++17 compatible compiler

### Build Instructions
```bash
cd pdp_lab7
mkdir build
cd build
cmake ..
make
```

### Run Instructions

**Single node (multiple processes):**
```bash
mpirun -np 4 ./pdp_lab7
```

**Multiple nodes:**
```bash
mpirun -np 8 --hostfile hosts.txt ./pdp_lab7
```

Where `hosts.txt` contains:
```
node1 slots=4
node2 slots=4
```

### Example Output

```
Polynomial Multiplication - MPI vs CPU Performance Comparison
Running with 4 MPI processes
================================================================================

Test: Small Polynomials (degree 10)
Polynomial A: degree 10
Polynomial B: degree 10
================================================================================

Algorithm                          Time (s)      Time (ms)    Correct
---------------------------------------------------------------------------
Regular Sequential O(n²) [CPU]     0.000012      0.012000     ✓
Regular Parallel O(n²) [CPU]      0.000015      0.015000     ✓
Karatsuba Sequential [CPU]        0.000018      0.018000     ✓
Karatsuba Parallel [CPU]          0.000020      0.020000     ✓
Regular O(n²) [MPI]               0.000250      0.250000     ✓
Karatsuba [MPI]                   0.000300      0.300000     ✓

Speedup vs fastest CPU:
  MPI Regular: 0.05x
  MPI Karatsuba: 0.04x
```

## Implementation Details

### File Structure
- `polynomial.h/cpp`: Polynomial class and CPU implementations
- `polynomial_mpi.h/cpp`: MPI implementations for polynomial multiplication
- `bigint.h/cpp`: BigInt class and basic operations
- `bigint_mpi.h/cpp`: MPI implementations for big number multiplication
- `main.cpp`: Benchmarking and comparison program
- `CMakeLists.txt`: Build configuration

### Key Design Decisions

1. **Regular MPI Algorithm:**
   - Uses coefficient-based distribution for better load balancing
   - `MPI_Reduce` simplifies result collection

2. **Karatsuba MPI Algorithm:**
   - Master-worker pattern for recursive calls
   - Limited to 3 workers per recursion level (one per subproblem)
   - Falls back to sequential for small problems

3. **Big Number MPI:**
   - Similar distribution to polynomial regular algorithm
   - Carry propagation done on master to avoid communication overhead

## Limitations and Future Improvements

1. **Karatsuba MPI:**
   - Currently limited to 3 workers per recursion level
   - Could be improved with dynamic work-stealing
   - Deeper recursion could use more processes

2. **Big Number Karatsuba:**
   - Currently uses regular MPI multiplication
   - Full Karatsuba implementation would require careful digit shifting

3. **Load Balancing:**
   - Karatsuba may have uneven load distribution
   - Could benefit from dynamic scheduling

4. **Communication Optimization:**
   - Could use non-blocking communication for better overlap
   - Collective operations could be optimized for specific topologies

## Conclusion

This implementation demonstrates:
- How to distribute polynomial multiplication across multiple MPI processes
- Comparison between regular O(n²) and Karatsuba algorithms in distributed setting
- Performance trade-offs between CPU and MPI implementations
- The importance of problem size and number of processes on performance

The MPI implementations show their strength for large problems where communication overhead is amortized over significant computation, while CPU implementations remain competitive for smaller problems due to lower overhead.

