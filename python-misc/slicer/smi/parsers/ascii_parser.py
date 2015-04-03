
import smi
import VectorMath

def parseTextSTL(filename, quaternion = None, scale = 1):
	for i in range(0,3):
		try:
			f = open(filename,'r')
			break
		except IOError as e:
			print e
			exit(1)
	m = smi.SimpleMesh()
	vert_view = m.vertices.viewkeys()
	line = f.next()
	print line

	print "="*50
	for line in f:
		print line
	return m