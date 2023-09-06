#Author: Jon Jafarnia
#Date: 11/17/2021
#Purpose: Write the breadth-first search algorithm as a function in a separate file, bfs.py.
# map_plot.py can import this function. The breadth-first search function should take as parameters
# references to the start and goal vertices, and it should return a path of vertices connecting them.
# Represent the path by a Python list of references to Vertex objects for all vertices on the path.

from collections import deque

#Purpose: find the fastest path from point a to point b by checking the adjacent vertices
#Parameters: v1, v2
def find_path(v1, v2):
    paths = deque()
    dict = {v1: None}

    paths.append(v1)

    while len(paths) > 0:
        vertex = paths.popleft()

        if vertex == v2:
            paths_list = []

            while vertex != None:
                paths_list.append(vertex)
                vertex = dict[vertex]
            return paths_list

        else:
            for adjacent in vertex.adjacent_vertices_list:
                if adjacent not in dict:
                    paths.append(adjacent)
                    dict[adjacent] = vertex