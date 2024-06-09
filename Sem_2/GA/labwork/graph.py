import copy
import random
import numpy as np
from collections import deque, defaultdict


def print_menu():
    print("1. get the number of vertices and edges")
    print("2. get vertices")
    print("3. there is an edge")
    print("4. in degree")
    print("5. out degree")
    print("6. outbound edges")
    print("7. inbound edges")
    print("8. endpoints of an edge")
    print("9. modify edge")
    print("10. add edge")
    print("11. remove edge")
    print("12. add vertex")
    print("13. remove vertex")
    print("14. write the graph to file")
    print("15. generate random graph")
    print("16. read the graph from file")
    print("17. print the graph")
    print("18. copy the graph")
    print("19. modify cost")
    print("0. exit")


class DirectedGraph:
    def __init__(self, nr_of_vertices=0):
        self.__outbound_list = {}
        self.__inbound_list = {}
        self.__edges = {}
        self.__n = 0
        self.__m = 0
        self.durations = {}

        for vertex in range(nr_of_vertices):
            self.__outbound_list[vertex] = []
            self.__inbound_list[vertex] = []

    """get the number of vertices"""

    def get_nr_of_vertices(self):
        # return len(self.__outbound_list)
        return self.__n

    """get the number of edges"""

    def get_nr_of_edges(self):
        # return len(self.__edges)
        return self.__m

    """parse (iterate) the set of vertices"""

    def get_vertices(self):
        return list(self.__outbound_list.keys())

    """get the in degree"""

    def get_in_degree(self, vertex):
        return len(self.__inbound_list[vertex])

    """get out degree"""

    def get_out_degree(self, vertex):
        return len(self.__outbound_list[vertex])

    """get the list of outbound neighbours"""

    def get_out_neighbors(self, vertex):
        """
        :param vertex: the vertex for which we need to find the neighbours
        :return: a list of vertices
        """
        return list(self.__outbound_list[vertex])

    def get_in_neighbors(self, vertex):
        """
        get the list of outbound neighbours
        :param vertex: the vertex for which we need to find the neighbours
        :return: a list of vertices
        """
        return list(self.__inbound_list[vertex])

    """retrieve or modify the information attached to a specified edge"""

    def get_edge_cost(self, edge_id):
        """
        :param edge_id: a tuple (x,y) that represents the start and end vertex
        :return: the cost of the edge or None if it does not exist
        """
        if edge_id in self.__edges:
            return self.__edges[edge_id]
        return None

    def set_edge_cost(self, edge_id, new_cost):
        """
        :param edge_id: a tuple (x,y) that represents the start and end vertex
        :param new_cost: the new cost that needs to be set
        :return:
        """
        if edge_id in self.__edges:
            self.__edges[edge_id] = new_cost

    """get the endpoints of an edge"""

    def get_endpoints(self, edge_id):
        """
        :param edge_id: a tuple (x,y) that represents the start and end vertex
        :return: the tuple it if exists, None otherwise
        """
        if edge_id in self.__edges:
            return edge_id
        return None

    def copy(self):
        """Create a deep copy of the graph."""
        return copy.deepcopy(self)

    """given two vertices, find out whether there is an edge from the first one to the second one"""

    def is_edge(self, source, target):
        """
        :param source: the source vertex
        :param target: the target vertex
        :return: True if the target is in the outbound list of the source (there is an edge between the 2 vertices), False otherwise
        """
        return target in self.__outbound_list[source]

    """add and remove vertex"""

    def add_vertex(self, vertex, duration):
        """
        :param duration: duration of the activity
        :param vertex: the vertex that needs to be added
        :return:
        """
        if vertex not in self.__outbound_list:
            self.__outbound_list[vertex] = []
            self.__inbound_list[vertex] = []
            self.__n += 1
            self.durations[vertex] = duration

    def remove_vertex(self, vertex):
        """
        Remove a vertex from the graph.
        :param vertex: the vertex that needs to be removed
        :return:
        """
        if vertex in self.__outbound_list:
            # Remove all outbound edges of the vertex
            for target in self.__outbound_list[vertex]:
                self.__inbound_list[target].remove(vertex)
                del self.__edges[(vertex, target)]
                self.__m -= 1

            # Remove all inbound edges to the vertex
            for source in self.__inbound_list[vertex]:
                self.__outbound_list[source].remove(vertex)
                del self.__edges[(source, vertex)]
                self.__m -= 1

            # Finally, remove the vertex from the graph
            del self.__outbound_list[vertex]
            del self.__inbound_list[vertex]
            self.__n -= 1

    """add and remove an edge"""

    def add_edge(self, source, target, cost):
        """
        :param source: the source vertex
        :param target: the target vertex
        :param cost: the cost of the edge
        :return:
        """
        if source not in self.__outbound_list or target not in self.__outbound_list:
            raise ValueError("One or both vertices do not exist in the graph")
        if not self.is_edge(source, target):
            self.__outbound_list[source].append(target)
            self.__inbound_list[target].append(source)
            self.__edges[(source, target)] = cost
            self.__m += 1

    def remove_edge(self, source, target):
        """
        :param source: the source vertex
        :param target: the target vertex
        :return:
        """
        if self.is_edge(source, target):
            self.__outbound_list[source].remove(target)
            self.__inbound_list[target].remove(source)
            del self.__edges[(source, target)]
            self.__m -= 1

    def modify_edge(self, source, target, new_cost):
        """
        Modify the cost of an edge.
        :param source: the source vertex
        :param target: the target vertex
        :param new_cost: the new cost of the edge
        :return:
        """
        if self.is_edge(source, target):
            self.__edges[(source, target)] = new_cost

    """parse (iterate) the set of outbound edges of a specified vertex"""

    def parse_outbound_edges(self, vertex):
        """
        :param vertex: the vertex we need
        :return:
        """
        result = []
        for v in self.__outbound_list[vertex]:
            result.append((vertex, v))
        return result

    """parse the set of inbound edges of a specified vertex"""

    def parse_inbound_edges(self, vertex):
        """
        :param vertex: the vertex we need
        :return:
        """
        result = []
        for v in self.__inbound_list[vertex]:
            result.append((v, vertex))
        return result

    @staticmethod
    def create_random_graph(n_vertices, n_edges):
        """Create a random graph with n_vertices vertices and n_edges edges."""
        if n_edges > n_vertices ** 2:
            raise ValueError("The number of edges is invalid")
        graph_rand = DirectedGraph()
        for vertex in range(n_vertices):
            graph_rand.add_vertex(vertex)

        for i in range(n_edges):
            source = random.randint(0, n_vertices - 1)
            target = random.randint(0, n_vertices - 1)
            while graph_rand.is_edge(source, target):
                source = random.randint(0, n_vertices - 1)
                target = random.randint(0, n_vertices - 1)
            cost = random.randint(1, 10)
            graph_rand.add_edge(source, target, cost)

        return graph_rand

    def __str__(self):
        s = f"{self.get_nr_of_vertices()} {self.get_nr_of_edges()}\n"
        for (source, target), cost in self.__edges.items():
            s = s + str(source) + " " + str(target) + " " + str(cost) + "\n"
        return s

    """write the graph to a file"""

    def write_to_file(self, file_name):
        with open(file_name, "w") as f:
            f.write(str(self))

    def clear(self):
        self.__outbound_list = {}
        self.__inbound_list = {}
        self.__edges = {}

    """read the graph from a file"""

    def read_from_file(self, file_name):
        self.clear()
        with open(file_name, "r") as f:
            lines = f.readlines()
            n, m = lines[0].split()
            if int(m) > int(n) ** 2:
                raise ValueError("This graph is not valid")
            for line in lines[1:]:
                source, target, cost = map(int, line.split())
                self.add_edge(source, target, cost)
            self.__n = int(n)
            self.__m = int(m)

    def BFS(self, source):
        """
        Does a BFS traversal of the graph.
        :param source: source vertex
        :return: a list of edges and the length
        """
        parent = {source: None}
        queue = [source]
        dist = {source: 0}
        children = {source: []}
        while queue:
            head = queue.pop(0)
            for edge in self.parse_outbound_edges(head):
                vertex = edge[1]
                if vertex not in parent:
                    queue.append(vertex)
                    parent[vertex] = head
                    dist[vertex] = dist[head] + 1
                    children[vertex] = []
        return parent, dist, children

    def shortest_path(self, source, target):
        """
        Finds the shortest path between two vertices using BFS
        :param target:
        :param source:
        :return:
        """
        parent, dist, children = self.BFS(source)
        path = []
        if target not in parent.keys():
            return None
        while target is not None:
            path.append(target)
            target = parent[target]
        return path[::-1]

    def initialize_distance_matrix(self, num_vertices):
        W = np.full((num_vertices, num_vertices), np.inf)
        F = np.full((num_vertices, num_vertices), -1)  # Use -1 to indicate no path

        for i in range(num_vertices):
            W[i][i] = 0
            F[i][i] = i

        for (source, target), cost in self.__edges.items():
            W[source][target] = cost
            F[source][target] = target  # Point directly to the target for direct paths

        return W, F

    def min_cost_walk(self, num_vertices):
        W, F = self.initialize_distance_matrix(num_vertices)
        k = 1
        while k < num_vertices:
            new_W = np.copy(W)
            new_F = np.copy(F)
            for i in range(num_vertices):
                for j in range(num_vertices):
                    for m in range(num_vertices):
                        if new_W[i][j] > W[i][m] + W[m][j]:
                            new_W[i][j] = W[i][m] + W[m][j]
                            new_F[i][j] = F[i][m]  # Set the path to go through vertex m
            W = new_W
            F = new_F
            k *= 2

        if any(W[i][i] < 0 for i in range(num_vertices)):
            return None, None  # Negative cycle detected

        return W, F

    def reconstruct_path(self, F, start, end):
        if F[start][end] == -1:
            return None  # No path exists

        path = []
        while start != end:
            path.append(start)
            start = F[start][end]
        path.append(end)
        return path

    def topological_sort(self):
        # source vertex is always before the target vertex in the final list
        in_degree = {v: len(self.__inbound_list[v]) for v in self.__outbound_list}
        zero_in_degree_queue = [v for v in self.__outbound_list if in_degree[v] == 0]
        topo_order = []

        while zero_in_degree_queue:
            vertex = zero_in_degree_queue.pop(0)
            topo_order.append(vertex)
            for neighbor in self.__outbound_list[vertex]:
                in_degree[neighbor] -= 1
                if in_degree[neighbor] == 0:
                    zero_in_degree_queue.append(neighbor)

        if len(topo_order) == len(self.__outbound_list):
            return topo_order
        else:
            raise ValueError("The graph is not a DAG")

    def compute_cost(self, start):
        parent = {start: None}
        dist = {start: 0}
        sorted_list= self.topological_sort()
        for vertex in sorted_list:
            min_cost = float('inf')
            if vertex != start:
                for x in self.__inbound_list[vertex]:
                    if min_cost > dist[vertex] + self.__edges[(vertex, x)]:
                        min_cost = dist[vertex] + self.__edges[(vertex, x)]
                        parent[vertex] = x
                dist[vertex] = min_cost
            else:
                dist[vertex] = 0
        return parent, dist

    def min_cost_path(self, source, target):
        parent, dist = self.compute_cost(source)


    def schedule_activities(self):
        topo_order = self.topological_sort()
        earliest_start = {}
        latest_start = {}
        total_time = 0

        # Calculate earliest start times
        for vertex in topo_order:
            earliest_start[vertex] = 0 if not self.__inbound_list[vertex] else max(
                earliest_start[pred] + self.durations[pred] for pred in self.__inbound_list[vertex])

        # Calculate total project time
        total_time = max(earliest_start[vertex] + self.durations[vertex] for vertex in topo_order)

        # Calculate latest start times
        for vertex in reversed(topo_order):
            if not self.__outbound_list[vertex]:
                latest_start[vertex] = total_time - self.durations[vertex]
            else:
                latest_start[vertex] = min(
                    latest_start[succ] - self.durations[vertex] for succ in self.__outbound_list[vertex])

        # Critical activities
        critical_activities = [v for v in topo_order if earliest_start[v] == latest_start[v]]

        return earliest_start, latest_start, total_time, critical_activities

    def count_paths(self, source, target):
        if source not in self.__outbound_list or target not in self.__outbound_list:
            return 0
        topo_order = self.topological_sort()
        path_counts = {vertex: 0 for vertex in topo_order}
        path_counts[source] = 1

        for vertex in topo_order:
            for neighbor in self.__outbound_list[vertex]:
                path_counts[neighbor] += path_counts[vertex]

        return path_counts[target]


