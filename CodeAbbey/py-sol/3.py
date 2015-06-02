import sys

def main():
	N = int(raw_input())
	result = []
	for _ in range(0,N):
		instr = raw_input().strip('\n').split(" ")
		if len(instr) != 2:
			print "input is not in pairs"
			sys.exit()
		tokens = [int(x) for x in instr]
		result.append(str(sum(tokens)))
	print " ".join(result)

if __name__ == '__main__':
	main()