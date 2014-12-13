import urllib
import urllib2
import sys
import os
from StringIO import StringIO
import gzip
import re


from bs4 import BeautifulSoup

root_url = "http://www.cs.rpi.edu/academics/courses/fall14/csci1200/"

header = {}
header['Host'] = 'www.cs.rpi.edu'
header['Accept']="text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
header['Accept-Language'] = 'en,en-GB;q=0.5'
header['Accept-Encoding'] = 'gzip,deflate'
header['Connection'] = 'keep-alive'
header['Cache-Control'] = 'max-age=0'

data = urllib.urlencode({})

req = urllib2.Request(root_url,data,header)
response = urllib2.urlopen(req)
page = response.read()
#the content is zipped so we must unzip 
if response.info().get('Content-Encoding') == 'gzip':
			vuff = StringIO(page)
			f = gzip.GzipFile(fileobj=vuff)
			data = f.read()
                                
soup = BeautifulSoup(data)
#find the calender where the links are
cal_match = re.compile('[cC]alendar')
calender_link = None
for link in soup.body.find_all('a'):
	if re.match(cal_match,str(link.string)):
		if not calender_link:
			calender_link = link['href'] #only take the first calender
		else:
			print "More than one calendar found!!"
			raise RuntimeError

req = urllib2.Request(calender_link,data,header)
response = urllib2.urlopen(req)
cal_page = response.read()
if response.info().get('Content-Encoding') == 'gzip':
			vuff = StringIO(cal_page)
			f = gzip.GzipFile(fileobj=vuff)
			cal_data = f.read()
cal_soup = BeautifulSoup(cal_data)
table = cal_soup.body.table
for tray in cal_soup.body.table.children:
	if tray.name:
		try:
			tray['valign']
		except KeyError as e:
			print tray.name
		else:
			if tray['valign'] == 'top':
				main_tray = tray

def white(element):
	#there is nothing in a white element
	print "Nothing Day"
	pass

def green(element):
	#this is a lecture day
	print "Lecture Day"
	for link in element.find_all('a'):
		print link.string

def red(element):
	#this is a test day
	print "Test Day"
	print element.prettify()
	for link in element.find_all('a'):
		print link.string

def yellow(element):
	#this is a lab day
	print "Lab Day"
	for link in element.find_all('a'):
		print link.string

def blue(element):
	#this is homework day
	print "Homework Day"
	for link in element.find_all('a'):
		print link.string

cal_switch = {
	#'calendar_white':white,
	#'calendar_green':green,
	'calendar_red':red,
	#'calendar_yellow':yellow,
	#'calendar_blue':blue
	'default': lambda x: x
}

print os.getcwd()
for part in main_tray.find_all('td'):
	case = part['class'][0]
	print "#"*50
	try:
		func = cal_switch[case]
	except KeyError:
		pass
	else:
		func(part)


