#! /usr/bin/python
import argparse
import sys
import string
import itertools

def roundrobin(*iterables):
	"roundrobin('ABC', 'D', 'EF') --> A D E B F C"
	# Recipe credited to George Sakkis
	pending = len(iterables)
	nexts = cycle(iter(it).__next__ for it in iterables)
	while pending:
		try:
			for next in nexts:
				yield next()
		except StopIteration:
			pending -= 1
			nexts = cycle(islice(nexts, pending))

class bcolors(object):
	CIS = '\033['
	FAIL = '\033[91m'
	ENDC = '\033[0m'

	@classmethod
	def format_string(cls, target, bright = False, background = False, color = 0, bold = False):
		#sanity check color to be in range [0, 7]
		if not (0 <= color <= 7):
			return None
		tmp = cls.CIS
		#dark colors are 30+i, light colors are 90+i
		#dark backgroands are 40+i, light backgrounds are 100+i
		base = 30
		if(bright):
			base += 60
		if(background):
			base += 10
		base += color
		#compose the opening qualifier
		tmp += str(base)
		if(bold):
			tmp += ';1'
		#close the qualifiers
		tmp += 'm'
		return tmp + target + cls.ENDC

class MonoSubstitution(object):
	def __init__(self):
		self.Ctext = [x for x in string.ascii_uppercase]
		self.Ptext = ['_' for x in range(0, len(string.ascii_uppercase))]
		self._CtP_mapping = {key: None for key in self.Ctext}
		#internal list of statistics used to generate frequency charts
		self._monograms = {'E': 529117365, 'T': 390965105, 'A': 374061888, 'O': 326627740, 
						   'I': 320410057, 'N': 313720540, 'S': 294300210, 'R': 277000841,
						   'H': 216768975, 'L': 183996130, 'D': 169330528, 'C': 138416451,
						   'U': 117295780, 'M': 110504544, 'F': 95422055, 'G': 91258980,
						   'P': 90376747, 'W': 79843664, 'Y': 75294515, 'B': 70195826, 
						   'V': 46337161, 'K': 35373464, 'J': 9613410, 'X': 8369915, 
						   'Z': 4975847, 'Q': 4550166, 'total': 4374127904}
		self.monogram_freq = {Y[0]: float(Y[1])/self._monograms['total'] for Y in self._monograms.items() if Y[0] is not 'total'} 
		#these are already sorted by frequency
		
		self._monographs_order = ['E', 'T', 'A', 'O', 'I', 'N', 'S', 'R', 'H', 'L',
								  'D', 'C', 'U', 'M', 'F', 'P', 'G', 'W', 'Y', 'B',
								  'V', 'K', 'X', 'J', 'Q', 'Z']
		self._digraphs_order = ['TH', 'HE', 'IN', 'ER', 'AN', 'RE', 'ON', 'AT', 'EN',
						 'ND', 'TI', 'ES', 'OR', 'TE', 'OF', 'ED', 'IS', 'IT',
						 'AL', 'AR', 'ST', 'TO', 'NT', 'NG', 'SE', 'HA', 'AS',
						 'OU', 'IO', 'LE', 'VE', 'CO', 'ME', 'DE', 'HI', 'RI',
						 'RO', 'IC', 'NE', 'EA', 'RA', 'CE', 'LI', 'CH', 'LL',
						 'BE', 'MA', 'SI', 'OM', 'UR']
		self._trigraphs_order = ['THE', 'AND', 'ING', 'ION', 'TIO', 'ENT', 'ATI', 'FOR',
						  'HER', 'TER', 'HAT', 'THA', 'ERE', 'ATE', 'HIS', 'CON',
						  'RES', 'VER', 'ALL', 'ONS', 'NCE', 'MEN', 'ITH', 'TED',
						  'ERS', 'PRO', 'THI', 'WIT', 'ARE', 'ESS', 'NOT', 'IVE',
						  'WAS', 'ECT', 'REA', 'COM', 'EVE', 'PER', 'INT', 'EST',
						  'STA', 'CTI', 'ICA', 'IST', 'EAR', 'AIN', 'ONE', 'OUR',
						  'ITI', 'RAT']
  		self._cipher_monographs = {}
		self._cipher_digraphs = {'JD': 37, 'DS':12}
		self._cipher_trigraphs = {}

		self._cipher_digraphs_order = ['JD', 'DS']

	def add_rule(self, rule):
		#takes a tuple mapping of chars to chars
		cipher = rule[0]
		plain = rule[1]
		#make sure subsitutions can be one to one
		if(len(cipher) != len(plain)):
			return None
		for key,character in enumerate(cipher):
			#make sure character is a letter
			assert(character in string.ascii_letters)
			character = character.upper()
			self.Ptext[self.Ctext.index(character)] = plain[key].upper()
			self._CtP_mapping[character] = plain[key].upper()

	def ciphertext():
		doc = "The ciphertext property."
		def fget(self):
			return self._ciphertext
		def fset(self, value):
			self._ciphertext = value
		def fdel(self):
			del self._ciphertext
		return locals()
	ciphertext = property(**ciphertext())

	def __repr__(self):
		return "Mappings:\nC: "+" ".join(self.Ctext)+'\nP: '+" ".join(self.Ptext)

	def show_digraphs(self, subs = False):
		tmp_digraphs = self._cipher_digraphs_order
		tmp_digraph_count = [self._cipher_digraphs[x] for x in self._cipher_digraphs_order]
		#if subs then we substitute the characters we know about
		#and form a new list for latter formatting and printing
		if(subs):
			tmp_digraphs = []
			#becuase we will be rewriting the keys
			tmp_digraph_count = []
			#colorize each digraph and translate it at same time
			for element in self._cipher_digraphs_order:
				tmp_digr = ''
				for character in element:
					if self._CtP_mapping[character] is not None:
						tmp_digr += bcolors.format_string(self._CtP_mapping[character],
										background = True, bright = True, color = 2)
					else:
						tmp_digr += character
				tmp_digraphs.append(tmp_digr)

		#now print out the digraphs
		for index,element in enumerate(tmp_digraphs):
			sys.stdout.write(element + ' ')
			sys.stdout.write(str(self._cipher_digraphs[self._cipher_digraphs_order[index]]))

			if(index < len(self._digraphs_order)):
				sys.stdout.write(' ' + self._digraphs_order[index])
			else:
				sys.stdout.write('  ')
			#now write the cipher text digraph count
			sys.stdout.write('\n')




	def show_ciphertext_substitutions(self):
		subbed = []
		unsubbed = []
		



