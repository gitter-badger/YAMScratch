import itertools
import sys

def main():
	N, P, G = tuple([int(x) for  x in raw_input().split()])
	#for each gangster clan
	for _ in range(0, N):
		num_gansters = int(raw_input())
		#print "Num",num_gansters
		gangsters = []
		for _ in range(0, num_gansters):
			line = raw_input()
			gangsters.append( tuple([int(x) for x in line.split()]))
		#print gangsters
		clan_solutions = []
		for L in range(1, len(gangsters)):
			for subset in itertools.combinations(gangsters, L):
				#reduce subset and check if it is a solution
				result = reduce(lambda x,y: (x[0]+y[0], x[1]+y[1]), subset)
				if(result[0] == P and result[1] == G):
					clan_solutions.append(subset)
		sizes = map(lambda x: len(x), clan_solutions)
		sys.stdout.write( str(max(sizes)) + " ")
		
if __name__ == '__main__':
	main()
