#Author: Jon Jafarnia
#Date: 11/17/2021
#Purpose: Draw map
from cs1lib import *
from load_graph_ec import load_graph
from bfs_ec import find_path

WIDTH = 1012
HEIGHT = 811
map = load_image("../Lab4/dartmouth_map.png")
dict  = load_graph("dartmouth_graph")

v1 = None
v2 = None

#Purpose: If the mouse moves then update the final location of the path
#Parameters: mx, my
def mouse_move(mx, my):
    global x, y, v2
    x = mx
    y = my
    for vertex in dict:
        #if is_mouse_pressed():
        if dict[vertex].check(x,y) == True:
            v2 = dict[vertex]



#Purpose: If mouse is pressed then set the location it is pressed at as the first vertext
#Parameters: mx, my
def mouse_press(mx, my):
    global x, y, v1
    x = mx
    y = my
    for vertex in dict:
        if dict[vertex].check(x,y) == True:
            v1 = dict[vertex]

#Purpose: draw the vertices, and the paths that connect them. If the it is one of vertices in
# the path then draw it in red
#Parameters:
def draw_vertex():
    for vertex in dict:
        for adjacent in dict[vertex].adjacent_vertices_list:
            dict[vertex].connect(adjacent,0,0,1)
    for vertex in dict:
        if dict[vertex] == v1:
            dict[vertex].draw_vertex(1,0,0)
            dict[vertex].name_vertex()
        else:
            dict[vertex].draw_vertex(0, 0, 1)

        if v1 != None and v2 != None:
            if dict[vertex] == v2:
                dict[vertex].name_vertex()
            paths_list = find_path(v1,v2)
            past_vertex = v2
            for vertex in paths_list:
                vertex.draw_vertex(1,0,0)
                vertex.connect(past_vertex,1,0,0)
                past_vertex = vertex









#Purpose: main draw function
#Parameters:
def main():
    draw_image(map,0,0)
    #connect_vertices()
    draw_vertex()

start_graphics(main,width=WIDTH,height=HEIGHT,mouse_move=mouse_move,mouse_press=mouse_press)