def main():
    # graph = DirectedGraph()
    # graph.add_vertex(0)
    # graph.add_vertex(1)
    # graph.add_vertex(2)
    # graph.add_vertex(3)
    #
    # graph.add_edge(0, 1, 1)
    # graph.add_edge(1, 2, 2)
    # graph.add_edge(2, 3, 3)
    # graph.add_edge(3, 2, 1)
    # graph.add_edge(1, 3, 7)
    # # graph.add_edge(3,0,-7)
    #
    # print("Number of vertices:", graph.get_nr_of_vertices())
    # print("Number of edges:", graph.get_nr_of_edges())
    # start = int(input("Start vertex: "))
    # end = int(input("End vertex: "))
    # matrix, F = graph.min_cost_walk(graph.get_nr_of_vertices())
    # if matrix is None:
    #     print("Negative cost cycle detected")
    # else:
    #     print("Minimum cost walk matrix:")
    #     print(matrix)
    #     path = graph.reconstruct_path(F, start, end)
    #     print("Path from", start, "to", end, ":", path, " with the cost: ",
    #           matrix[start][end] if path else "No path found")

    # next for lab 4 2, for lab 5 4

    graph = DirectedGraph()
    graph.add_vertex(0, 3)
    graph.add_vertex(1, 2)
    graph.add_vertex(2, 1)
    graph.add_vertex(3, 4)
    graph.add_vertex(4, 2)
    graph.add_vertex(5, 2)

    graph.add_edge(0, 1, 1)
    # graph.add_edge(1,0,1)
    graph.add_edge(1, 2, 1)
    graph.add_edge(1, 3, 1)
    graph.add_edge(2, 3, 1)
    graph.add_edge(2, 4, 1)
    graph.add_edge(3, 4, 1)
    print(graph)

    try:
        e_start, l_start, total_time, critical_activities = graph.schedule_activities()
        print("Earliest Start Times:", e_start)
        print("Latest Start Times:", l_start)
        print("Total Project Time:", total_time)
        print("Critical Activities:", critical_activities)

        num_paths = graph.count_paths(0, 4)
        print(f"Number of distinct paths from 0 to 4 is {num_paths}")
    except ValueError as e:
        print(e)

#
# if __name__ == "__main__":
#     main()
