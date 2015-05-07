import sys
import os

class CharWrapper(object):
	"""Purpose: wrap the related datastructures that stores and
		operates on a series of integers keys that represent indices.
		We provide some simple memoization features in a foolish attempt
		to prematurely optimize and to save time writing the same code

		WARNING: The code assumes only positive integer keys
			Since keys are meant represent indices, negative values are not supported
			
	"""
	def __init__(self, character, keys = []):
		#we assume that the list of integer keys is sorted in increasing order
		if keys:
			#cursory check, will not catch everything
			assert(keys[0] <= keys[-1])
		#takes a list of keys and sets the default appearance count to a null value
		self.keys = list()
		self.count = list()
		self.smallest_key = sys.maxint
		self.largest_key = 0
		for key in keys:
			self.keys.append(key)
			self.count.append(0)

			if key < self.smallest_key:
				self.smallest_key = key
			elif key > self.largest_key:
				self.largest_key = key

		#sephomore for making sure the two lists are trimmed the same

	def sumGreaterThanKey(self, key):
		#slices list of keys to count only values with a key greater than argument
		#then sums the permuation counts of all passible indices
		running_sum = 0
		#for key,value in 

		return running_sum

	def pruneLessThan(self, minimum):
		#remove all integer keys less than or equal 
		for key in self.keys:
			if key <= minimum:
				#this block would be sychronized in java
				self.keys.pop(0)
				self.count.pop(0)
				#reset the cached value once everything is changed
				if self.keys:
					self.smallest_key = self.keys[0]
				else:
					self.smallest_key = None

	def pruneGreaterThan(self, maximum):
		#remove all integer keys greater than or equal to 
		for key in reversed(self.keys):
			if key >= maximum:
				self.keys.pop()
				self.count.pop()
				if self.keys:
					self.largest_key = self.keys[-1]
				else:
					self.largest_key = None

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


	#set up the base case for the last letter

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
