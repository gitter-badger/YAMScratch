

def batman():
	print "batman"

class Mesh(object):
	def __init__(self):
		print "hi from Mesh"

	def hello(self):
		print "hi"

	def makeEdgeKey(self, a, b):
	    smallest = a
	    largest = b
	    #sort by z value first
	    if a[2] > b[2]:
	        smallest = b
	        largest = a
	    elif a[2] == b[2]:
	        if a[1] > b[1]:
	            smallest = b
	            largest = a
	        elif a[1] == b[1]:
	            if a[0] > b[0]:
	                smallest = b
	                largest = a
	            elif a[0] == b[0]:
	                #if they are the same then
	                return False
	    return (smallest,largest)

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

