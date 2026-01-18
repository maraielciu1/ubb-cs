# MPI Implementation Explanation

This document explains all MPI functions and concepts used in the `pdp_lab7` implementation.

## Table of Contents
1. [MPI Basics](#mpi-basics)
2. [MPI Functions Used](#mpi-functions-used)
3. [Algorithm-Specific Usage](#algorithm-specific-usage)
4. [Communication Patterns](#communication-patterns)

---

## MPI Basics

### What is MPI?
**MPI (Message Passing Interface)** is a standard for distributed-memory parallel programming. It allows multiple processes (running on the same machine or across a network) to communicate and coordinate.

### Key Concepts:
- **Process**: An independent program instance with its own memory
- **Rank**: Unique identifier for each process (0, 1, 2, ...)
- **Communicator**: A group of processes that can communicate (usually `MPI_COMM_WORLD`)
- **Collective Operations**: Operations where all processes participate
- **Point-to-Point Operations**: Direct communication between two processes

---

## MPI Functions Used

### 1. `MPI_Init(&argc, &argv)`
**Location**: `main.cpp:230`

**Purpose**: Initializes the MPI environment. Must be called before any other MPI function.

**Parameters**:
- `argc`: Pointer to argument count (from `main`)
- `argv`: Pointer to argument vector (from `main`)

**Usage**:
```cpp
int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);  // Initialize MPI
    // ... rest of program
    MPI_Finalize();  // Clean up
}
```

**Why it's needed**: Sets up the MPI runtime, creates processes, and establishes communication channels.

---

### 2. `MPI_Finalize()`
**Location**: `main.cpp:306`

**Purpose**: Cleans up MPI resources and terminates the MPI environment. Must be called at the end of the program.

**Usage**:
```cpp
MPI_Finalize();  // Clean up MPI before program exit
return 0;
```

**Why it's needed**: Properly shuts down MPI, frees resources, and ensures all pending communications complete.

---

### 3. `MPI_Comm_rank(MPI_COMM_WORLD, &rank)`
**Location**: Used throughout (e.g., `polynomial_mpi.cpp:287`, `bigint_mpi.cpp:13`)

**Purpose**: Gets the rank (ID) of the current process in the communicator.

**Parameters**:
- `MPI_COMM_WORLD`: The communicator (all processes)
- `&rank`: Output parameter - receives the rank (0, 1, 2, ...)

**Usage**:
```cpp
int rank;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
if (rank == 0) {
    // This is the master/root process
} else {
    // This is a worker process
}
```

**Why it's needed**: Processes need to know their identity to decide what work to do (master vs worker).

---

### 4. `MPI_Comm_size(MPI_COMM_WORLD, &size)`
**Location**: Used throughout (e.g., `polynomial_mpi.cpp:288`, `bigint_mpi.cpp:14`)

**Purpose**: Gets the total number of processes in the communicator.

**Parameters**:
- `MPI_COMM_WORLD`: The communicator
- `&size`: Output parameter - receives the total number of processes

**Usage**:
```cpp
int size;
MPI_Comm_size(MPI_COMM_WORLD, &size);
// If run with: mpirun -np 4 ./program
// Then size = 4
```

**Why it's needed**: To know how many processes are available for parallel work distribution.

---

### 5. `MPI_Barrier(MPI_COMM_WORLD)`
**Location**: `main.cpp:59`, `main.cpp:75`, `main.cpp:128`, `main.cpp:250`, etc.

**Purpose**: Synchronization point - all processes wait here until every process reaches the barrier.

**Parameters**:
- `MPI_COMM_WORLD`: The communicator

**Usage**:
```cpp
MPI_Barrier(MPI_COMM_WORLD);  // All processes wait here
// After barrier, all processes continue together
```

**Why it's needed**: 
- Ensures all processes are synchronized before timing measurements
- Prevents race conditions in distributed algorithms
- Used before benchmarks to get accurate timing

**Visual Example**:
```
Process 0: [work] --->|barrier|---> [continue]
Process 1: [work] --->|barrier|---> [continue]
Process 2: [work] --->|barrier|---> [continue]
Process 3: [work] --->|barrier|---> [continue]
                    ↑
              All wait here
```

---

### 6. `MPI_Bcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)`
**Location**: 
- `polynomial_mpi.cpp:60-61` (sizes)
- `polynomial_mpi.cpp:69` (polynomial B)
- `polynomial_mpi.cpp:118-119` (gather metadata)
- `polynomial_mpi.cpp:170-171` (Karatsuba base case)
- `bigint_mpi.cpp:22` (sizes)
- `bigint_mpi.cpp:46-47` (digits)
- `bigint_mpi.cpp:53` (signs)

**Purpose**: **Broadcast** - sends data from one process (root) to all other processes.

**Parameters**:
- `buffer`: Pointer to data (on root: data to send; on others: where to receive)
- `count`: Number of elements to broadcast
- `datatype`: MPI data type (e.g., `MPI_INT`, `MPI_LONG_LONG`)
- `root`: Rank of the process sending the data (usually 0)
- `comm`: Communicator

**Usage Example**:
```cpp
int n = (rank == 0) ? a.size() : 0;  // Only rank 0 knows size
MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
// After this, ALL processes know n
```

**Why it's needed**: 
- Distributes input data to all processes
- Efficient: O(log P) communication steps using a tree structure
- All processes need the same data to participate in computation

**Visual Example** (4 processes):
```
Before:
Rank 0: n = 100
Rank 1: n = ???
Rank 2: n = ???
Rank 3: n = ???

MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

After:
Rank 0: n = 100
Rank 1: n = 100  ← received
Rank 2: n = 100  ← received
Rank 3: n = 100  ← received
```

**Tree Structure** (logarithmic steps):
```
        Rank 0 (root)
       /              \
   Rank 0            Rank 2
   /    \            /    \
Rank 0  Rank 1   Rank 2  Rank 3
```

---

### 7. `MPI_Scatterv(void* sendbuf, int* sendcounts, int* displs, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)`
**Location**: `polynomial_mpi.cpp:89-99`

**Purpose**: **Scatter with variable sizes** - distributes different-sized chunks of data from root to all processes.

**Parameters**:
- `sendbuf`: Source buffer on root (nullptr on non-root)
- `sendcounts`: Array of chunk sizes for each process
- `displs`: Array of displacements (offsets) in sendbuf for each process
- `sendtype`: MPI data type of send buffer
- `recvbuf`: Destination buffer on each process
- `recvcount`: Size of receive buffer (can vary per process)
- `recvtype`: MPI data type of receive buffer
- `root`: Rank of sending process (usually 0)
- `comm`: Communicator

**Usage Example**:
```cpp
// Root prepares chunks
std::vector<int> sendcounts(size), displs(size);
if (rank == 0) {
    int offset = 0;
    for (int i = 0; i < size; ++i) {
        sendcounts[i] = base_chunk + (i < remainder ? 1 : 0);
        displs[i] = offset;
        offset += sendcounts[i];
    }
}

// Scatter chunks
Poly a_local(local_chunk);
MPI_Scatterv(
    rank == 0 ? a.data() : nullptr,  // Source (only on root)
    sendcounts.data(),                // Chunk sizes
    displs.data(),                    // Offsets
    MPI_LONG_LONG,                    // Data type
    a_local.data(),                   // Destination
    local_chunk,                      // Receive size
    MPI_LONG_LONG,
    0,                                // Root
    MPI_COMM_WORLD
);
```

**Why it's needed**: 
- Distributes work evenly when data size isn't divisible by number of processes
- Each process gets its own chunk to work on independently
- More flexible than `MPI_Scatter` (which requires equal chunk sizes)

**Visual Example** (polynomial A with 10 coefficients, 4 processes):
```
Before (on rank 0):
A = [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9]

sendcounts = [3, 3, 2, 2]  // Uneven distribution
displs = [0, 3, 6, 8]       // Starting positions

After MPI_Scatterv:
Rank 0: [a0, a1, a2]        ← 3 elements
Rank 1: [a3, a4, a5]        ← 3 elements
Rank 2: [a6, a7]            ← 2 elements
Rank 3: [a8, a9]            ← 2 elements
```

---

### 8. `MPI_Gatherv(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int* recvcounts, int* displs, MPI_Datatype recvtype, int root, MPI_Comm comm)`
**Location**: `polynomial_mpi.cpp:128-138`

**Purpose**: **Gather with variable sizes** - collects different-sized chunks from all processes to root.

**Parameters**:
- `sendbuf`: Data to send from each process
- `sendcount`: Size of data being sent (can vary)
- `sendtype`: MPI data type of send buffer
- `recvbuf`: Destination buffer on root (nullptr on non-root)
- `recvcounts`: Array of sizes expected from each process
- `displs`: Array of displacements in recvbuf for each process
- `recvtype`: MPI data type of receive buffer
- `root`: Rank of receiving process (usually 0)
- `comm`: Communicator

**Usage Example**:
```cpp
// All processes must have recvcounts and recvdispls arrays
std::vector<int> recvcounts(size), recvdispls(size);
if (rank == 0) {
    for (int i = 0; i < size; ++i) {
        recvcounts[i] = sendcounts[i] + m - 1;  // Result size
        recvdispls[i] = displs[i];
    }
}
// Broadcast metadata to all processes (required!)
MPI_Bcast(recvcounts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Bcast(recvdispls.data(), size, MPI_INT, 0, MPI_COMM_WORLD);

// Gather results
MPI_Gatherv(
    local_result.data(),              // What to send
    local_result.size(),              // Size of my data
    MPI_LONG_LONG,
    rank == 0 ? gathered.data() : nullptr,  // Where to receive (root only)
    recvcounts.data(),                // Expected sizes
    recvdispls.data(),                // Offsets
    MPI_LONG_LONG,
    0,                                // Root
    MPI_COMM_WORLD
);
```

**Why it's needed**: 
- Collects partial results from all processes
- Handles variable-sized results (important for polynomial multiplication where result size depends on chunk size)
- Inverse operation of `MPI_Scatterv`

**Visual Example**:
```
Before:
Rank 0: [r0, r1, r2]        (3 elements)
Rank 1: [r3, r4, r5]        (3 elements)
Rank 2: [r6, r7]            (2 elements)
Rank 3: [r8, r9]            (2 elements)

After MPI_Gatherv (on rank 0):
gathered = [r0, r1, r2, r3, r4, r5, r6, r7, r8, r9]
```

**Important Note**: `recvcounts` and `recvdispls` must be allocated and populated on ALL processes, even though only root uses them. This is an MPI requirement.

---

### 9. `MPI_Reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)`
**Location**: `bigint_mpi.cpp:79-81`

**Purpose**: **Reduction** - combines data from all processes using an operation (sum, max, min, etc.) and stores result on root.

**Parameters**:
- `sendbuf`: Input data from each process
- `recvbuf`: Output buffer on root (nullptr on non-root)
- `count`: Number of elements
- `datatype`: MPI data type
- `op`: Reduction operation (`MPI_SUM`, `MPI_MAX`, `MPI_MIN`, etc.)
- `root`: Rank where result is stored
- `comm`: Communicator

**Usage Example**:
```cpp
// Each process computes partial results
std::vector<long long> local_result(resultSize, 0);
// ... compute local_result ...

// Sum all local results into global_result on rank 0
std::vector<long long> global_result;
if (rank == 0) {
    global_result.resize(resultSize, 0);
}

MPI_Reduce(
    local_result.data(),                    // What to send
    rank == 0 ? global_result.data() : nullptr,  // Where to receive (root only)
    resultSize,                             // Number of elements
    MPI_LONG_LONG,                          // Data type
    MPI_SUM,                                // Operation: sum
    0,                                      // Root
    MPI_COMM_WORLD
);
```

**Why it's needed**: 
- Efficiently combines partial results from all processes
- Built-in operations are optimized (tree-based reduction)
- Perfect for parallel computations where each process computes part of the result

**Visual Example** (summing arrays):
```
Before:
Rank 0: [1, 2, 3]
Rank 1: [4, 5, 6]
Rank 2: [7, 8, 9]
Rank 3: [10, 11, 12]

MPI_Reduce(..., MPI_SUM, 0, ...);

After (on rank 0):
global_result = [22, 26, 30]  // Element-wise sum
// 1+4+7+10=22, 2+5+8+11=26, 3+6+9+12=30
```

**Tree Reduction Structure**:
```
        [22,26,30]  ← Final result
       /            \
   [5,7,9]        [17,19,21]
   /    \          /      \
[1,2,3] [4,5,6] [7,8,9] [10,11,12]
```

---

### 10. `MPI_Send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)`
**Location**: `polynomial_mpi.cpp:212-216`, `polynomial_mpi.cpp:252`

**Purpose**: **Point-to-point send** - sends data from current process to a specific destination process.

**Parameters**:
- `buf`: Buffer containing data to send
- `count`: Number of elements
- `datatype`: MPI data type
- `dest`: Rank of destination process
- `tag`: Message tag (for message matching, usually 0)
- `comm`: Communicator

**Usage Example**:
```cpp
// Master sends task to worker
int task_id = 1;
int size_a = task.poly_a.size();
MPI_Send(&task_id, 1, MPI_INT, next_worker, 0, MPI_COMM_WORLD);
MPI_Send(&size_a, 1, MPI_INT, next_worker, 0, MPI_COMM_WORLD);
MPI_Send(task.poly_a.data(), size_a, MPI_LONG_LONG, next_worker, 0, MPI_COMM_WORLD);
```

**Why it's needed**: 
- Direct communication between specific processes
- Used in master-worker patterns for task distribution
- More flexible than collective operations for irregular communication

**Visual Example**:
```
Rank 0 (Master)              Rank 1 (Worker)
     |                            |
     |--- MPI_Send(task) -------->|
     |                            | (receives and processes)
```

**Blocking Behavior**: `MPI_Send` is **blocking** - it waits until the message is sent (but not necessarily received). The corresponding `MPI_Recv` must be called on the destination.

---

### 11. `MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status* status)`
**Location**: `polynomial_mpi.cpp:229-233`, `polynomial_mpi.cpp:260-270`

**Purpose**: **Point-to-point receive** - receives data from a specific source process.

**Parameters**:
- `buf`: Buffer to store received data
- `count`: Maximum number of elements to receive
- `datatype`: MPI data type
- `source`: Rank of source process (or `MPI_ANY_SOURCE` for any process)
- `tag`: Message tag to match (or `MPI_ANY_TAG` for any tag)
- `comm`: Communicator
- `status`: Status object (can be `MPI_STATUS_IGNORE` if not needed)

**Usage Example**:
```cpp
// Worker receives task from master
int task_id;
MPI_Recv(&task_id, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

if (task_id == -1) break;  // Termination signal

int size_a, size_b;
MPI_Recv(&size_a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
MPI_Recv(&size_b, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

Poly poly_a(size_a), poly_b(size_b);
MPI_Recv(poly_a.data(), size_a, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
MPI_Recv(poly_b.data(), size_b, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
```

**Why it's needed**: 
- Receives data sent via `MPI_Send`
- Blocking operation - waits until message arrives
- Used in master-worker patterns

**Visual Example**:
```
Rank 0 (Master)              Rank 1 (Worker)
     |                            |
     |--- MPI_Send(task) -------->|
     |                            |--- MPI_Recv(task) ---|
     |                            | (blocks until received)
```

**Blocking Behavior**: `MPI_Recv` **blocks** until the matching message arrives. If no message is available, the process waits.

---

## MPI Data Types

### Common MPI Data Types Used:

| MPI Type | C++ Type | Size | Usage |
|----------|----------|------|-------|
| `MPI_INT` | `int` | 4 bytes | Sizes, indices, flags |
| `MPI_LONG_LONG` | `long long` | 8 bytes | Polynomial coefficients, BigInt digits |
| `MPI_CHAR` | `char` | 1 byte | Characters, strings |

**Why use MPI types**: MPI needs to know data sizes for proper serialization/deserialization across different architectures (endianness, size differences).

---

## Algorithm-Specific Usage

### 1. Regular Polynomial Multiplication (`multiply_naive_mpi`)

**Communication Pattern**: Scatter-Gather

**Steps**:
1. **Broadcast sizes** (`MPI_Bcast`): All processes need to know polynomial sizes
2. **Broadcast polynomial B** (`MPI_Bcast`): All processes need full polynomial B
3. **Scatter polynomial A** (`MPI_Scatterv`): Each process gets a chunk of A
4. **Local computation**: Each process multiplies its chunk of A with full B
5. **Gather results** (`MPI_Gatherv`): Collect partial results from all processes
6. **Merge on root**: Root combines overlapping coefficient regions

**Why this pattern**:
- Polynomial B is needed in full by all processes (used in every multiplication)
- Polynomial A can be split (each process handles different coefficients)
- Result chunks overlap (coefficient positions i+j span multiple chunks)

**Example** (degree 3 polynomial, 2 processes):
```
A = [a0, a1, a2, a3]
B = [b0, b1, b2, b3]

After Scatterv:
Rank 0: A_chunk = [a0, a1]
Rank 1: A_chunk = [a2, a3]
Both: B_full = [b0, b1, b2, b3]

Local computation:
Rank 0 computes: a0*b0, a0*b1, a0*b2, a0*b3, a1*b0, a1*b1, a1*b2, a1*b3
Rank 1 computes: a2*b0, a2*b1, a2*b2, a2*b3, a3*b0, a3*b1, a3*b2, a3*b3

After Gatherv (on rank 0):
Result positions overlap:
- Position 0: a0*b0 (from rank 0)
- Position 1: a0*b1 + a1*b0 (from rank 0)
- Position 2: a0*b2 + a1*b1 + a2*b0 (from rank 0 and 1)
- Position 3: a0*b3 + a1*b2 + a2*b1 + a3*b0 (from rank 0 and 1)
- Position 4: a1*b3 + a2*b2 + a3*b1 (from rank 0 and 1)
- Position 5: a2*b3 + a3*b2 (from rank 1)
- Position 6: a3*b3 (from rank 1)
```

---

### 2. Karatsuba Polynomial Multiplication (`multiply_karatsuba_mpi`)

**Communication Pattern**: Master-Worker (Point-to-Point)

**Steps**:
1. **Broadcast sizes** (`MPI_Bcast`): All processes know sizes
2. **Base case check**: Small polynomials computed locally (all processes participate)
3. **Master decomposes**: Root splits into 3 subproblems (P1, P2, P3)
4. **Master distributes** (`MPI_Send`): Sends subproblems to workers
5. **Workers receive** (`MPI_Recv`): Workers get tasks and compute
6. **Workers send results** (`MPI_Send`): Send results back to master
7. **Master receives** (`MPI_Recv`): Collects all results
8. **Master reconstructs**: Combines P1, P2, P3 into final result
9. **Termination signals** (`MPI_Send`): Master tells workers to stop

**Why this pattern**:
- Karatsuba creates 3 recursive subproblems
- Subproblems have different sizes (irregular workload)
- Point-to-point communication is more flexible than collective operations
- Master can assign tasks dynamically

**Visual Example** (4 processes):
```
Master (Rank 0):
  Decompose: P1, P2, P3
  Send P1 → Rank 1
  Send P2 → Rank 2
  Send P3 → Rank 3
  Compute locally if needed

Workers (Rank 1, 2, 3):
  Receive task
  Compute using sequential Karatsuba
  Send result back

Master:
  Receive P1, P2, P3
  Reconstruct final result
  Send termination signal to all workers
```

---

### 3. BigInt Multiplication (`multiplyBigIntRegularMPI`)

**Communication Pattern**: Broadcast-Reduce

**Steps**:
1. **Broadcast sizes** (`MPI_Bcast`): All processes know digit counts
2. **Broadcast digits** (`MPI_Bcast`): All processes get full BigInt A and B
3. **Broadcast signs** (`MPI_Bcast`): All processes know sign information
4. **Local computation**: Each process computes different result positions
   - Process i computes: positions i, i+numProcs, i+2*numProcs, ...
5. **Reduce results** (`MPI_Reduce` with `MPI_SUM`): Sum all partial results
6. **Carry propagation** (on root): Convert to proper BigInt format

**Why this pattern**:
- Each process computes different result digits (no overlap)
- `MPI_Reduce` with `MPI_SUM` efficiently combines results
- Simpler than gather because results don't overlap

**Example** (4 processes, computing result[0..9]):
```
Process 0 computes: result[0], result[4], result[8]
Process 1 computes: result[1], result[5], result[9]
Process 2 computes: result[2], result[6]
Process 3 computes: result[3], result[7]

After MPI_Reduce(MPI_SUM):
All partial results summed into global_result on rank 0
```

**Work Distribution**:
```cpp
for (size_t resultPos = rank; resultPos < resultSize; resultPos += numProcs) {
    // Compute result[resultPos]
    // Process 0: 0, 4, 8, 12, ...
    // Process 1: 1, 5, 9, 13, ...
    // Process 2: 2, 6, 10, 14, ...
    // Process 3: 3, 7, 11, 15, ...
}
```

---

## Communication Patterns Summary

| Pattern | Functions Used | When to Use |
|---------|---------------|-------------|
| **Broadcast** | `MPI_Bcast` | One-to-all: distribute input data |
| **Scatter-Gather** | `MPI_Scatterv`, `MPI_Gatherv` | Distribute work, collect results (variable sizes) |
| **Reduce** | `MPI_Reduce` | All-to-one: combine partial results |
| **Master-Worker** | `MPI_Send`, `MPI_Recv` | Dynamic task distribution, irregular workloads |

---

## Performance Considerations

### 1. **Communication Overhead**
- MPI operations have latency and bandwidth costs
- Collective operations (Bcast, Reduce) are optimized (tree-based)
- Point-to-point (Send/Recv) has less overhead for small messages

### 2. **Load Balancing**
- `MPI_Scatterv` handles uneven data distribution
- Master-worker pattern allows dynamic load balancing
- Regular BigInt multiplication uses round-robin distribution

### 3. **Synchronization**
- `MPI_Barrier` ensures all processes are synchronized
- Important for accurate timing measurements
- Can be a bottleneck if processes finish at different times

### 4. **Scalability**
- Collective operations scale as O(log P) where P = number of processes
- Point-to-point operations scale as O(1) per message
- Communication overhead increases with more processes

---

## Common MPI Pitfalls

### 1. **Deadlocks**
- **Problem**: All processes waiting for each other
- **Example**: Process 0 sends, then receives; Process 1 receives, then sends
- **Solution**: Ensure matching Send/Recv pairs, or use non-blocking operations

### 2. **Not All Processes Participating**
- **Problem**: Only some processes call collective operations
- **Example**: `MPI_Bcast` called only on rank 0
- **Solution**: ALL processes must call collective operations

### 3. **Buffer Allocation**
- **Problem**: Non-root processes don't allocate receive buffers
- **Example**: `MPI_Gatherv` needs `recvcounts` on all processes
- **Solution**: Allocate arrays on all processes, even if only root uses them

### 4. **Data Type Mismatches**
- **Problem**: Sending `int` but receiving as `long long`
- **Solution**: Use matching MPI data types

---

## Running the Program

```bash
# Compile (if needed)
cd pdp_lab7
cd cmake-build-debug
cmake ..
make

# Run with 4 processes
mpirun -np 4 ./pdp_lab7

# Run with 8 processes
mpirun -np 8 ./pdp_lab7
```

The `-np` flag specifies the number of MPI processes to create. Each process will have a different rank (0, 1, 2, ...) and will participate in the parallel computation.

---

## Summary

This implementation uses MPI to distribute polynomial and BigInt multiplication across multiple processes:

1. **Collective Operations** (`Bcast`, `Scatterv`, `Gatherv`, `Reduce`): Efficient for regular data distribution and collection
2. **Point-to-Point Operations** (`Send`, `Recv`): Flexible for master-worker patterns and irregular workloads
3. **Synchronization** (`Barrier`): Ensures proper coordination and accurate timing

Each algorithm chooses the communication pattern that best fits its computation structure, balancing communication overhead with parallel speedup.


