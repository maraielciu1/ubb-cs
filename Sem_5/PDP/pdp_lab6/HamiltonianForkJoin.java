import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.*;
import java.util.stream.*;

public class HamiltonianForkJoin {
    // Graph represented as adjacency list
    private final List<List<Integer>> graph;
    private final int N;
    private final AtomicBoolean found = new AtomicBoolean(false);
    private List<Integer> solution = null;
    private final Object lock = new Object();

    public HamiltonianForkJoin(List<List<Integer>> graph) {
        this.graph = graph;
        this.N = graph.size();
    }

    public List<Integer> solve(ForkJoinPool pool) {
        List<Integer> path = new ArrayList<>();
        path.add(0);
        boolean[] used = new boolean[N];
        used[0] = true;
        pool.invoke(new ExploreTask(path, used, 0));
        return solution;
    }

    private class ExploreTask extends RecursiveTask<Boolean> {
        private final List<Integer> path;
        private final boolean[] used;
        private final int current;

        public ExploreTask(List<Integer> path, boolean[] used, int current) {
            this.path = path;
            this.used = Arrays.copyOf(used, used.length);
            this.current = current;
        }

        @Override
        protected Boolean compute() {
            if (found.get())
                return false;

            // Base case: all nodes visited, check for edge to start
            if (path.size() == N) {
                for (int neighbor : graph.get(current)) {
                    if (neighbor == path.get(0)) {
                        if (found.compareAndSet(false, true)) {
                            synchronized (lock) {
                                solution = new ArrayList<>(path);
                                solution.add(path.get(0)); // close the cycle
                            }
                        }
                        return true;
                    }
                }
                return false;
            }

            // Collect all unused neighbors
            List<Integer> candidates = graph.get(current).stream()
                    .filter(n -> !used[n]).collect(Collectors.toList());
            if (candidates.isEmpty())
                return false;

            List<ExploreTask> tasks = new ArrayList<>();
            // Branch among all neighbors in parallel
            for (int neighbor : candidates) {
                if (found.get())
                    break;
                List<Integer> nextPath = new ArrayList<>(path);
                nextPath.add(neighbor);
                boolean[] nextUsed = Arrays.copyOf(used, used.length);
                nextUsed[neighbor] = true;
                ExploreTask task = new ExploreTask(nextPath, nextUsed, neighbor);
                tasks.add(task);
            }

            boolean result = false;
            int n = tasks.size();
            for (int i = 1; i < n; ++i) {
                tasks.get(i).fork();
            }
            if (n > 0) {
                result = tasks.get(0).compute();
            }
            for (int i = 1; i < n; ++i) {
                result = tasks.get(i).join() || result;
            }
            return result;
        }
    }

