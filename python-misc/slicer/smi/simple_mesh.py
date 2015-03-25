import mesh

class SimpleMesh(mesh.Mesh):
    """docstring for Mesh"""
    def __init__(self):
        #print "mesh created"
        #use integer keys
        self.faces = {}
        self.edges = {}
        self.vertices = {}

        self.verts_index = 0
        self.edges_index = 0
        self.faces_index = 0
        self.vert_view = self.vertices.viewkeys()
        self.edge_view = self.edges.viewkeys()
        self.min_coord = [0,0,0]
        self.max_coord = [0,0,0]

    def addFace(self, normal, vert1, vert2, vert3, attr_code):
        #create a face with a normal
        temp_face = mesh.Face(normal, self.faces_index)
        self.faces[self.faces_index] = temp_face
        #determine the numbering of the vertex
        for c_vert in (vert1, vert2, vert3):
            if c_vert in self.vert_view:
                #the vertex is in the keys
                this_vert_key = self.vertices[c_vert][0]
                #add this face to the vertex
                self.vertices[c_vert][1].faces.add(self.faces_index)
                #add the vertex to the face
                temp_face.vertices.append(this_vert_key)
            else:
                if(c_vert[0] < self.min_coord[0]):
                    self.min_coord[0] = c_vert[0]
                elif(c_vert[0] > self.max_coord[0]):
                    self.max_coord[0] = c_vert[0]
                if(c_vert[1] < self.min_coord[1]):
                    self.min_coord[1] = c_vert[1]
                elif(c_vert[1] > self.max_coord[1]):
                    self.max_coord[1] = c_vert[1] 
                if(c_vert[2] < self.min_coord[2]):
                    self.min_coord[2] = c_vert[2]
                elif(c_vert[2] > self.max_coord[2]):
                    self.max_coord[2] = c_vert[2]

                #create a new vertex object
                v = mesh.Vertex(c_vert,self.verts_index)
                self.vertices[c_vert] = (self.verts_index, v)
                temp_face.vertices.append(self.verts_index)
                self.verts_index += 1

        #create the edge keys
        e1 = self.makeEdgeKey(vert1, vert2)
        e2 = self.makeEdgeKey(vert2, vert3)
        e3 = self.makeEdgeKey(vert1, vert3)

        for c_edge_key in (e1, e2, e3):
            if c_edge_key in self.edge_view:
                #get the edge in the keys
                this_edge_key = self.edges[c_edge_key][0]
                #add this face to the edge
                self.edges[c_edge_key][1].faces.add(self.faces_index)
                #add this key to the face
                temp_face.edges.add(this_edge_key)
                #add this edge to the two vertices
                self.vertices[c_edge_key[0]][1].edges.add(this_edge_key)
                self.vertices[c_edge_key[1]][1].edges.add(this_edge_key)
            else:
                #create new edge
                e = mesh.Edge(c_edge_key[0], c_edge_key[1], self.faces_index)
                self.edges[c_edge_key] = (self.edges_index, e)
                temp_face.edges.add(self.edges_index)
                #add this edge to the two vertices
                self.vertices[c_edge_key[0]][1].edges.add(self.edges_index)
                self.vertices[c_edge_key[1]][1].edges.add(self.edges_index)
                self.edges_index += 1

        self.faces_index += 1

    def acceptChanges(self):
        #First swap the edges key value pairs in mesh.edges dict
        #we still need vertex loop up by edge
        for old_key in self.edges.keys():
            if len(old_key) > 1:
                #unpack the tuple objects
                (new_key, Edge_object) = self.edges[old_key]
                #make sure that old edge is the same
                assert(self.makeEdgeKey(Edge_object.vertices[0], Edge_object.vertices[1]) == old_key)
                a_vert = Edge_object.vertices[0]
                b_vert = Edge_object.vertices[1]
                a_vert_key = self.vertices[a_vert][0]
                b_vert_key = self.vertices[b_vert][0]
                #clear the old vert tuples and add keys
                Edge_object.vertices[0] = a_vert_key
                Edge_object.vertices[1] = b_vert_key
                self.edges[new_key] = Edge_object
                del self.edges[old_key]
            else:
                pass
        #now go and swap the key value pairs in the mesh.vertices dict
        for old_key in self.vertices.keys():
            if len(self.vertices[old_key]) > 1:
                #unpacke the tuple object
                (new_key, Vert_object) = self.vertices[old_key]
                #make sure that the old position is the same
                assert(Vert_object.coord == old_key)
                self.vertices[new_key] = Vert_object
                del self.vertices[old_key]
            else:
                pass

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
        