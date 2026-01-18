# Lab 6: Parallel Hamiltonian Cycle Finder

## Goal

Implement a parallel algorithm to find a Hamiltonian cycle in a directed graph, if one exists. The solution uses a specified number of threads to parallelize the search, with work splitting at multiple levels of the search tree.

## Problem Description

A **Hamiltonian cycle** in a directed graph is a cycle that visits each vertex exactly once and returns to the starting vertex. This is an NP-complete problem, making it computationally expensive for large graphs.

### Requirements

- Start the search from a fixed vertex (typically vertex 0)
- Split work between threads at multiple levels of the search tree
- At each vertex, distribute available threads among all possible neighbor choices
- Example: With 8 threads and a vertex with out-degree 3:
  - Two branches get 3 threads each
  - One branch gets 2 threads
  - Further down the tree, threads are recursively split among neighbors

## Algorithms

### Backtracking Search Algorithm

Both implementations use a **backtracking depth-first search** approach:

1. **Base Case**: If all vertices are visited (path length = N), check if there's an edge back to the starting vertex
2. **Recursive Case**: 
   - Get all unvisited neighbors of the current vertex
   - For each neighbor, recursively explore that path
   - Mark vertices as used/unused during backtracking

### Parallelization Strategy

#### C++ Implementation (std::async)

- **Thread Allocation**: At each level, available threads are divided among neighbor branches
- **Dynamic Splitting**: 
  - If a vertex has `k` neighbors and `t` threads available:
    - Create `min(k, t)` subtasks
    - Each subtask handles a chunk of neighbors
    - Threads are divided as evenly as possible among subtasks
- **Recursive Division**: Each subtask receives `threads / subtasks` threads for its subtree
- **Early Termination**: All threads check an atomic `found` flag to stop searching once a solution is found

#### Java Implementation (ForkJoinPool)

- **Work Stealing**: Uses Java's ForkJoinPool with work-stealing algorithm
- **Task Creation**: Each neighbor branch becomes a separate `RecursiveTask`
- **Fork-Join Pattern**:
  - Fork all neighbor tasks except one
  - Compute one task directly (to avoid unnecessary overhead)
  - Join all forked tasks
- **Early Termination**: Uses `AtomicBoolean` to signal when solution is found

## Synchronization

### C++ Implementation

1. **Atomic Boolean (`std::atomic<bool> found`)**:
   - Used to signal when a solution is found
   - All threads check this flag before continuing search
   - `exchange(true)` atomically sets the flag and returns previous value

2. **Mutex (`std::mutex solution_mtx`)**:
   - Protects access to the `solution` path vector
   - Only the first thread to find a solution writes to it
   - Uses `std::lock_guard` for RAII-style locking

3. **Future Synchronization**:
   - `std::async` returns `std::future<void>` objects
   - `future.wait()` ensures all subtasks complete before continuing

### Java Implementation

1. **Atomic Boolean (`AtomicBoolean found`)**:
   - Similar to C++ version
   - `compareAndSet(false, true)` atomically sets flag only if currently false

2. **Synchronized Block (`synchronized(lock)`)**:
   - Protects the `solution` list when writing
   - Ensures only one thread writes the solution

3. **ForkJoinPool Synchronization**:
   - `invoke()` waits for root task completion
   - `fork()` and `join()` handle task distribution and synchronization
   - Work-stealing queue provides implicit synchronization

## Performance Measurements

### Test Graphs

The implementation includes several test cases:

1. **Directed 5-cycle**: Small cycle graph (has solution)
2. **Complete graph (6 vertices)**: Fully connected graph (has solution)
3. **Line graph**: Linear path with no cycle (no solution)
4. **Wheel graph (7 vertices)**: Hub connected to all vertices (has solution)
5. **Sparse graph**: Path that can return to start but not a valid cycle (no solution)

### Expected Performance Characteristics

1. **Scalability**: 
   - Performance improves with more threads for graphs with high branching factor
   - Diminishing returns as thread count exceeds available cores

2. **Early Termination**:
   - Once a solution is found, all threads stop searching
   - This provides significant speedup when solution is found early

3. **Load Balancing**:
   - C++: Threads are divided evenly among branches
   - Java: Work-stealing automatically balances load

4. **Overhead**:
   - Thread creation and synchronization have overhead
   - For small graphs, sequential search may be faster
   - Parallelization is most beneficial for larger, more complex graphs

