def main():
	N = int(raw_input(""))
	result = []
	for _ in range(0,N):
		tmp = [int(x) for x in raw_input("").strip('\n').split(" ") ]
		assert(len(tmp) == 3)
		result.append( str((((tmp[2]**2 - tmp[2])//2) * tmp[1]) + tmp[2] * tmp[0] ))

	print " ".join(result)

if __name__ == '__main__':
	main()