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
	def __init__(self):
		#format background, color, bright

		self.CIS = '\033['
		self.FAIL = '\033[91m'
		self.ENDC = '\033[0m'

	def format_string(self, target, bright = False, background = False, color = 0, bold = False):
		#sanity check color to be in range [0, 7]
		if not (0 <= color <= 7):
			return None
		tmp = self.CIS
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
		return tmp + target + self.ENDC

class MonoSubstitution(object):
	def __init__(self):
		self.Ctext = [x for x in string.ascii_uppercase]
		self.Ptext = ['_' for x in range(0, len(string.ascii_uppercase))]
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
			self.Ptext[self.Ctext.index(character)] = plain[key].upper()

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

	def show_digraphs_substitutions(self):
		pass

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
	bc = bcolors()
	test = bc.format_string("foo", color = 2 , bright = False)
	print subs


	parser = argparse.ArgumentParser(prog = "Mono Subsitution Helper", 
	description = "assists in decodeing a mono substitution	cipher")
	parser.add_argument('cmd', choices = ['create','delete','help','quit'])
	while True:
		astr = raw_input('$: ')
		# print astr
		try:
			args = parser.parse_args(astr.split())
		except SystemExit:
			# trap argparse error message
			print 'error'
			continue
		if args.cmd in ['create', 'delete']:
			print 'doing', args.cmd
		elif args.cmd == 'help':
			parser.print_help()
		else:
			print 'done'
			break


if __name__ == '__main__':
	main()
