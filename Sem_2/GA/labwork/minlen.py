# from collections import defaultdict, deque
#
#
# def topological_sort(graph, n):
#     in_degree = [0] * n
#     for u in range(n):
#         for v, _ in graph[u]:
#             in_degree[v] += 1
#
#     zero_in_degree = deque([u for u in range(n) if in_degree[u] == 0])
#     top_order = []
#
#     while zero_in_degree:
#         u = zero_in_degree.popleft()
#         top_order.append(u)
#         for v, _ in graph[u]:
#             in_degree[v] -= 1
#             if in_degree[v] == 0:
#                 zero_in_degree.append(v)
#
#     return top_order
#
#
# def count_min_cost_paths(graph, start, end, n):
#     top_order = topological_sort(graph, n)
#
#     minCost = [float('inf')] * n
#     pathCount = [0] * n
#
#     minCost[start] = 0
#     pathCount[start] = 1
#
#     for u in top_order:
#         for v, weight in graph[u]:
#             if minCost[v] > minCost[u] + weight:
#                 minCost[v] = minCost[u] + weight
#                 pathCount[v] = pathCount[u]
#             elif minCost[v] == minCost[u] + weight:
#                 pathCount[v] += pathCount[u]
#
#     return pathCount[end]
#
#
# # Example usage
# n = 4  # Number of vertices
# graph = defaultdict(list)
# edges = [
#     (0, 1, 2),
#     (0, 2, 4),
#     (1, 2, 1),
#     (1, 3, 2),
#     (2, 3, 1)
# ]
#
# for u, v, w in edges:
#     graph[u].append((v, w))
#
# start = 0
# end = 3
#
# print("Number of distinct minimum-cost paths from start to end:", count_min_cost_paths(graph, start, end, n))



# from collections import deque, defaultdict
#
#
# def count_min_length_paths(graph, start, end, n):
#     # Initialize distances and path counts
#     min_dist = [float('inf')] * n
#     path_count = [0] * n
#
#     # BFS queue initialization
#     queue = deque([(start, 0)])
#     min_dist[start] = 0
#     path_count[start] = 1
#
#     while queue:
#         current, dist = queue.popleft()
#
#         for neighbor in graph[current]:
#             if min_dist[neighbor] > dist + 1:
#                 # Found a shorter path to neighbor
#                 min_dist[neighbor] = dist + 1
#                 path_count[neighbor] = path_count[current]
#                 queue.append((neighbor, dist + 1))
#             elif min_dist[neighbor] == dist + 1:
#                 # Found another shortest path to neighbor
#                 path_count[neighbor] += path_count[current]
#
#     return path_count[end]
#
#
# # Example usage:
# n = 5  # Number of vertices
# graph = defaultdict(list)
# edges = [
#     (0, 1),
#     (0, 2),
#     (1, 2),
#     (1, 3),
#     (2, 3),
#     (3, 4),
#     (2,4),
#     (0,3)
#
# ]
#
# for u, v in edges:
#     graph[u].append(v)
#     graph[v].append(u)  # If the graph is undirected
#
# start = 0
# end = 4

#print("Number of distinct minimum-length paths from start to end:", count_min_length_paths(graph, start, end, n))

from collections import deque, defaultdict

#
# def find_min_len_cycle(graph, current, start, visited, path, min_cycle, cycle_path):
#     visited[current] = True
#     path.append(current)
#
#     for neighbor in graph[current]:
#         if not visited[neighbor]:
#             # Recursive DFS call
#             min_cycle, cycle_path = find_min_len_cycle(graph, neighbor, start, visited, path, min_cycle, cycle_path)
#         elif neighbor == start:
#             # Cycle detected
#             cycle_length = len(path) - path.index(start)+1
#             if cycle_length < min_cycle:
#                 min_cycle = cycle_length
#                 cycle_path = path[path.index(start):] + [start]
#
#     # Backtrack
#     path.pop()
#     visited[current] = False
#
#     return min_cycle, cycle_path
#
#
# def find_minimum_cycle(graph):
#     min_cycle = float('inf')
#     cycle_path = []
#
#     for node in graph:
#         visited = {n: False for n in graph}
#         path = []
#         min_cycle, cycle_path = find_min_len_cycle(graph, node, node, visited, path, min_cycle, cycle_path)
#
#     return (min_cycle-1 if min_cycle != float('inf') else -1), cycle_path
#
#
# # Example usage
# graph = {
#     0: [1, 2],
#     1: [2],
#     2: [1,3],
#     3: [0]
# }
#
# min_cycle_length, min_cycle_path = find_minimum_cycle(graph)
# print("Minimum cycle length:", min_cycle_length)
# print("Cycle path:", min_cycle_path)

from collections import defaultdict

# Class to represent a graph
class Graph:

    def _init_(self, vertices):
        self.V = vertices  # No. of vertices
        self.graph = []  # default dictionary
        # to store graph

    # function to add an edge to graph
    def addEdge(self, u, v, w):
        self.graph.append([u, v, w])

    # utility function used to print the solution
    def printArr(self, dist):
        print("Vertex Distance from Source")
        for i in range(self.V):
            print("{0}\t\t{1}".format(i, dist[i]))

    # The main function that finds shortest distances from src
    # to all other vertices using Bellman-Ford algorithm. The
    # function also detects negative weight cycle
    def BellmanFord(self, src):
        # Step 1: Initialize distances from src to all other vertices
        # as INFINITE
        dist = [float("Inf")] * self.V
        dist[src] = 0

        # Step 2: Relax all edges |V| - 1 times. A simple shortest
        # path from src to any other vertex can have at-most |V| - 1
        # edges
        for _ in range(self.V - 1):
            # Update dist value and parent index of the adjacent vertices of
            # the picked vertex. Consider only those vertices which are still in
            # queue
            for u, v, w in self.graph:
                if dist[u] != float("Inf") and dist[u] + w < dist[v]:
                    dist[v] = dist[u] + w

        # Step 3: check for negative-weight cycles. The above step
        # guarantees shortest distances if graph doesn't contain
        # negative weight cycle. If we get a shorter path, then there
        # is a cycle.
        for u, v, w in self.graph:
            if dist[u] != float("Inf") and dist[u] + w < dist[v]:
                print("Graph contains negative weight cycle")
                return

        print("Graph doesn't contain negative weight cycle")

        # Find the minimum cycle length
        min_cycle_length = float("Inf")
        for u, v, w in self.graph:
            if dist[u] != float("Inf") and dist[u] + w < min_cycle_length:
                min_cycle_length = dist[u] + w

        print("Minimum cycle length:", min_cycle_length)


# Driver program to test above functions
g = Graph()
g.addEdge(0, 1, 1)
g.addEdge(1, 2, -1)
g.addEdge(2, 3, -1)
g.addEdge(3, 0, -1)

# Print the solution
g.BellmanFord(0)







