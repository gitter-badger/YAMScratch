'''
Command line based slicer. Outputs DXFs from STLs

Copyright (c) 2015 Isaiah Bell all rights reserved
'''

import sys
import os
import argparse
import struct
import re

import ezdxf
import VectorMath

class Mesh(object):
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
        temp_face = Face(normal, self.faces_index)
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
                v = Vertex(c_vert,self.verts_index)
                self.vertices[c_vert] = (self.verts_index, v)
                temp_face.vertices.append(self.verts_index)
                self.verts_index += 1

        #create the edge keys
        e1 = makeEdgeKey(vert1, vert2)
        e2 = makeEdgeKey(vert2, vert3)
        e3 = makeEdgeKey(vert1, vert3)

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
                e = Edge(c_edge_key[0], c_edge_key[1], self.faces_index)
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
                assert(makeEdgeKey(Edge_object.vertices[0], Edge_object.vertices[1]) == old_key)
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

class Face(object):
    def __init__(self,normal,id):
        self.id = id
        self.edges = set()
        #order matters
        #ordered in clockwise order
        self.vertices = []
        self.normal = normal

class Edge(object):
    def __init__(self,vert1, vert2, face):
        self.faces = set()
        self.faces.add(face)
        #this will be ordered according to makeEdgeKey
        self.vertices = [vert1, vert2]
        #the keys will be the layer ids
        self.intersections = {}

class Vertex(object):
    def __init__(self,coord, id):
        self.id = id
        self.coord = coord
        self.faces = set()
        self.edges = set()


class Layer(object):
    def __init__(self):
        self.points = []

def makeEdgeKey(a,b):
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

def parseBinarySTL(filename, quaternion = None, scale = 1):
    #retry 3 times
    for i in range(0,3):
        try:
            f = open(fn, "rb")
            buff = f.read(80)
            #print buff
            break
        except IOError as e:
            print e
    #create a mesh object to hold our entire system
    m = Mesh()
    vert_view = m.vertices.viewkeys()
    #find and store the number of faces
    buff = f.read(4)
    num_faces = struct.unpack_from("<i",buff)[0]
    #start reading the faces in
    buff = f.read(50)
    face_index = 0
    vert_index = 0
    while(buff):
        face = struct.unpack_from("<ffffffffffffH",buff)
        normal = tuple([face[i] for i in range(0,3)])
        #now get each vertex for the face
        vert1 = tuple([face[i] for i in range(3,6)])
        vert2 = tuple([face[i] for i in range(6,9)])
        vert3 = tuple([face[i] for i in range(9,12)])
        #now get the attribute
        attribute_code = face[12]
        #scale the objects up
        if scale:
            vert1 = tuple([vert1[i]*scale for i in range(0, len(vert1))])
            vert2 = tuple([vert2[i]*scale for i in range(0, len(vert2))])
            vert3 = tuple([vert3[i]*scale for i in range(0, len(vert3))])
        #rotate the coordinates by quaternion if present 
        if quaternion:
            normal = VectorMath.Quaternion.rotation(normal, quaternion)
            vert1 =  tuple(VectorMath.Quaternion.rotation(vert1, quaternion))
            vert2 =  tuple(VectorMath.Quaternion.rotation(vert2, quaternion))
            vert3 =  tuple(VectorMath.Quaternion.rotation(vert3, quaternion))

        m.addFace(normal, vert1, vert2, vert3, attribute_code)

        buff = f.read(50)
    #=============================================
    #We are done parsing the file
    f.close()
    #=============================================
    m.acceptChanges()
    #finish up and return the mesh
    assert(num_faces == len(m.faces))
    return m

def lineInterpolate(vec1, vec2, z_value):
    assert(len(vec1) == 3)
    assert(len(vec2) == 3)
    t = float(z_value - vec1[2]) / float(vec2[2] - vec1[2])
    assert(t >= 0.0)
    point_x = vec1[0] + t * (vec2[0] - vec1[0])
    point_y = vec1[1] + t * (vec2[1] - vec1[1])
    #we only need the x and y component
    return (point_x, point_y)

