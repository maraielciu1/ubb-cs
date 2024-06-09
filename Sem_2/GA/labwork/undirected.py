from copy import deepcopy
from random import randrange


class UndirectedGraph:
    def __init__(self, n=0, m=0):
        self._vertices = set()
        self._neighbours = dict()
        self._cost = dict()
        for i in range(n):
            self.add_vertex(i)
        # for j in range(m):
        #     vertex1 = randrange(n)
        #     vertex2 = randrange(n)
        #     while self.is_edge(vertex1, vertex2) or vertex2 == vertex1:
        #         vertex1 = randrange(n)
        #         vertex2 = randrange(n)
        #     self.add_edge(vertex1, vertex2, randrange(50))

    def vertices_iterator(self):
        """
        Returns an iterator to the set of vertices.
        """
        for vertex in self._vertices:
            yield vertex

    def neighbours_iterator(self, vertex):
        """
        Returns an iterator to the set of (outbound) neighbours of a vertex.
        """
        if not self.is_vertex(vertex):
            raise ValueError("Invalid vertex.")
        for neighbour in self._neighbours[vertex]:
            yield neighbour

    def edges_iterator(self):
        """
        Returns an iterator to the set of edges.
        """
        for key, value in self._cost.items():
            yield key[0], key[1], value

    def is_vertex(self, vertex):
        """
        Returns True if vertex belongs to the graph.
        """
        return vertex in self._vertices

    def is_edge(self, vertex1, vertex2):
        """
        Returns True if the edge from vertex1 to vertex2 belongs to the graph.
        """
        if vertex1 > vertex2:
            vertex1, vertex2 = vertex2, vertex1
        return vertex1 in self._neighbours and vertex2 in self._neighbours[vertex1]

    def count_vertices(self):
        """
        Returns the number of vertices in the graph.
        """
        return len(self._vertices)

    def count_edges(self):
        """
        Returns the number of edges in the graph.
        """
        return len(self._cost)

    def degree(self, vertex):
        """
        Returns the number of edges with the start point vertex.
        """
        if vertex not in self._neighbours:
            raise ValueError("The specified vertex does not exist.")
        return len(self._neighbours[vertex])

    def get_edge_cost(self, vertex1, vertex2):
        """
        Returns the cost of an edge if it exists.
        """
        if vertex1 > vertex2:
            vertex1, vertex2 = vertex2, vertex1
        if (vertex1, vertex2) not in self._cost:
            raise ValueError("The specified edge does not exist.")
        return self._cost[(vertex1, vertex2)]

    def set_edge_cost(self, vertex1, vertex2, new_cost):
        """
        Sets the cost of an edge in the graph if it exists.
        """
        if vertex1 > vertex2:
            vertex1, vertex2 = vertex2, vertex1
        if (vertex1, vertex2) not in self._cost:
            raise ValueError("The specified edge does not exist.")
        self._cost[(vertex1, vertex2)] = new_cost

    def add_vertex(self, vertex):
        """
        Adds a vertex to the graph.
        """
        if self.is_vertex(vertex):
            raise ValueError("Cannot add a vertex which already exists.")
        self._vertices.add(vertex)
        self._neighbours[vertex] = set()

    def add_edge(self, vertex1, vertex2, edge_cost=0):
        """
        Adds an edge to the graph.
        """
        if vertex1 > vertex2:
            vertex1, vertex2 = vertex2, vertex1
        if self.is_edge(vertex1, vertex2):
            raise ValueError("The specified edge already exists")
        if not self.is_vertex(vertex1) or not self.is_vertex(vertex2):
            raise ValueError("The vertices on the edge do not exist.")
        self._neighbours[vertex1].add(vertex2)
        self._neighbours[vertex2].add(vertex1)
        self._cost[(vertex1, vertex2)] = edge_cost

    def remove_edge(self, vertex1, vertex2):
        """
        Removes an edge from the graph.
        """
        if vertex1 > vertex2:
            vertex1, vertex2 = vertex2, vertex1
        if not self.is_edge(vertex1, vertex2):
            raise ValueError("The specified edge does not exist.")
        del self._cost[(vertex1, vertex2)]
        self._neighbours[vertex1].remove(vertex2)
        self._neighbours[vertex2].remove(vertex1)

    def remove_vertex(self, vertex):
        """
        Removes a vertex from the graph.
        """
        if not self.is_vertex(vertex):
            raise ValueError("Cannot remove a vertex which doesn't exist.")
        to_remove = []
        for node in self._neighbours[vertex]:
            to_remove.append(node)
        for node in to_remove:
            self.remove_edge(vertex, node)
        del self._neighbours[vertex]
        self._vertices.remove(vertex)

    def copy(self):
        """
        Returns a deepcopy of the graph.
        """
        return deepcopy(self)

    def find_hamiltonian_cycle(self):
        """
        Finds a Hamiltonian cycle in the graph if it exists.
        Returns a list of vertices representing the cycle, or None if no cycle exists.
        """
        path = []
        start_vertex = next(self.vertices_iterator(), None)
        if start_vertex is None:
            return None

        def backtrack(current_vertex):
            if len(path) == self.count_vertices():
                if start_vertex in self._neighbours[current_vertex]:
                    path.append(start_vertex)
                    return True
                return False

            for neighbour in self._neighbours[current_vertex]:
                if neighbour not in path:
                    path.append(neighbour)
                    if backtrack(neighbour):
                        return True
                    path.pop()
            return False

        path.append(start_vertex)
        if backtrack(start_vertex):
            return path
        return None



graph = UndirectedGraph(5, 10)
graph.add_edge(0, 1, 10)
graph.add_edge(1, 2, 20)
graph.add_edge(2, 3, 30)
graph.add_edge(3, 4, 40)
graph.add_edge(4, 0, 50)
graph.add_edge(0, 2, 60)
graph.add_edge(1, 3, 70)
print("Vertices:", list(graph.vertices_iterator()))
print("Edges:", list(graph.edges_iterator()))
cycle = graph.find_hamiltonian_cycle()
if cycle:
    print("Hamiltonian Cycle found:", cycle)
else:
    print("No Hamiltonian Cycle exists.")
