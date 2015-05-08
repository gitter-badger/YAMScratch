import unittest
import string

from welcome_to_code_jam import CharWrapper

class TestCharWrapper(unittest.TestCase):
	def setUp(self):
		self.default_wrap = CharWrapper("d")
		#use the default constructor
		self.keys = [x*4 for x in range(0,10) ]
		self.wrap1 = CharWrapper("1", self.keys)

	def test_pruneGreaterThan(self):
		keys = [x for x in range(10,20)]
		bar = CharWrapper("l", keys)
		self.assertEquals(bar.char, "l")
		maximum = 15
		return_code = bar.pruneGreaterThan(maximum)
		self.assertEquals(return_code, 0)
		self.assertEquals(bar.largest_key, maximum-1)

	def test_pruneLessThan(self):
		keys = [x for x in range(10,20) ]
		cat = CharWrapper("k", keys)
		self.assertEquals(cat.char, "k")
		minimum = 15
		return_code = cat.pruneLessThan(minimum)
		self.assertEquals(return_code, 0)
		self.assertEquals(cat.smallest_key, minimum+1)

	def test_characterSupport(self):
		#pretty useless test for now, just make sure we can handle the ascii
		#charaters we want to. in case we do anything latter with the char property
		for char in string.printable:
			temp = CharWrapper(char)
			self.assertEquals(char, temp.char)
			del temp

	def test_printing(self):
		keys = [x for x in range(10,100,11)]
		foo = CharWrapper("f", keys)
		counts = [x - 3 for x in keys]
		for index in range(0, len(foo.count)):
			foo.count[index] = counts[index]
		print "\n"
		print "Keys:  ", keys
		print "Counts:", counts
		print
		print foo


if __name__ == '__main__':
	unittest.main()
