import urllib
import urllib2
import sys
import os
import re
from StringIO import StringIO
import gzip

import lxml
from bs4 import BeautifulSoup

base_url = "http://www.scorec.rpi.edu/~shephard/"

header = {'User-Agent': "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.4 (KHTML, like Gecko) Chrome/22.0.1250.0 Iron/22.0.2150.0 Safari/537.4"}
header['Host'] = "www.scorec.rpi.edu"
header['Accept'] = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
header['Connection'] = "keep-alive"
header['Pragma'] = "no-cache"
header['Cache-Control'] = "no-cache"
header['Accept-Language'] = "en,en-GB,q=0.5"
header['Accept-Encoding'] = "gzip,deflate"

data = None

req = urllib2.Request(base_url,data,header)
print req
response = urllib2.urlopen(req)


def read_and_decode_with_bs4(response):
	page = response.read(response)
	if response.info().get('Content-Encoding') == 'gzip':
				vuff = StringIO(page)
				f = gzip.GzipFile(fileobj=vuff)
				data = f.read()
				soup = BeautifulSoup(data)
	else:
		soup = BeautifulSoup(page)
	return soup

soup = read_and_decode_with_bs4(response)
#all pieces
all_class_homepages = set()
for link in soup.find_all('a'):
	if link.has_attr('href'):
		#find all of the links that actually have something
		#ignoring the '/' directory
		if re.match('^..*/$', link['href']):
			print link['href']
			all_class_homepages.add(link)

for classpage in all_class_homepages:
	print
	class_url = base_url+classpage['href']
	req = urllib2.Request(class_url,None, header)
	response = urllib2.urlopen(req)
	this_soup = read_and_decode_with_bs4(response)

	for link in this_soup.find_all('a'):
		if link.has_attr('href'):
			#find all of the links that actually have something
			#ignoring the '/' directory
			if re.match('^..*/$', link['href']):
				print link['href']
	
	print class_url

#visit all of the class pages one by one and download everythin!!
#print soup.prettify()
