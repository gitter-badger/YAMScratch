import sys
import os
import argparse
import struct

import ezdxf

class Mesh(object):
	"""docstring for Mesh"""
	def __init__(self):
		print "mesh created"
		#use integer keys
		self.faces = {}
		self.edges = {}
		self.vertices = {}

		self.verts_index = 0
		self.edges_index = 0
		self.faces_index = 0
		self.vert_view = self.vertices.viewkeys()
		self.edge_view = self.edges.viewkeys()

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
				#add this edge to the two verticies
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

		#now swap the edges key value pairs in mesh.edges dict
		for old_key in self.edges.keys():
			if len(old_key) > 1:
				#unpack the tuple objects
				(new_key, Edge_object) = self.edges[old_key]
				#make sure that old edge is the same
				assert(makeEdgeKey(Edge_object.vertices[0], Edge_object.vertices[1]) == old_key)
				self.edges[new_key] = Edge_object
				del self.edges[old_key]
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



def parseBinarySTL(filename):
	#retry 3 times
	for i in range(0,3):
		try:
			f = open(fn, "rb")
			buff = f.read(80)
			print buff
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


if __name__ == '__main__':
	parser = argparse.ArgumentParser(description = '''Slice the input STL file 
		and output a DXF file for each and every slice.	Output files are 
		named FILENAME_slice(n).dxf where (n) is the layer.''')

	parser.add_argument('filename',metavar = 'FILENAME', type=str, nargs = '+',
		help = 'Source stl file' )

	args = parser.parse_args()
	print args.filename
	fn = args.filename[0]

	mesh = parseBinarySTL(fn)
	print mesh.verts_index
	print mesh.edges_index
	print mesh.faces_index

	# dwg = ezdxf.new("AC1015")
	# msp = dwg.modelspace()

	# points = [(0,0), (3,0), (6,3), (6,6)]
	# msp.add_line((0,0), (1,1))
	# msp.add_lwpolyline(points)

	# dwg.saveas("test.dxf")