class FreqAnalysis(object):
	def __init__(self, ciphertext = ""):
		self.ciphertext = ciphertext
		self.length = len(ciphertext)
		self.monographs = {}
		self.digraphs = {}
		self.trigraphs = {}

	def display_digraphs(self, mapping = None):
		#we can display the digraphs of the cipher text
		#as is, or we can substitute the mappings we know about
		#into the diagraphs
		if mappings != None:
			#translate each mapping into
			pass
		else:
			#only display the digraphs
			pass


def main():

	subs = MonoSubstitution()
	subs.add_rule(('js', 'te'))
	print subs
	print subs._CtP_mapping
	subs.show_digraphs()
	print
	subs.show_digraphs(subs = True)

	test = bcolors.format_string("foo", color = 2 , bright = False)
	print subs


	parser = argparse.ArgumentParser(prog = "Mono Subsitution Helper", 
	description = "assists in decodeing a mono substitution	cipher")
	#parser.add_argument('cmd', choices = ['create','delete','help','quit'])
	parser.add_argument('-decode', nargs=2, 
			help = "<A> <B> where A in ciphertext maps to B in plaintext",
			metavar = '<char>')
	parser.add_argument('-quit', action = 'store_true', default = False)
	while True:
		astr = raw_input('$ ')
		# print astr
		try:
			args = parser.parse_args(astr.split())
		except SystemExit:
			# trap argparse error message
			#print 'error'
			continue

		if args.quit:
			break
		if args.decode:
			print args.decode[0], args.decode[1]


if __name__ == '__main__':
	main()
