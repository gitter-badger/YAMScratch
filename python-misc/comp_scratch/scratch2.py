import sys
import os
import time
import re
import itertools
import string
import struct
import urllib
import urllib2
import requests
from bs4 import BeautifulSoup
import gzip
import pickle
import sqlite3
from StringIO import StringIO

filename = "PracticeInput.txt"
#filename = "Happy-Numbers_InputForSubmission_3.txt"

import string
digs = string.digits + string.letters

def int2base(x, base):
  if x < 0: sign = -1
  elif x == 0: return digs[0]
  else: sign = 1
  x *= sign
  digits = []
  while x:
    digits.append(digs[x % base])
    x /= base
  if sign < 0:
    digits.append('-')
  digits.reverse()
  return ''.join(digits)


with open(filename, "r") as f:
	for line in f:
		#print line.strip('\n')
		tmp_thing = line.split(',')
		#print tmp_thing
		
			in_base = int(tmp_thing[1])
			out_base = int(tmp_thing[2])
		try:
			a = int(tmp_thing[0],in_base)
			b = int2base(a, out_base)
			print b
		except Exception:
			print "Input Invalid"
