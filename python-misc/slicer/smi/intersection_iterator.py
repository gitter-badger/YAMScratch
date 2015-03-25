from single_layer import SingleLayer

def lineInterpolate(vec1, vec2, z_value):
    assert(len(vec1) == 3)
    assert(len(vec2) == 3)
    t = float(z_value - vec1[2]) / float(vec2[2] - vec1[2])
    assert(t >= 0.0)
    point_x = vec1[0] + t * (vec2[0] - vec1[0])
    point_y = vec1[1] + t * (vec2[1] - vec1[1])
    #we only need the x and y component
    return (point_x, point_y)

def IntersectionGenerator(mesh,layer_height):
    z_offset = 0.5*layer_height
    #we assume we are always slicing along positive z axis
    #start at an offset of half layer height and move until out of shape
    
    layer_index = 0
    section_criteria = mesh.min_coord[2] + z_offset
    while section_criteria < mesh.max_coord[2]:
        #begin bad algorithm
        #iterate over every face
        #and store the points in a layer object
        this_single_layer = SingleLayer(3)
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

                #this_single_layer.addLineSegment(point_a, point_b)

                three_a = (point_a[0], point_a[1], section_criteria)
                three_b = (point_b[0], point_b[1], section_criteria)
                this_single_layer.addLineSegment(three_a, three_b)
                loop_count += 1

        #print loop_count
        section_criteria += layer_height
        layer_index += 1

        yield this_single_layer
