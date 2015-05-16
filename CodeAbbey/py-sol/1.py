import sys

def main():
	if len(sys.argv) !=3:
		print "Incorrect number of input arguments"
		sys.exit(-1)
	a = int(sys.argv[1])
	b = int(sys.argv[2])
	print a+b


if __name__ == '__main__':
	main()