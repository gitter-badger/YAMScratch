import math
import copy


def make_range(current_index,results,ultimate_size,solutions):
	if current_index == -1:
		#we abort the simulation
		run_sim(results,ultimate_size,solutions)
	else: #we have more things to do
		for i in range(0,ultimate_size**2):
			if i in results[current_index:len(results)]:
				pass #do not need the value if the 
			else:
				results[current_index] = i
				make_range(current_index-1,results,ultimate_size,solutions)

def run_sim(indexes,ultimate_size,solutions,pretty = False):

	new_board = [['_' for x in range(0,ultimate_size)] for x in range(0,ultimate_size)]
	for place in indexes:
		new_board[place/ultimate_size][place%ultimate_size] = str(0)
	del place

	#for row in new_board:
	#	print " ".join(row)

	current_round = copy.deepcopy(indexes)
	last_round = set() #store the new indices
	total_positions = set(copy.deepcopy(indexes)) #store all positions
	iteration_count = 0
	square_size = ultimate_size**2
	#coordinates ((column offset,row_offset),(possible squares)
	locations = [
				[[0,2],[[0,1]]],
				[[0,-2],[[0,-1]]],
				[[-2,0],[[-1,0]]],
				[[2,0],[[1,0]]],
				[[1,1],[[1,0],[0,1]]],
				[[1,-1],[[1,0],[0,-1]]],
				[[-1,1],[[-1,0],[0,1]]],
				[[-1,-1],[[-1,0],[0,-1]]]
				] #the offsets we will check


	while iteration_count<5: #run the sim
		iteration_count += 1
		#print"new round",total_positions,"current round",current_round
		for index in current_round:
			#print "New index:",index
			#the coordinates of index
			y = index/ultimate_size
			x = index%ultimate_size

			for offset in locations:
				pass_flag = True
				y_off = offset[0][1]
				x_off = offset[0][0]
				#check the offset only if condtions are right
				#if the offset y is above the index function
				if ultimate_size-1 < (y+y_off) or y+y_off < 0:
					#then the off set range is too high or too low
					pass_flag = False
				if ultimate_size-1 < x+x_off or x+x_off < 0:
					# then the offset range is to the left or the right
					pass_flag = False
				if pass_flag:
					total_offset = x_off+(y_off*ultimate_size)
					if (index+total_offset) in total_positions: #if the index is on the board
						for candidate in offset[1]:
							candi_total = candidate[0]+candidate[1]*ultimate_size
							if 0 <= candi_total+index < square_size:
								if candi_total+index in total_positions:
									pass
								else:
									last_round.add(candi_total+index) # add the index to the list
									total_positions.add(candi_total+index) #add the new idex to the total list
									new_board[(candi_total+index)/ultimate_size][(candi_total+index)%ultimate_size] = str(iteration_count)
		#update the new indices to check
		if len(total_positions) == square_size:
			print "found one"
			solutions.append(copy.deepcopy(indexes))
			for row in new_board:
				print " ".join(row)
			break
		if len(last_round) == 0:
			#print "no further candidates"
			break
		if pretty:
			for row in new_board:
				print " ".join(row)

		current_round = copy.deepcopy(last_round)
		last_round = set()
		

					



				


	'''
	new_board = copy.deepcopy(board)
	print '**************NEW*************'

	print indexes
	for index in indexes:
		new_board[index/ultimate_size][index%ultimate_size] = '1'
	for row in new_board:
		print " ".join(row)
'''

result_set = set()


if __name__ == "__main__":
	n = int(raw_input("Enter the size:"))
	k = int(raw_input("Enter the k value:"))
	############MAKE BOARD################
	board = [['0' for x in range(0,n)] for x in range(0,n)]
	for j in board:
		print " ".join(j)
	######################################

	solutions = []

	for num in range(1,k):
		targets = [None for x in range(0,num+1)] # setup the results list
		make_range(num,targets,n,solutions)

	print solutions
'''
	solutions = []
	foo = [0,5,10,15]
	n =4
	board = [['_' for x in range(0,n)] for x in range(0,n)]
	run_sim(foo,n,solutions, True)
'''
