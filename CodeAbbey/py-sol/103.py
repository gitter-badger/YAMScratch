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

def count_set_bits(iterable, N):
	count = [0 for x in range(0,N)]
	#used for reduceing list of MaskData bit counts
	for mask in iterable:
		for index in mask.bits:
			count[index] += 1
	return count

def input_closure(K_bits):
	#takes a bit array of the value K we want to find
	#subset that XORs to the same value
	def inner_closure(bit_counts):
		def take_away_possible(mask):
			for bit_index in mask.bits:
				if K_bits[bit_index] == 1 and bit_counts[bit_index] == 1:
						#we cannot take away any more bits
						return False
			return True	
		return take_away_possible
	return inner_closure

def remove_one(K, gen, bit_counts, S):
	#test if we have found it
	tmp = 0;
	for mask in S:
		tmp ^= mask.value
	if tmp == K:
		print "found solution"
		for thing in S:
			print thing.key
		return None

	#left masks is a set, bit counts
	take_away = gen(bit_counts)
	#get list of all candidates for removal
	possible = filter(take_away, S)
	#stop if no more can take away
	if possible == []:
		return None

	for mask in possible:
		#take out the bits from bit count
		for index in mask.bits:
			bit_counts[index] -= 1
		t = set()
		t.add(mask)
		remove_one(K, gen, bit_counts, S.difference(t))
		#add them back in
		for index in mask.bits:
			bit_counts[index] += 1



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
	print "Saved:", len(_masks) - len(unique)

	gen = input_closure(K_bits)
	bit_counts = count_set_bits(unique, N)
	#recursive call

	remove_one(K, gen, bit_counts, unique)


if __name__ == '__main__':
	main()
