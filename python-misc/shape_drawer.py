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
def draw_ellipse(da,db):
	'''
		(x/a)^2 + (y/b)^2 = 1

		a = da/2
		b = db/2

	'''

	board = [ ['_' for x in range(0,da)] for x in range(0,db)]

	wb = db-1
	wa = da-1
	board[0][wa>>1] = '1'
	board[wb][wa>>1] = '2'
	board[0][wa-(wa>>1)] = '3'
	board[wb][wa-(wa>>1)] = '4'

	board[wb>>1][0] = '5'
	board[wb-(wb>>1)][wa] = '6'
	board[wb>>1][wa] = '7'
	board[wb-(wb>>1)][0] = '8'

	for x in range(wa-(wa>>1),da):
		print x+1
	print '#'*50
	for y in range(wb-(wb>>1),db):
		print y+1

	print_board(board)

def simple_ellipse(a,b):
	x = a
	y = 0
	x_square = x*x
	y_square = y*y
	a_sq = float(a*a)
	b_sq = float(b*b)

	x_square_next = x_square - 4*x +4
	y_square_next = y_square + 4*y +4

	'''
		(x^2/a^2)^2-((x-2)^2/a^2)^2 + 2*((x^2*(y+2)^2)/(a^2*b^2)) -2*(((x-2)^2*y^2)/(a^2*b^2)) - 2*x^2/a^2 + 2*(x-2)^2/a^2 - 2*(y+2)^2/b^2 + 2*y^2/b^2
	'''

	
	while(x>0):
		print x,y
		left = (x_square/a_sq + y_square_next/b_sq -1)
		right = (x_square_next/a_sq +y_square/b_sq -1)
		#print "left:",left
		#print "right:",right
		left = math.pow(left,2)
		right = math.pow(right,2)
		print "left:",left
		print "right:",right


		if left -right < 0:
			#then incrementing y gives smaller error
			y_square = y_square_next
			y_square_next = y_square + 4*y +4
			y += 2
		else:
			x_square = x_square_next
			x_square_next = x_square - 4*x +4
			x -=2
		print "#"*50


	#error		

def print_board(board):
	print '+'+'-'*len(board[0])+'+'
	for index,row in enumerate(board):
		print '|'+''.join(row)+'|'+str(index+1)
	print '+'+'-'*len(board[0])+'+'
	print ' '+''.join([str(x%10) for x in range(1,len(board[0])+1)])

#draw_line(1,3,20,20)
#draw_circle(16,0,0)
#draw_ellipse(20,20)
simple_ellipse(10,10)