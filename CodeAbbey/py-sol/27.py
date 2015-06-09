def main():
	N = int(raw_input())
	delimeter = " "
	in_array = [ int(x) for x in raw_input().split(delimeter)]
	assert(len(in_array) == N)
	pass_count = 0
	swap_count = 0
	pass_flag = True
	#do the obvious, and come back later refactor, and use lambdas and map
	while pass_flag:
		pass_flag = False
		for ii in range(0,N-1):
			if in_array[ii] > in_array[ii+1]:
				tmp = in_array[ii]
				in_array[ii] = in_array[ii+1]
				in_array[ii+1] = tmp
				swap_count += 1
				pass_flag = True
		pass_count += 1
	print pass_count, swap_count

if __name__ == '__main__':
	main()
