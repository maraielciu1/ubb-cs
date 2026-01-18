#include <algorithm>
#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using Graph = std::vector<std::vector<int>>;
using Path = std::vector<int>;

class HamiltonianSolver
{
public:
        const Graph graph;       // Input directed graph
        const int N;             // Number of vertices in the graph
        std::atomic<bool> found; // Indicates if a solution has been found
        std::mutex solution_mtx; // Guards access to the solution path
        Path solution;           // Stores the solution cycle if found

        explicit HamiltonianSolver(Graph g) : graph(std::move(g)), N(graph.size()), found(false) {}

        void solve(unsigned nrThreads, int startVertex = 0)
        {
                Path visited;
                visited.push_back(startVertex);
                std::vector<bool> used(N, false);
                used[startVertex] = true;
                search(visited, used, startVertex, nrThreads);
        }

        void search(Path path, std::vector<bool> used, int current, unsigned threads)
        {
                if (found)
                        return;

                // Check if all nodes are visited and can close a cycle
                if (path.size() == N)
                {
                        for (int neighbor : graph[current])
                        {
                                if (neighbor == path.front())
                                { // Can close the cycle
                                        bool wasSet = !found.exchange(true);
                                        if (wasSet)
                                        {
                                                std::lock_guard<std::mutex> lk(solution_mtx);
                                                solution = path;
                                                solution.push_back(path.front());
                                        }
                                        return;
                                }
                        }
                        return;
                }

                // Get all unused neighbors
                std::vector<int> nexts;
                for (int neighbor : graph[current])
                {
                        if (!used[neighbor])
                                nexts.push_back(neighbor);
                }

                if (nexts.empty())
                        return; // Dead end

                // Number of subtasks to spawn equals number of neighbors (up to thread limit)
                unsigned subtasks = std::min<unsigned>(threads, nexts.size());

                if (threads > 1 && subtasks > 1)
                {
                        // Allocate threads among neighbor branches
                        std::vector<std::future<void>> futures;
                        size_t consumed = 0;

                        for (unsigned i = 0; i < subtasks; ++i)
                        {
                                // Divide neighbors among subtasks
                                size_t left = nexts.size() - consumed;
                                size_t alloc = left / (subtasks - i);
                                if (left % (subtasks - i))
                                        ++alloc;

                                std::vector<int> thisChunk(nexts.begin() + consumed,
                                                           nexts.begin() + consumed + alloc);

                                consumed += alloc;

                                futures.emplace_back(std::async(std::launch::async, [this, thisChunk, threads, subtasks, path, used]()
                                                                {
                    for (int neighbor : thisChunk) {
                        if (this->found) return;

                        auto pathCopy = path;
                        pathCopy.push_back(neighbor);
                        auto usedCopy = used;
                        usedCopy[neighbor] = true;

                        this->search(std::move(pathCopy), std::move(usedCopy), neighbor, threads / subtasks);
                    } }));
                        }

                        for (auto &future : futures)
                                future.wait();
                }
                else
                {
                        // Sequential search when only one thread is available
                        for (int neighbor : nexts)
                        {
                                if (found)
                                        return;

                                path.push_back(neighbor);
                                used[neighbor] = true;
                                search(path, used, neighbor, 1);
                                used[neighbor] = false;
                                path.pop_back();
                        }
                }
        }
};

/**
 * Generates a test graph and attempts to find a Hamiltonian cycle.
 */
