import sys
import os

class LetterComboTracker(object):
	#utility class for each char in a search sequence
	#used offsets and 
	def __init__(self, character = ""):
		self.char = character

	def __repr__(self):
		pretty_print = "Char: "  + self.char + ""
		return pretty_print

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
			print '''ERROR: char from search string not found in dictionary of unique chars of the search string'''
			print "Letter searched for: '{}'' | ord: {}\n".format(letter, ord(letter))
			return -1
		else:
			letter_count[char] += 1

	#there should be an offset value for each letter in the search string
	assert(len(substring) == len(letter_offsets))

	#show the available letter indices for each letter overall
	# for index,letter in enumerate(substring):
	# 	print "{} : {}".format(letter, char_index[letter][letter_offsets[index]:])


	#we prune the lists of letter indices for efficiency below
	#			NOT IMPLEMENTED

	#set up the base case for the last letter
	reverse_it = reversed(substring)
	base_letter = LetterComboTracker(reverse_it.next())
	print base_letter
	all_letters = [base_letter]
	for char in reverse_it:
		all_letters.insert(0, char)
	print all_letters


	return 0

def main():
	magic_search = "welcome to code jam"

	if len(sys.argv) == 2:
		with open(sys.argv[1], "r") as f:
			try:
				num_test_cases = int(f.readline())
			except ValueError:
				print "ERROR: could not determine number of test cases"
				print "please use a numeric, found {} instead".format(num_test_cases)
				return -1

			for case in range(0,num_test_cases):
				test_string = f.readline()
				test_string = test_string.strip("\n")
				result = substring_find(magic_search, test_string)
				if result == -1:
					print "Error in searching\n"
					continue
				print "Case {}: {}".format(case, result)

	else:
		print "Usage: <input filename>"

if __name__ == '__main__':
	main()
