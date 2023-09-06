#Auhor: Jon Jafarnia
#Date: 11/15/2021
#Purpose: Load the graph by writing the load_graph function. Create all of the vertices.

from vertex_ec import Vertex

def parse_line(line):
    section_split = line.split(";")
    vertex_name = section_split[0].strip()

    adjacent_vertices = section_split[1].split(",")

    location = section_split[2].strip().split(",")

    x = location[0]
    y = location[1]


    # add all except empty strings
    adjacent = []
    for a in adjacent_vertices:
        if a:
            adjacent.append(a.strip())

    #print(adjacent)
    return vertex_name, adjacent, x, y


def load_graph(filename):

    vertex_dict = {}

    # Read the lines in the file into a list of lines:
    file = open(filename, "r")

    for l in file:

        # if this is a line in the correct format:
        vertex_name, adjacent_vertices, x, y = parse_line(l)

            #print("vertex " + vertex_name)
            #print(" adjacent:  " + str(adjacent_vertices))
            #print(" text:  " + text)

        # YOU WRITE THIS PART
        # create a graph vertex here and add it to the dictionary
        vertex_dict[vertex_name] = Vertex(vertex_name, x, y, [])
    file.close()

    file2 = open(filename, "r")
    for line in file2:
        vertex_name, adjacent_vertices, x, y = parse_line(line)
        adjacent_vertices_objects = []
        for a in adjacent_vertices:
            a = a.strip()
            adjacent_vertices_objects.append(vertex_dict[a])
            vertex_dict[vertex_name].adjacent_vertices_list = adjacent_vertices_objects

    file2.close()

    return vertex_dict

vertex_dict = load_graph("dartmouth_graph")
