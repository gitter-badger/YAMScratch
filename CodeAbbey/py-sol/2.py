import sys

def main():
	N = raw_input()
	tokens = raw_input().strip('\n')
	delimeter = " "
	tokens = tokens.split(delimeter)
	tokens = [int(x) for x in tokens]
	print sum(tokens)

if __name__ == '__main__':
	main()