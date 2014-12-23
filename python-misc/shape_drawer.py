import sys
import os

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

	board = [ [' ' for x in range(0,x0+radius+1)] for x in range(0,y0+radius+1)]

	print_board(board)

def print_board(board):
	print '+'+'-'*len(board[0])+'+'
	for index,row in enumerate(board):
		print '|'+''.join(row)+'|'+str(index)
	print '+'+'-'*len(board[0])+'+'
	print ' '+''.join([str(x%10) for x in range(0,len(board[0]))])

#draw_line(1,3,20,20)
draw_circle(12,3,3)