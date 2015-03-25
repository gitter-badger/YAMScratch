import struct

import smi

def parseBinarySTL(filename, quaternion = None, scale = 1):
    #retry 3 times
    for i in range(0,3):
        try:
            f = open(filename, "rb")
            buff = f.read(80)
            #print buff
            break
        except IOError as e:
            print e
    #create a mesh object to hold our entire system
    m = smi.SimpleMesh()
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