class SingleLayer(object):
    def __init__(self, dimension):
        self.dimension = dimension
        self.min_coord = [0 for x in range(0,dimension)]
        self.max_coord = [0 for x in range(0,dimension)]
        self.lines = []
        self.recompute_size_flag = True
        #hold the fit into the tree
        self.fit = None

    def addLineSegment(self, point_a, point_b):
        together = tuple([point_a, point_b])
        self.lines.append(together)
        for test_point in together:
            for index in range(0, self.dimension):
                if test_point[index] < self.min_coord[index]:
                    self.min_coord[index] = test_point[index]
                if test_point[index] > self.max_coord[index]:
                    self.max_coord[index] = test_point[index]
        #reset the flag so the method knows when to recompute
        self.recompute_size_flag = True

    def width():
        doc = "The width property."
        def fget(self):
            if self.recompute_size_flag:
                self._width = abs(self.max_coord[0] - self.min_coord[0])
                self._height = abs(self.max_coord[1] - self.min_coord[1])
                self.recompute_size_flag = False
            return self._width
        def fset(self, value):
            self._width = value
        def fdel(self):
            del self._width
        return locals()
    width = property(**width())

    def height():
        doc = "The height property."
        def fget(self):
            if self.recompute_size_flag:
                self._width = abs(self.max_coord[0] - self.min_coord[0])
                self._height = abs(self.max_coord[1] - self.min_coord[1])
                self.recompute_size_flag = False
            return self._height
        def fset(self, value):
            self._height = value
        def fdel(self):
            del self._height
        return locals()
    height = property(**height())

    def area(self):
        return self.width*self.height

class LayerHolder(object):
    def __init__(self):
        self.layers = []

    def addLayer(self, layer_obj):
        self.layers.append(layer_obj)

