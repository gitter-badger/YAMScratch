import unittest


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
		maximum = 15
		return_code = bar.pruneGreaterThan(maximum)
		self.assertEquals(return_code, 0)
		self.assertEquals(bar.largest_key, maximum-1)

	def test_pruneLessThan(self):
		keys = [x for x in range(10,20) ]
		cat = CharWrapper("l", keys)
		minimum = 15
		return_code = cat.pruneLessThan(minimum)
		self.assertEquals(return_code, 0)
		self.assertEquals(cat.smallest_key, minimum+1)

if __name__ == '__main__':
	unittest.main()
