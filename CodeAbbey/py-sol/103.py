#! /usr/bin/python
class MaskData(object):
	def __init__(self, strn):
		tmp = strn.split(':')
		#there should only be one ':' per line
		assert(len(tmp) == 2)
		self.key = int(tmp[0]);
		self.value = 0;
		#isolate the second part
		tmp = [int(x) for x in tmp[1].split() if x != '']
		self.bits = [];
		for digit in tmp:
			self.value |= 1<<digit
			self.bits.append(digit)


	#here we promise to immutable
	def __hash__(self):
		return self.value

	def __eq__(self, other):
		return self.value == other.value

	def __repr__(self):
		return "V:"+ str(self.value) + "\nB: " + str(self.bits)


def main():
	#remove all white space from line
	line = ''.join(raw_input().split())
	print line
	K = int(line, 2)
	K_bits = [ int(digit) for digit in line]

	_masks = []
	while True:
		try:
			line = raw_input()
		except EOFError:
			break;
		if ':' in line:
			_masks.append(MaskData(line))

	for m in _masks:
		print m


if __name__ == '__main__':
	main()