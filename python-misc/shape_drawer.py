import sys
import os

def draw_line(x0,y0,x1,y1):
	deltax = x1-x0
	deltay = y1-y0

	board = [ [' ' for x in range(0,deltax)] for x in range(0,deltay)]
	print_board(board)


def print_board(board):
	print '+'+'-'*len(board[0])+'+'
	for row in board:
		print '|'+''.join(row)+'|'
	print '+'+'-'*len(board[0])+'+'

draw_line(0,0,35,10)