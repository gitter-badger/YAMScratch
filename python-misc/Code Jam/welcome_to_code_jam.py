import sys
import os

def substring_find(substring, string):
	#find number of possible substrings
	unique_chars = set([x for x in substring])
	#reduce the input string to indices
	char_index = {}
	for char in unique_chars:
		temp_s = list()
		for index,letter in enumerate(string):
			if letter == char:
				temp_s.append(index)
		#add the list into the dictionary
		char_index[char] = temp_s
	#cache how many times a letter is in the substring
	letter_count = {key : 0 for key in unique_chars}
	#cache how many times the charater has appeared before it in the substring
	letter_offsets = list()
	for char in substring:
		try:
			letter_offsets.append(letter_count[char])
		except KeyError as e:
			print e
			return -1
		else:
			letter_count[char] += 1

	#there should be an offset value for each letter in the search string
	assert(len(substring) == len(letter_offsets))

	#show the available letter indices for each letter overall
	for index,letter in enumerate(substring):
		print "{} : {}".format(letter, char_index[letter][letter_offsets[index]:])
	return 0


if __name__ == '__main__':
	magic_search = "welcome to code jam"

	if len(sys.argv) == 2:
		with open(sys.argv[1], "r") as f:
			num_test_cases = int(f.readline())
			for case in range(0,num_test_cases):
				test_string = f.readline()
				test_string = test_string.strip("\n")
				result = substring_find(magic_search, test_string)
				print "Case {}: {}".format(case, result)

	else:
		print "Usage: <input filename>"