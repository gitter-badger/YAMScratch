

def batman():
    print "batman"

class Mesh(object):
    def __init__(self):
        print "hi from Mesh"

    def hello(self):
        print "hi"



class Face(object):
    def __init__(self,normal,id, attr = None):
        self.id = id
        self.edges = set()
        #order matters
        #ordered in clockwise order
        self.vertices = []
        self.normal = normal
        self.attribute_code = attr

class Edge(object):
    def __init__(self,vert1, vert2, face):
        self.faces = set()
        self.faces.add(face)
        #this will be ordered according to makeEdgeKey
        self.vertices = [vert1, vert2]
        #the keys will be the layer ids
        self.intersections = {}
        
    def makeEdge(self):
        pass

class Vertex(object):
    def __init__(self,coord, id):
        self.id = id
        self.coord = coord
        self.faces = set()
        self.edges = set()

