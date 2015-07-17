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
	#count the number of bits total
	N = len(line)
	K_bits = [ int(digit) for digit in line]
	# reverse the bit order of the line to match endianess
	line = line[::-1]
	K = int(line, 2)
	_masks = []
	unique = set()
	for _ in range(0, N):
		try:
			line = raw_input()
		except EOFError:
			break;
		if ':' in line:
			_masks.append(MaskData(line))
			unique.add(_masks[-1])
	#don't compute repeated masks
	masks = [m for m in unique]
	M = len(unique)
	solutions = []
	#brute force
	for ii in range(0, 2**M):
		tmp = 0;
		for jj in range(0, M):
			if (1<<jj & ii) != 0:
				tmp ^= masks[jj].value
		if tmp == K:
			one_sol = [masks[jj].key for jj in range(0, M) if (1<<jj & ii) != 0]
			solutions.append(one_sol)
			del one_sol

	for sol in solutions:
		print " ".join([str(x) for x in sorted(sol)])
					


if __name__ == '__main__':
	main()
