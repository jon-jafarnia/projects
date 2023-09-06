#Author: Jon Jafarnia
#Date: 11/15/2021
#Purpose: Write the vertex class
RADIUS = 10
STROKE_WIDTH = 3

FONTSIZE = 20
SHIFT = 12

from cs1lib import *

class Vertex:
    def __init__(self, name, x, y, adjacent_vertices_list):
        self.name = name
        self.x = int(x)
        self.y = int(y)
        self.adjacent_vertices_list = adjacent_vertices_list

    #Purpose: Check if the mouse is located at a vertex
    #Parameters: gx, gy
    def check(self, gx, gy):
        if self.x - RADIUS <= gx <= self.x +RADIUS:
            if self.y - RADIUS <= gy <= self.y + RADIUS:
                return True

    # Purpose: draw the lines to connect vertices
    # Parameters: adjacent, r, g, b
    def connect(self, adjacent, r, g, b):
        set_stroke_color(r,g,b)
        set_stroke_width(STROKE_WIDTH)
        draw_line(self.x, self.y, adjacent.x, adjacent.y)

    # Purpose: draw a vertex. I chose to outline the vertices in black because it stands out more
    # Parameters: r, g, b
    def draw_vertex(self,r,g,b):
        set_stroke_color(0,0,0)
        set_stroke_width(2)
        set_fill_color(r,g,b)
        draw_circle(self.x,self.y, RADIUS)


        # Purpose: draw a vertex
        # Parameters: r, g, b
    def name_vertex(self):
        set_stroke_color(.75, 0, .75)
        set_font_size(FONTSIZE)
        draw_text(self.name, self.x + SHIFT, self.y + SHIFT)


    def __str__(self):
        adj_string = ""
        for a in self.adjacent_vertices_list:
            if a == self.adjacent_vertices_list[0]:
                adj_string += a.name
            else:
                adj_string += ", " +a.name
        return self.name +"; Location: "+ str(self.x) + ", "+str(self.y)+"; Adjacent Vertices: "+adj_string