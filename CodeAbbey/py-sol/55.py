def main():
	#ignore the fact that we are supposed to look
	#for the word 'end' to stop reading
	words = [x for x in raw_input().split()]
	counts = {}
	for word in words:
		if word in counts:
			counts[word] += 1
		else:
			counts[word] = 1

	#find all words with a count greater than two
	frequent = [key for (key, value) in counts.items() if value > 1]
	print ' '.join(sorted(frequent))

if __name__ == '__main__':
	main()