    private static List<List<Integer>> generateCompleteGraph(int n) {
        List<List<Integer>> graph = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            List<Integer> neighbors = new ArrayList<>();
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    neighbors.add(j);
                }
            }
            graph.add(neighbors);
        }
        return graph;
    }

    public static void main(String[] args) {
        List<TestGraph> testGraphs = Arrays.asList(
                new TestGraph("Directed 5-cycle", Arrays.asList(
                        Arrays.asList(1, 2),
                        Arrays.asList(2, 3),
                        Arrays.asList(3, 4),
                        Arrays.asList(4, 0),
                        Arrays.asList(0, 1))),
                new TestGraph("Complete graph with 6 vertices", Arrays.asList(
                        Arrays.asList(1, 2, 3, 4, 5), // 0
                        Arrays.asList(0, 2, 3, 4, 5), // 1
                        Arrays.asList(0, 1, 3, 4, 5), // 2
                        Arrays.asList(0, 1, 2, 4, 5), // 3
                        Arrays.asList(0, 1, 2, 3, 5), // 4
                        Arrays.asList(0, 1, 2, 3, 4) // 5
                )),
                new TestGraph("Line graph (no cycle)", Arrays.asList(
                        Arrays.asList(1),
                        Arrays.asList(2),
                        Arrays.asList(3),
                        Arrays.asList(4),
                        Arrays.asList())),
                new TestGraph("Wheel graph with 7 vertices", Arrays.asList(
                        Arrays.asList(1, 2, 3, 4, 5, 6), // 0 (hub)
                        Arrays.asList(0, 2),
                        Arrays.asList(0, 3),
                        Arrays.asList(0, 4),
                        Arrays.asList(0, 5),
                        Arrays.asList(0, 6),
                        Arrays.asList(0, 1))),
                new TestGraph("Wheel graph with 10 vertices", Arrays.asList(
                        Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9), // 0 (hub)
                        Arrays.asList(0, 2, 9), // 1
                        Arrays.asList(0, 1, 3), // 2
                        Arrays.asList(0, 2, 4), // 3
                        Arrays.asList(0, 3, 5), // 4
                        Arrays.asList(0, 4, 6), // 5
                        Arrays.asList(0, 5, 7), // 6
                        Arrays.asList(0, 6, 8), // 7
                        Arrays.asList(0, 7, 9), // 8
                        Arrays.asList(0, 8, 1) // 9
                )),
                new TestGraph("Sparse graph (Hamiltonian path but no cycle)", Arrays.asList(
                        Arrays.asList(1),
                        Arrays.asList(2),
                        Arrays.asList(3),
                        Arrays.asList(4),
                        Arrays.asList(0))),
                new TestGraph("Large irregular graph (10 vertices, work-stealing friendly)", Arrays.asList(
                        // Vertex 0: hub with many connections
                        Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9), // 0
                        // Vertices 1-3: moderate branching
                        Arrays.asList(2, 3, 4, 5), // 1
                        Arrays.asList(3, 4, 5, 6), // 2
                        Arrays.asList(4, 5, 6, 7), // 3
                        // Vertices 4-6: high branching (many choices)
                        Arrays.asList(5, 6, 7, 8, 9, 0), // 4
                        Arrays.asList(6, 7, 8, 9, 0, 1), // 5
                        Arrays.asList(7, 8, 9, 0, 1, 2), // 6
                        // Vertices 7-8: low branching
                        Arrays.asList(8, 9, 0), // 7
                        Arrays.asList(9, 0, 1), // 8
                        // Vertex 9: connects back to start
                        Arrays.asList(0, 1, 2, 3, 4, 5, 6, 7, 8) // 9
                )),
                new TestGraph("Very large graph with highly irregular branching (12 vertices)", Arrays.asList(
                        // Start: many quick dead ends, one long path
                        Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11), // 0 - many branches
                        // Many vertices with single outgoing edge (quick dead ends)
                        Arrays.asList(2), // 1 - dead end path
                        Arrays.asList(3), // 2 - dead end path
                        Arrays.asList(4), // 3 - dead end path
                        Arrays.asList(5), // 4 - dead end path
                        Arrays.asList(6), // 5 - dead end path
                        Arrays.asList(7), // 6 - dead end path
                        Arrays.asList(8), // 7 - dead end path
                        Arrays.asList(9), // 8 - dead end path
                        Arrays.asList(10), // 9 - dead end path
                        Arrays.asList(11), // 10 - dead end path
                        // Vertex 11: connects to many, creating complex search
                        Arrays.asList(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) // 11 - back to many vertices
                )),
                new TestGraph("Complete graph with 8 vertices (many paths, work-stealing advantage)", Arrays.asList(
                        Arrays.asList(1, 2, 3, 4, 5, 6, 7), // 0
                        Arrays.asList(0, 2, 3, 4, 5, 6, 7), // 1
                        Arrays.asList(0, 1, 3, 4, 5, 6, 7), // 2
                        Arrays.asList(0, 1, 2, 4, 5, 6, 7), // 3
                        Arrays.asList(0, 1, 2, 3, 5, 6, 7), // 4
                        Arrays.asList(0, 1, 2, 3, 4, 6, 7), // 5
                        Arrays.asList(0, 1, 2, 3, 4, 5, 7), // 6
                        Arrays.asList(0, 1, 2, 3, 4, 5, 6) // 7
                )),
                new TestGraph("Complete graph with 9 vertices (large search space)", Arrays.asList(
                        Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8), // 0
                        Arrays.asList(0, 2, 3, 4, 5, 6, 7, 8), // 1
                        Arrays.asList(0, 1, 3, 4, 5, 6, 7, 8), // 2
                        Arrays.asList(0, 1, 2, 4, 5, 6, 7, 8), // 3
                        Arrays.asList(0, 1, 2, 3, 5, 6, 7, 8), // 4
                        Arrays.asList(0, 1, 2, 3, 4, 6, 7, 8), // 5
                        Arrays.asList(0, 1, 2, 3, 4, 5, 7, 8), // 6
                        Arrays.asList(0, 1, 2, 3, 4, 5, 6, 8), // 7
                        Arrays.asList(0, 1, 2, 3, 4, 5, 6, 7) // 8
                )),
                new TestGraph("Complete graph with 100 vertices (very large search space)",
                        generateCompleteGraph(100)),
                new TestGraph("Complete graph with 1000 vertices (very large search space)",
                        generateCompleteGraph(500)));

        int maxThreads = Runtime.getRuntime().availableProcessors();
        if (maxThreads < 2)
            maxThreads = 4;
        ForkJoinPool pool = new ForkJoinPool(maxThreads);

        for (TestGraph test : testGraphs) {
            System.out.println("\n===== Test: " + test.name + " =====");
            long start = System.nanoTime();
            HamiltonianForkJoin solver = new HamiltonianForkJoin(test.graph);
            List<Integer> cycle = solver.solve(pool);
            long end = System.nanoTime();

            if (cycle != null) {
                System.out.println("Hamiltonian cycle: " + cycle);
            } else {
                System.out.println("No Hamiltonian cycle found.");
            }
            System.out.printf("Execution time: %.2f ms\n", (end - start) / 1e6);
        }

        pool.shutdown();
    }

    private static class TestGraph {
        String name;
        List<List<Integer>> graph;

        TestGraph(String name, List<List<Integer>> graph) {
            this.name = name;
            this.graph = graph;
        }
    }
}
