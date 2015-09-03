import sys
import os
import time
import re
import itertools
import string
import struct
import urllib
import urllib2
import requests
from bs4 import BeautifulSoup
import gzip
import pickle
import sqlite3
from StringIO import StringIO

filename = "Matrix-Multipliation_InputForSubmission_1.txt"

import scipy
import numpy


with open(filename, "r") as f:
	all_lines = []
	for line in f:
		all_lines.append(line)

	glob_starts = []
	mat_starts = set()
	for line in all_lines:
		tmp_starts  = [m.start() for m in re.finditer('\|', line)]
		glob_starts.append(tmp_starts)
		for start in tmp_starts:
			mat_starts.add(start)

	#create the dict of each matrix start
	tmp_starts = [x for x in mat_starts]
	tmp_starts.sort()
	#print tmp_starts

	actual_mats = {}
	indx_mats_DONTCHANGE = []
	for ii in xrange(0, len(tmp_starts), 2):
		#print tmp_starts[ii]
		indx_mats_DONTCHANGE.append(tmp_starts[ii])
		actual_mats[tmp_starts[ii]] = [0,0]

	indx_mats_DONTCHANGE.sort()
	# print "==================="
	# print indx_mats_DONTCHANGE
	# print "===================="

	for indx, thing in enumerate(glob_starts):
		for indx2, real_start in enumerate(thing):
			if real_start in actual_mats.keys():
			 	actual_mats[real_start][0] +=1
			 	#hacky but we dont care
			 	tmp_slice = all_lines[indx][real_start+1:thing[indx2+1]]
			 	#print tmp_slice
			 	tmp_slice = tmp_slice.split()
			 	#print tmp_slice
			 	actual_mats[real_start][1] = len(tmp_slice)

	print actual_mats

	better_mats = []
	for key in indx_mats_DONTCHANGE:
		y_sz = actual_mats[key][0]
		x_sz = actual_mats[key][0]
		#print x_sz, y_sz
		better_mats.append( [ [0 for k in xrange(0,x_sz)] for x in xrange(0,y_sz)])


	curs_indx = [0 for x in xrange(0, len(better_mats))]

	for indx, thing in enumerate(glob_starts):
		for indx2, real_start in enumerate(thing):
			if real_start in actual_mats.keys():
			 	#hacky but we dont care
			 	tmp_slice = all_lines[indx][real_start+1:thing[indx2+1]]
			 	#print tmp_slice
			 	tmp_slice = tmp_slice.split()
			 	tmp_slice = [int(x) for x in tmp_slice]
			 	tmp_MAT_INDX = indx_mats_DONTCHANGE.index(real_start)
			 	#print "inserting",tmp_MAT_INDX
			 	tmp_row = curs_indx[tmp_MAT_INDX]
			 	try:
			 		better_mats[tmp_MAT_INDX][tmp_row] = tmp_slice
			 	except Exception:
			 		print tmp_row, tmp_MAT_INDX
			 		print tmp_slice
			 	curs_indx[tmp_MAT_INDX] += 1
			 	print tmp_slice


	a = better_mats[0]

	for mat in better_mats[1:]:
		b = numpy.array(mat)
		#print b
		c = numpy.dot(a,b)
		#print c
		a = c

	for row in a:
		sys.stdout.write( "| ")
		for elm in row:
			sys.stdout.write(str(elm) + " ")
		sys.stdout.write( "|\n")
	# for line in all_lines:
	# 	tmp_starts  = [m.start() for m in re.finditer('\|', line)]
	# 	for start in tmp_starts:
	# 		if start in actual_mats.


	


