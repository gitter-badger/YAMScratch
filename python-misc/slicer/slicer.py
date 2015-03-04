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

class Face(object):
	def __init__(self,normal,id):
		self.id = id
		self.edges = set()
		#order matters
		#ordered in clockwise order
		self.vertices = []
		self.normal = normal

class Edge(object):
	def __init__(self):
		self.faces = set()
		self.vertices = set()
		self.intersections = set()

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
	return tuple(smallest,largest)


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

	buff = f.read(4)
	num_faces = struct.unpack_from("<i",buff)[0]
	print num_faces

	buff = f.read(50)
	face_index = 0
	vert_index = 0
	while(buff):
		face = struct.unpack_from("<ffffffffffffH",buff)
		normal = tuple([face[i] for i in range(0,3)])
		#create a face with a normal
		temp_face = Face(normal, face_index)
		m.faces[face_index] = temp_face
		
		#now get each vertex for the face
		vert1 = tuple([face[i] for i in range(3,6)])
		vert2 = tuple([face[i] for i in range(6,9)])
		vert3 = tuple([face[i] for i in range(9,12)])

		all_verts = (vert1, vert2, vert3)
		#determine the numbering of the vertex
		for candidate_vert in all_verts:
			if candidate_vert in vert_view:
				#the vertex is in the keys
				this_vert_key = m.vertices[candidate_vert][0]
				#add this face to the vertex
				m.vertices[candidate_vert][1].faces.add(face_index)
				#add the key to the face
				temp_face.vertices.append(this_vert_key)
			else:
				#create a new vertex object
				v = Vertex(candidate_vert,vert_index)
				m.vertices[candidate_vert] = tuple([vert_index, v])
				temp_face.vertices.append(vert_index)
				vert_index += 1

		face_index += 1

		buff = f.read(50)
	#=============================================
	#We are done parsing the file
	f.close()
	#=============================================
	#now go and swap the key value pairs in the mesh.vertices dict
	for old_key in m.vertices.keys():
		#unpacke the tuple object
		(new_key, Vert_object) = m.vertices[old_key]
		#make sure that the old position is the same
		assert(Vert_object.coord == old_key)
		m.vertices[new_key] = Vert_object
		del m.vertices[old_key]



	print face_index
	print vert_index
	print '---------------------'
	for value in m.vertices.values():
		print value.faces
		print value.edges
		print '\n'
	print '---------------------'
	print m.edges
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

	# dwg = ezdxf.new("AC1015")
	# msp = dwg.modelspace()

	# points = [(0,0), (3,0), (6,3), (6,6)]
	# msp.add_line((0,0), (1,1))
	# msp.add_lwpolyline(points)

	# dwg.saveas("test.dxf")