'''
Command line based slicer. Outputs DXFs from STLs

Copyright (c) 2015 Isaiah Bell all rights reserved
'''

import sys
import os
import argparse
import re

import ezdxf
import VectorMath
import smi
from smi import SimpleMesh
from smi import Face, Edge, Vertex
from smi import SingleLayer
from guillotine_node import GuillotineNode

class LayerHolder(object):
    def __init__(self):
        self.layers = []

    def addLayer(self, layer_obj):
        self.layers.append(layer_obj)

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
        #print norm_vec
        #print VectorMath.magnitude(norm_vec)
        (basis, rot_quaternion) = VectorMath.generate_basis(norm_vec)
        mesh = smi.parsers.parseBinarySTL(fn, quaternion = rot_quaternion, scale = global_scale )
    else:
        mesh = smi.parsers.parseBinarySTL(fn)

    print "Min",mesh.min_coord
    print "Max",mesh.max_coord

    all_dwg = ezdxf.new("AC1015")
    all_msp = all_dwg.modelspace()
    all_layer_name = out_name + "_all.dxf"

    layer_index = 0
    for layer in smi.IntersectionGenerator(mesh, layer_height):
        if not args.packed:
            this_layer_name = out_name + "_" + str(layer_index) + ".dxf"
            this_layer_path = os.path.join(out_dir,this_layer_name)
            print this_layer_path
            #open the new DXF
            dwg = ezdxf.new("AC1015")
            msp = dwg.modelspace()
            for segment in layer.lines:
                point_a = list(segment[0])    
                point_b = list(segment[1])
                msp.add_line(point_a, point_b)
                all_msp.add_line(point_a, point_b)
            #now save the filename out
            dwg.saveas(this_layer_path)
        else:
            layer_holder.addLayer(layer)

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
        print root_node
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
            print layer.width
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