class GuillotineNode(object):
    def __init__(self, x0, y0, width, height):
        self.width = width
        self.height = height
        #define in global coordinates where this node is
        self.x0 = x0
        self.y0 = y0
        self.used = False
        #tree pointers
        self.up = None
        self.right = None

    def __str__(self):
        result = "x0: " + str(self.x0) + " y0: " + str(self.y0) + '\n'
        result += (str(self.width) + " " + str(self.height) + '\n')
        return result

    @classmethod
    def findNode(cls ,root, w, h):
        if(root.used):
            found_node = GuillotineNode.findNode(root.right, w, h)
            if found_node:
                return found_node
            #try the other node
            other_node = GuillotineNode.findNode(root.up, w, h)
            if other_node:
                return other_node
            #if neither node has room, then return None
            #we coerce the truth value of None a lot in this function
            else:
                return None

        elif (w <= root.width and h <= root.height):
            return root
        else:
            #return null if the node is not used but the box does not fit
            return None

    def splitNode(self, w_box, h_box):
        '''
        depiction below of splitting procedure
        +-----------------------+
        |                       |
        |         up            |
        |                       |
        +-------+---------------+   
        |       |               |
        | root  |     right     |
        +-------+---------------+
        the root node is the current node, which creates two child nodes
        when a box is inserted into the root. The root node becomes the
        same size as the box going in
        '''
        #create the two child nodes
        self.up = GuillotineNode(self.x0, (self.y0 + h_box), self.width, (self.height - h_box))
        self.right = GuillotineNode(self.x0 + w_box, self.y0, (self.width - w_box), h_box)
        self.used = True
        #we return self so the layer object can store a reference to it
        return self

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = '''Slice the input STL file 
        and output a DXF file for each and every slice. Output files are 
        named FILENAME_slice(n).dxf where (n) is the layer.''')

    parser.add_argument('filename',metavar = 'FILENAME', type=str, nargs = '+',
        help = 'Source stl file' )
    parser.add_argument('-n','--normal', help = 'Comma separated normal vector',
        action = 'store', default = '0,0,1')
    parser.add_argument('-l','--layer-height', help = 'Layer height in stl file units',
        default = 0.5)
    parser.add_argument('-s', '--scale', help = 'Scaling factor, does not effect layer height',
            default = '1')
    parser.add_argument('--packed', help = 'Packs the shapes into a sheet of size M by N',
        default = False)

    args = parser.parse_args()
    #get the normal vector
    norm_vec = tuple([int(comp) for comp in args.normal.split(",")])
    assert( len(norm_vec) == 3)
    #set default
    layer_height = float(args.layer_height)
    global_scale = float(args.scale)
    #choose to pack the objects into a size buffer
    if args.packed:
        layer_holder = LayerHolder()
        temp = tuple([float(comp) for comp in args.packed.split(",")])
        packed_width = temp[0]
        packed_height = temp[1]
        del temp
        print "Packing:",packed_width, packed_height
    #check that the file name is stl
    fn = args.filename[0]
    stl_regex = re.compile("(\.STL)|(\.stl)$")
    if(re.search(stl_regex, fn) is not None):
        out_name = re.split(stl_regex, fn)[0]
    else:
        print "Exiting: invalid filename is not a .stl or .STL"
        print "\t"+fn
        exit(1)

    #try to create a dir for output
    out_dir = "./out"
    try:
        os.mkdir(out_dir)
    except OSError:
        print "dir already exists"

    magnitude = VectorMath.magnitude(norm_vec)
    norm_vec = tuple([norm_vec[i]/magnitude for i in range(0, len(norm_vec))])

    if norm_vec != (0,0,1):

        print norm_vec
        print VectorMath.magnitude(norm_vec)

        (basis, rot_quaternion) = VectorMath.generate_basis(norm_vec)
        mesh = parseBinarySTL(fn, quaternion = rot_quaternion, scale = global_scale )
    else:
        mesh = parseBinarySTL(fn)

    print "Min",mesh.min_coord
    print "Max",mesh.max_coord
    #we assume we are always slicing along positive z axis
    #start at an offset of half layer height and move until out of shape
    z_offset = 0.5*layer_height
    layer_index = 0
    section_criteria = mesh.min_coord[2] + z_offset
    all_dwg = ezdxf.new("AC1015")
    all_msp = all_dwg.modelspace()
    all_layer_name = out_name + "_all.dxf"

    while(section_criteria < mesh.max_coord[2]):
        if not args.packed:
            this_layer_name = out_name + "_" + str(layer_index) + ".dxf"
            this_layer_path = os.path.join(out_dir,this_layer_name)
            print this_layer_path
            #open the new DXF
            dwg = ezdxf.new("AC1015")
            msp = dwg.modelspace()
        else:
            this_single_layer = SingleLayer(2)
            layer_holder.addLayer(this_single_layer)
        #begin bad algorithm
        #iterate over every face
        loop_count = 0
        for face in mesh.faces.values():
            #test each edge for intersection
            vert_intersection = False
            point_a = None
            point_b = None

            for edge_key in face.edges:
                a_vert_key = mesh.edges[edge_key].vertices[0]
                b_vert_key = mesh.edges[edge_key].vertices[1]
                a_z = mesh.vertices[a_vert_key].coord[2]
                b_z = mesh.vertices[b_vert_key].coord[2]
                #first case, we intersect a vertex and a edge
                #we will hit this case twice for two edges if so
                #the second time will just overwrite the coord
                face_inter_count = 0
                if a_z != b_z:
                    if a_z == section_criteria:
                        assert(point_a == None)
                        point_a = (mesh.vertices[a_vert_key].coord[0], mesh.vertices[a_vert_key].coord[1])
                        #if we already have found intersection, then quit
                        if point_b != None:
                            break
                        else:
                            vert_intersection = True

                    elif b_z == section_criteria:
                        assert(point_a == None)
                        point_a = (mesh.vertices[b_vert_key].coord[0], mesh.vertices[b_vert_key].coord[1])
                        #if we already have found intersection, then quit
                        if point_b != None:
                            break
                        else:
                            vert_intersection = True
                    
                    #second case, we might intersect a edge
                    if a_z > b_z:
                        if b_z < section_criteria < a_z:
                            inter_test = True
                        else:
                            inter_test = False
                    elif a_z < b_z:
                        if a_z < section_criteria < b_z:
                            inter_test = True
                        else:
                            inter_test = False

                    #now we know if the edge is an intersection
                    #this is if we have point_a and we can then calculate point_b
                    if inter_test and point_b == None:
                        point_b = lineInterpolate(mesh.vertices[a_vert_key].coord, 
                                                  mesh.vertices[b_vert_key].coord,
                                                  section_criteria)
                        if vert_intersection:
                            assert(point_a is not None)
                            break
                    #point_b is if we get here filled
                    elif inter_test:
                        point_a = lineInterpolate(mesh.vertices[a_vert_key].coord, 
                                                  mesh.vertices[b_vert_key].coord,
                                                  section_criteria)
                        assert(point_b is not None)
                        break

                else:
                    #the points are on the same z
                    if a_z == section_criteria:
                        #then return the line
                        point_a = (mesh.vertices[a_vert_key].coord[0], mesh.vertices[a_vert_key].coord[1])
                        point_b = (mesh.vertices[b_vert_key].coord[0], mesh.vertices[b_vert_key].coord[1])
                        break

            #if none of the points
            #we must have two disimilar points
            if(point_a and point_b):
                if not args.packed:
                    msp.add_line(point_a, point_b)
                else:
                    this_single_layer.addLineSegment(point_a, point_b)

                three_a = (point_a[0], point_a[1], section_criteria)
                three_b = (point_b[0], point_b[1], section_criteria)
                all_msp.add_line(three_a, three_b)
                loop_count += 1

        if not args.packed:
            dwg.saveas(this_layer_path)
        else:
            pass

        #print loop_count
        section_criteria += layer_height
        layer_index += 1

    #save the composite dxf to top level dir
    all_dwg.saveas(all_layer_name)
    #if we are packinng, do it here
    if args.packed:
        #sort the layers by total area
        layer_holder.layers.sort(key = lambda x: x.area(), reverse = True)
        #set up the first sheet
        current_sheet_count = 0
        this_sheet_name = out_name + "_sheet_" + str(current_sheet_count) + ".dxf"
        this_sheet_path = os.path.join(out_dir,this_sheet_name)

        root_node = GuillotineNode(0, 0, packed_width, packed_height)
        sheet_dwg = ezdxf.new("AC1015")
        sheet_msp = sheet_dwg.modelspace()
        #add a border of space to each layer when placing in sheet
        border_spacing = 0.05

        for layer in layer_holder.layers:
            #move on to next layer if this one is empty
            if len(layer.lines) == 0:
                continue
            #try to place the block in the root node. if we cannot
            #then we must start a new sheet
            found_node = GuillotineNode.findNode(root_node, 
                                                layer.width + border_spacing, 
                                                layer.height + border_spacing)
            if found_node == None:
                print "new sheet required"
                sheet_dwg.saveas(this_sheet_path)
                #replace the thing with a new sheet
                current_sheet_count += 1
                this_sheet_name = out_name + "_sheet_" + str(current_sheet_count) + ".dxf"
                this_sheet_path = os.path.join(out_dir,this_sheet_name)

                root_node = GuillotineNode(0, 0, packed_width, packed_height)
                #make sure that the new layer will fit into the fresh sheet
                assert(layer.width + border_spacing <= root_node.width)
                assert(layer.height + border_spacing <= root_node.height)
                sheet_dwg = ezdxf.new("AC1015")
                sheet_msp = sheet_dwg.modelspace()
                found_node = GuillotineNode.findNode(root_node, 
                                    layer.width + border_spacing, 
                                    layer.height + border_spacing)
            #now continue by plaving each layer into the sheet
            layer.fit = found_node.splitNode(layer.width + border_spacing, 
                                            layer.height + border_spacing)

            #add the layer lines to the sheet
            #first determine the x and y offsets for the coordinates
            x_offset = (layer.fit.x0 + border_spacing/2) - layer.min_coord[0]
            y_offset = (layer.fit.y0 + border_spacing/2) - layer.min_coord[1]
            #print "offset: ", x_offset, y_offset
            for segment in layer.lines:
                #translate the segment coordinates to the global coordinates
                point_a = list(segment[0])
                point_a[0] += x_offset
                point_a[1] += y_offset

                point_b = list(segment[1])
                point_b[0] += x_offset
                point_b[1] += y_offset

                sheet_msp.add_line(point_a, point_b)


        #at the end, write out the final sheet
        print "done with final sheet"
        sheet_dwg.saveas(this_sheet_path)
    # dwg = ezdxf.new("AC1015")
    # msp = dwg.modelspace()

    # points = [(0,0), (3,0), (6,3), (6,6)]
    # msp.add_line((0,0), (1,1))
    # msp.add_lwpolyline(points)

    # dwg.saveas("test.dxf")

    #must fit 30by18 rectangle
