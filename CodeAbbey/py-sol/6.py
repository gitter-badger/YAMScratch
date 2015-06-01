import sys

def main():
	N = int(raw_input(""))
	result = []
	for ii in range(0,N):
		tmp = raw_input("").strip('\n').split(" ")
		if len(tmp) != 2:
			print "error in input"
			sys.exit(-1)
		result.append(int(float(tmp[0]) / float(tmp[1]) + 0.5))
		
	print " ".join([str(x) for x in result])

if __name__ == '__main__':
	main()