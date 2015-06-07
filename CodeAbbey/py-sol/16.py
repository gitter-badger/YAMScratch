def main():
	N = int(raw_input())
	result = []
	for _ in range(0,N):
		a = [int(x) for x in raw_input().split(" ")]
		result.append( int(float(reduce(lambda x,y: x+y, a, 0)) / float(len(a)-1) + 0.5))
	print result

if __name__ == '__main__':
	main()