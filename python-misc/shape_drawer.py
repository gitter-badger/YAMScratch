import sys
import os
import math

def draw_line(x0,y0,x1,y1):
	dx = x1 - x0
	dy = y1 - y0

	board = [ [' ' for x in range(0,x1+1)] for x in range(0,y1+1)]
	#begin algorithm
	board[y0][x0] = '#'

	y = y0
	D = 2*dy -dx
	for x in range(x0+1,x1+1):
		if D > 0:
			y = y+1
			D += (2*dy -2*dx)
			board[y][x] = '#'
		else:
			D += 2*dy
			board[y][x] = '#'
		

	print_board(board)

def draw_circle(radius,x0,y0):
	board = [ [' ' for x in range(0,x0+int(2*radius)+1)] for x in range(0,y0+int(2*radius)+1)]
	x0 +=radius
	y0 +=radius
	y = 1
	x = radius
	RE = 1-x
	while x>=y:
		a =raw_input(':')
		print_board(board)
		board[int(y+y0)][int(x+x0)] = '1'
		board[int(x+y0)][int(y+x0)] = '2'
		board[int(-x+y0)][int(y+x0)] = '3'
		board[int(-y+y0)][int(x+x0)] = '4'
		board[int(-x+y0)][int(-y+x0)] = '5'
		board[int(-y+y0)][int(-x+x0)] = '6'
		board[int(x+y0)][int(-y+x0)] = '7'
		board[int(y+y0)][int(-x+x0)] = '8'
		y += 1
		if RE < 0:
			RE += 2*y +1
		else:
			x -= 1
			RE += 2*(y-x+1)
	print_board(board)

''' Drawing Ellipse'''
def simple_ellipse(a,b):
	'''
		a = width of ellipse in pixels
		b = height of the ellipse in pixels
	'''
	#precompute some constants
	a2 = a*a
	b2 = b*b
	x_offset = (a+1)%2
	y_offset = (b+1)%2
	#initialize the x and y arrays
	x = a - 1
	y = y_offset
	x_list = [x]
	y_list = [y]
	#compute the first interation of x and y steps
	x2n0 = x*x
	y2n0 = y*y
	 
	while y_list[-1] < b-1:
		#recursively compute the next piece
		x2n1 = x2n0 - 4*x +4
		y2n1 = y2n0 + 4*y +4
		#calculate each step
		possible = [(math.pow(b2*x2n1 + a2*y2n0 - a2*b2,2),(x2n1,y2n0),(x-2,y)),
					(math.pow(b2*x2n0 + a2*y2n1 - a2*b2,2),(x2n0,y2n1),(x,y+2)),
					(math.pow(b2*x2n1 + a2*y2n1 - a2*b2,2),(x2n1,y2n1),(x-2,y+2))
					]
		minimum = sorted(possible,key = lambda target: target[0])[0]
		x2n0,y2n0 = minimum[1]
		x,y = minimum[2]
		x_list.append(x)
		y_list.append(y)
	#burn down the x error
	while x > 1:
		x -= 2
		x_list.append(x)
		y_list.append(y)
	#create the entire coordinate list
	all_x = []
	all_y = []
	for i in [-1,1]:
		for j in [-1,1]:
			for index,k in enumerate(x_list):
				all_x.append(i*k)
				all_y.append(j*y_list[index])
	
	index_x = []
	index_y = []
	for index,elem in enumerate(all_x):
		x_coord = (elem - x_offset + a)>>1
		y_coord = (all_y[index] - y_offset + b)>>1
		index_x.append(x_coord)
		index_y.append(y_coord)
	#this is a large return because this is for testing
	#and we use this for different things
	return (index_x,index_y,all_x,all_y)

def thick_ellipse(a,b,thickness):
	#there is a strange bug in this routine
	#found it: the way the rounding by division by two
	#keeps the center of a 1 pixel diameter samller circle
	#in the same place.
	board = [ [' ' for x in range(0,a)] for x in range(0,b)]

	for offset in [x*2 for x in range(0,thickness)]:
		print offset
		temp_a = a-offset
		temp_b = b-offset
		x_offset = (a+1)%2
		y_offset = (b+1)%2
		print temp_a,temp_b
		not_used,not_used2,x_list,y_list = simple_ellipse(temp_a,temp_b)
		for index,elem in enumerate(x_list):
			x_coord = (elem - x_offset + a)>>1
			y_coord = (y_list[index] - y_offset + b)>>1
			board[y_coord][x_coord] = '#'

	print_board(board)

def print_board(board):
	print '+'+'-'*len(board[0])+'+'
	for index,row in enumerate(board):
		print '|'+''.join(row)+'|'+str(index+1)
	print '+'+'-'*len(board[0])+'+'
	print ' '+''.join([str(x%10) for x in range(1,len(board[0])+1)])

#draw_line(1,3,20,20)
#draw_circle(16,0,0)
#draw_ellipse(20,20)
a = int(raw_input('A: ')) 
b = int(raw_input('B: '))
t = int(raw_input('T: '))
#declare board

#simple_ellipse(a,b,board)
thick_ellipse(a,b,t)