int main()
{
        unsigned numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0)
                numThreads = 4;

        struct TestGraph
        {
                std::string name;
                Graph graph;
        };

        // Define several graphs (some have Hamiltonian cycles, some don't)
        std::vector<TestGraph> tests{
            {"Directed 5-cycle",
             {
                 {1, 2}, // 0
                 {2, 3}, // 1
                 {3, 4}, // 2
                 {4, 0}, // 3
                 {0, 1}  // 4
             }},
            {"Complete graph with 6 vertices",
             {
                 {1, 2, 3, 4, 5}, // 0
                 {0, 2, 3, 4, 5}, // 1
                 {0, 1, 3, 4, 5}, // 2
                 {0, 1, 2, 4, 5}, // 3
                 {0, 1, 2, 3, 5}, // 4
                 {0, 1, 2, 3, 4}  // 5
             }},
            {"Line graph (no cycle)",
             {
                 {1}, // 0
                 {2}, // 1
                 {3}, // 2
                 {4}, // 3
                 {}   // 4
             }},
            {"Wheel graph with 7 vertices",
             {
                 {1, 2, 3, 4, 5, 6}, // 0 (hub)
                 {0, 2},             // 1
                 {0, 3},             // 2
                 {0, 4},             // 3
                 {0, 5},             // 4
                 {0, 6},             // 5
                 {0, 1}              // 6
             }},
            {"Wheel graph with 10 vertices",
             {
                 {1, 2, 3, 4, 5, 6, 7, 8, 9}, // 0 (hub)
                 {0, 2, 9},                   // 1
                 {0, 1, 3},                   // 2
                 {0, 2, 4},                   // 3
                 {0, 3, 5},                   // 4
                 {0, 4, 6},                   // 5
                 {0, 5, 7},                   // 6
                 {0, 6, 8},                   // 7
                 {0, 7, 9},                   // 8
                 {0, 8, 1}                    // 9
             }},
            {"Sparse graph (Hamiltonian path but no cycle)",
             {
                 {1}, // 0
                 {2}, // 1
                 {3}, // 2
                 {4}, // 3
                 {0}  // 4
             }},
            {"Large irregular graph (10 vertices, work-stealing friendly)",
             {
                 // Vertex 0: hub with many connections
                 {1, 2, 3, 4, 5, 6, 7, 8, 9}, // 0
                                              // Vertices 1-3: moderate branching
                 {2, 3, 4, 5},                // 1
                 {3, 4, 5, 6},                // 2
                 {4, 5, 6, 7},                // 3
                                              // Vertices 4-6: high branching (many choices)
                 {5, 6, 7, 8, 9, 0},          // 4
                 {6, 7, 8, 9, 0, 1},          // 5
                 {7, 8, 9, 0, 1, 2},          // 6
                                              // Vertices 7-8: low branching
                 {8, 9, 0},                   // 7
                 {9, 0, 1},                   // 8
                                              // Vertex 9: connects back to start
                 {0, 1, 2, 3, 4, 5, 6, 7, 8}  // 9
             }},
            {"Very large graph with highly irregular branching (12 vertices)",
             {
                 // Start: many quick dead ends, one long path
                 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, // 0 - many branches
                                                      // Many vertices with single outgoing edge (quick dead ends)
                 {2},                                 // 1 - dead end path
                 {3},                                 // 2 - dead end path
                 {4},                                 // 3 - dead end path
                 {5},                                 // 4 - dead end path
                 {6},                                 // 5 - dead end path
                 {7},                                 // 6 - dead end path
                 {8},                                 // 7 - dead end path
                 {9},                                 // 8 - dead end path
                 {10},                                // 9 - dead end path
                 {11},                                // 10 - dead end path
                                                      // Vertex 11: connects to many, creating complex search
                 {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}   // 11 - back to many vertices
             }},
            {"Complete graph with 8 vertices (many paths, work-stealing advantage)",
             {
                 {1, 2, 3, 4, 5, 6, 7}, // 0
                 {0, 2, 3, 4, 5, 6, 7}, // 1
                 {0, 1, 3, 4, 5, 6, 7}, // 2
                 {0, 1, 2, 4, 5, 6, 7}, // 3
                 {0, 1, 2, 3, 5, 6, 7}, // 4
                 {0, 1, 2, 3, 4, 6, 7}, // 5
                 {0, 1, 2, 3, 4, 5, 7}, // 6
                 {0, 1, 2, 3, 4, 5, 6}  // 7
             }},
            {"Complete graph with 9 vertices (large search space)",
             {
                 {1, 2, 3, 4, 5, 6, 7, 8}, // 0
                 {0, 2, 3, 4, 5, 6, 7, 8}, // 1
                 {0, 1, 3, 4, 5, 6, 7, 8}, // 2
                 {0, 1, 2, 4, 5, 6, 7, 8}, // 3
                 {0, 1, 2, 3, 5, 6, 7, 8}, // 4
                 {0, 1, 2, 3, 4, 6, 7, 8}, // 5
                 {0, 1, 2, 3, 4, 5, 7, 8}, // 6
                 {0, 1, 2, 3, 4, 5, 6, 8}, // 7
                 {0, 1, 2, 3, 4, 5, 6, 7}  // 8
             }},
            {"Complete graph with 100 vertices (very large search space)",
             []()
             {
                     Graph g(100);
                     for (int i = 0; i < 100; ++i)
                     {
                             for (int j = 0; j < 100; ++j)
                             {
                                     if (i != j)
                                     {
                                             g[i].push_back(j);
                                     }
                             }
                     }
                     return g;
             }()},
            {"Complete graph with 500 vertices (very large search space)",
             []()
             {
                     Graph g(500);
                     for (int i = 0; i < 500; ++i)
                     {
                             for (int j = 0; j < 500; ++j)
                             {
                                     if (i != j)
                                     {
                                             g[i].push_back(j);
                                     }
                             }
                     }
                     return g;
             }()}};

        for (const auto &test : tests)
        {
                std::cout << "\n===== Test: " << test.name << " =====\n";
                auto start = std::chrono::high_resolution_clock::now();
                HamiltonianSolver solver(test.graph);
                solver.solve(numThreads, 0);
                auto end = std::chrono::high_resolution_clock::now();
                double ms = std::chrono::duration<double, std::milli>(end - start).count();

                if (solver.found)
                {
                        std::lock_guard<std::mutex> lk(solver.solution_mtx);
                        std::cout << "Hamiltonian cycle found: ";
                        for (int v : solver.solution)
                                std::cout << v << " ";
                        std::cout << "\n";
                }
                else
                {
                        std::cout << "No Hamiltonian cycle found.\n";
                }
                std::cout << "Execution time: " << ms << " ms\n";
        }

        return 0;
}
