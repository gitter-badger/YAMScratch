import math
import copy


def make_range(current_index,results,ultimate_size):
	if current_index == -1:
		#we abort the simulation
		run_sim(results,ultimate_size)
	else: #we have more things to do
		for i in range(0,ultimate_size**2):
			if i in results[current_index:len(results)]:
				pass #do not need the value if the 
			else:
				results[current_index] = i
				make_range(current_index-1,results,ultimate_size)

def run_sim(indexes,ultimate_size):

	current_round = copy.deepcopy(indexes)
	last_round = set() #store the new indices
	total_positions = set(copy.deepcopy(indexes)) #store all positions
	iteration_count = 0
	square_size = ultimate_size**2
	locations = {-2:[-1],
				2:[1],
				ultimate_size-1:[ultimate_size,-1],
				ultimate_size+1:[ultimate_size,1],
				-ultimate_size-1:[-ultimate_size,-1],
				-ultimate_size+1:[-ultimate_size,1],
				 2*ultimate_size:[ultimate_size],
				 -2*ultimate_size:[-ultimate_size]} #the offsets we will check
	new_board = copy.deepcopy(board)

	while iteration_count<5: #run the sim
		print"new round",total_positions,"current round",current_round
		for index in current_round:
			print "New index:",index
			for offset in locations.keys():
				pass_flag = True
				print "offset: ",offset, (index%ultimate_size == ultimate_size-1)
				#check all possible offsets
				if ((index/ultimate_size) == 0 ) and (offset <-2):
					#no thing can go above the board
					print "above"
					pass_flag = False
				elif ((index/ultimate_size) == (ultimate_size-1)) and (offset > 2):
					#no thing can go below the board
					print"below"
					pass_flag = False
				elif (index%ultimate_size == 0) and (offset ==ultimate_size-1 or offset == -ultimate_size-1):
					#cannot wrap past left side
					print"to left"
					pass_flag = False
				elif (index%ultimate_size == ultimate_size-1) and (offset == ultimate_size+1 or offset == -ultimate_size+1):
					#cannot wrap past right side
					print "to right"
					pass_flag = False

				if pass_flag:
					if (index+offset) in total_positions:
						print (index+offset)
						#if the index is on the board
						for candidate in locations[offset]:
							#check to make sure we are not wrapping the edges
							#there are only four locations we can put the new square in
							#so we check them all
						
							if 0 <= candidate+index < square_size:
								if candidate+index in total_positions:
									pass
								else:
									print "   here is new", candidate+index
									last_round.add(candidate+index) # add the index to the list
									total_positions.add(candidate+index) #add the new idex to the total list
		#update the new indices to check
		if len(total_positions) == square_size:
			print "found one"
			break
		if len(last_round) == 0:
			print "no further candidates"
			break 
		current_round = copy.deepcopy(last_round)
		last_round = set()
		iteration_count += 1

					



				


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
	'''
	n = int(raw_input("Enter the size:"))
	k = int(raw_input("Enter the k value:"))
	############MAKE BOARD################
	board = [['0' for x in range(0,n)] for x in range(0,n)]
	for j in board:
		print " ".join(j)
	######################################

	
	for num in range(1,k):
		targets = [None for x in range(0,num+1)] # setup the results list
		make_range(num,targets,n)
'''
	foo = [6,4,2]
	board = [['0' for x in range(0,3)] for x in range(0,3)]
	run_sim(foo,3)