5. **C++ vs Java Performance**:
   - **C++ advantages**: Native code compilation, no JVM overhead, direct memory access
   - **C++ disadvantages**: `std::async` creates threads dynamically, which has overhead at each recursion level
   - **Java advantages**: 
     - ForkJoinPool uses a thread pool with work-stealing (no thread creation overhead)
     - JIT compiler optimizations accumulate over time
     - Better load balancing for irregular workloads
   - **When Java can be faster** 
     - Larger graphs (9+ vertices) where thread creation overhead in C++ accumulates
     - Highly irregular workloads with load imbalance (work-stealing excels)
     - When using significantly more threads than CPU cores (oversubscription)
     - After JIT warm-up on long-running computations
   - **When C++ is faster**:
     - Small to medium graphs where thread creation overhead is minimal
     - Regular, balanced workloads
     - Short-running computations before JIT optimizations kick in
   - **Key Insight**: Java's ForkJoinPool with work-stealing can outperform C++'s `std::async` approach on larger graphs because:
     1. **Thread Pool Reuse**: Java reuses threads from a pool, eliminating per-task thread creation overhead
     2. **Work-Stealing**: Automatically handles load imbalance by allowing idle threads to steal work
     3. **Fine-Grained Tasks**: Creates a separate task for each neighbor (matching our C++ implementation)
     4. **JIT Optimizations**: Compiler optimizes hot code paths over time
   - **Implementation Differences** (based on reference implementations):
     - **C++**: Uses `std::async` which creates threads dynamically - overhead accumulates on larger graphs
     - **Java**: Uses `ForkJoinPool` thread pool - threads are reused, making it more efficient for many small tasks
     - **Task Granularity**: Both now create fine-grained tasks (one per neighbor), but Java's work-stealing handles them more efficiently
   - **Note**: On very large graphs (10+ vertices) with irregular workloads, Java's work-stealing and thread pool reuse can make it faster than C++'s dynamic thread creation approach.

### Running the Programs

#### Quick Performance Comparison

The easiest way to run both implementations and compare their performance:

```bash
cd pdp_lab6
./compare_performance.sh
```

This script will:
- Build the C++ program if needed
- Compile the Java program if needed
- Run both implementations
- Display a side-by-side performance comparison table
- Show detailed output from both programs
- Calculate average times and overall speedup

#### Individual Implementations

**C++ Implementation:**

```bash
cd pdp_lab6
mkdir -p cmake-build-debug
cd cmake-build-debug
cmake ..
make
./pdp_lab6
```

**Java Implementation:**

```bash
cd pdp_lab6
javac HamiltonianForkJoin.java
java HamiltonianForkJoin
```

### Performance Comparison

The implementations can be compared by:

1. **Execution Time**: Measure time to find solution (or determine no solution exists)
2. **Thread Utilization**: Monitor CPU usage across threads
3. **Scalability**: Test with different thread counts (1, 2, 4, 8, 16, ...)
4. **Graph Size**: Test with progressively larger graphs

### Example Output

```
========================================
HAMILTONIAN CYCLE FINDER (C++)
Using 8 threads
========================================

===== Test: Directed 5-cycle =====
Hamiltonian cycle found: 0 1 2 3 4 0 
Execution time: 0.123 ms

===== Test: Complete graph with 6 vertices =====
Hamiltonian cycle found: 0 1 2 3 4 5 0 
Execution time: 0.456 ms

===== Test: Line graph (no cycle) =====
No Hamiltonian cycle found.
Execution time: 0.089 ms
```

## Implementation Details

### C++ Key Features

- Uses `std::async` with `std::launch::async` for true parallelism
- Creates a task for EACH neighbor (fine-grained, matching Java's approach)
- Recursive thread allocation: threads distributed among neighbors
- **Key Limitation**: `std::async` creates threads dynamically at each recursion level
  - This creates overhead that accumulates on larger graphs
  - Unlike Java's thread pool, threads are not reused
- RAII-style mutex locking for exception safety

### Java Key Features

- Extends `RecursiveTask<Boolean>` for ForkJoin integration
- Work-stealing automatically handles load balancing
- Immutable task state (copies of path and used arrays)
- Efficient early termination with atomic flags

## Complexity Analysis

- **Time Complexity**: O(N!) worst case (must explore all permutations)
- **Space Complexity**: O(N) for path and used arrays per thread
- **Parallel Speedup**: Up to P× (where P = number of threads) for ideal cases
- **Actual Speedup**: Typically less due to:
  - Synchronization overhead
  - Load imbalance
  - Early termination benefits

## Conclusion

Both implementations successfully parallelize the Hamiltonian cycle search using different concurrency models:

- **C++ (std::async)**: Explicit thread management with fine-grained control
- **Java (ForkJoinPool)**: Automatic work-stealing with less manual management

The choice between implementations depends on:
- Programming language preference
- Need for explicit control vs. automatic load balancing
- Integration with existing codebase

Both approaches demonstrate effective parallelization of backtracking search algorithms.

