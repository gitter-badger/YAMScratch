import urllib
import urllib2
import sys
import os
from StringIO import StringIO
import gzip
import re


from bs4 import BeautifulSoup

class DataStructures(object):

	def __init__(self):
		self.root_url = "http://www.cs.rpi.edu/academics/courses/fall14/csci1200/"

		self.header = {}
		self.header['Host'] = 'www.cs.rpi.edu'
		self.header['Accept']="text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
		self.header['Accept-Language'] = 'en,en-GB;q=0.5'
		self.header['Accept-Encoding'] = 'gzip,deflate'
		self.header['Connection'] = 'keep-alive'
		self.header['Cache-Control'] = 'max-age=0'

		self.data = urllib.urlencode({})

		self.calendar_link = None

	def find_calendar(self):
		req = urllib2.Request(self.root_url,self.data,self.header)
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
		for link in soup.body.find_all('a'):
			if re.match(cal_match,str(link.string)):
				if not self.calendar_link:
					#store the link for the calenar
					self.calendar_link = link['href'] #only take the first calender
				else:
					print "More than one calendar found!!"
					raise RuntimeError

	def read_calendar(self,calendar_link):
		req = urllib2.Request(calendar_link,self.data,self.header)
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
					#print tray.name
					pass
				else:
					if tray['valign'] == 'top':
						self.main_tray = tray

''' END CLASS'''

def white(element,base_url):
	#there is nothing in a white element
	print "Nothing Day"
	pass


def green(element,base_url):
	#this is a lecture day
	lecture_pdf_links =[]
	other_content_links = []

	#getting the links
	for link in element.find_all('a'):
		if link.string is None:
			if len(link.contents) == 3:
				#every entry should have a
				folder_name = (link.contents[0]+" "+link.contents[2]).encode('ascii', 'ignore')
				lecture_pdf_links.append([folder_name,link['href']])

		else:
			other_content_links.append(link['href'])

	#getting the files
	for index,sub_list in enumerate(lecture_pdf_links):
		#there may be more than one link with string text
		if not index:
			#we only take the first link as the name of the folder
			if len(sub_list) == 2:
				if not os.path.exists(sub_list[0]):
					os.mkdir(sub_list[0])
					print "Created:",sub_list[0]
				else:
					print "Updating:",sub_list[0]
				os.chdir(sub_list[0])
				filename = sub_list[1].split("/")[-1]
				file_url = base_url+sub_list[1]
				print "Downloading...", filename
				urllib.urlretrieve(file_url,filename)

			else:
				#there shoul not be more than two items in the list
				raise RuntimeError
		else:
			#start adding to the folder
			filename = sub_list[1].split("/")[-1]
			file_url = base_url+sub_list[1]
			print "Downloading...", filename
			urllib.urlretrieve(file_url,filename)

	#careful: content is a string of url, not a list like sub_list above
	for content in other_content_links:
		filename = content.split("/")[-1]
		file_url = base_url+content
		print "Downloading...", filename
		urllib.urlretrieve(file_url,filename)

	#clean up by returning to root file
	os.chdir("../")

def red(element,base_url):
	#this is a test day
	#print "Test Day"
	#print element.prettify()
	for link in element.find_all('a'):
		print link.string, ':',link['href']
		filename = link['href'].split('/')

def yellow(element,base_url):
	#this is a lab day
	print "Lab Day"
	for link in element.find_all('a'):
		print link.string, ':',link['href']
		

def blue(element,base_url):
	#this is homework day
	print "Homework Day"
	all_links = {}
	for link in element.find_all('a'):
		print link.string, ':',link['href']


cal_switch = {
	#'calendar_white':white,
	'calendar_green':green,
	#'calendar_red':red,
	#'calendar_yellow':yellow,
	#'calendar_blue':blue,
	'default': lambda x: x
}


if __name__ == '__main__':
	#determine how to call
	if len(sys.argv) == 1:
		print "\n","This will create folder in current script directory"
		procede = raw_input("continue? [y/n]") # this will be a string
		if procede.lower() == 'y':
			root_filepath = os.getcwd()
			#anything else will abort
		else:
			exit()
		

	elif len(sys.argv) == 2:
		root_filepath = sys.argv[1]

	target_dirname = "Data Structures"
	target_dir_path = os.path.join(root_filepath,target_dirname)

	if not os.path.exists(target_dir_path):
		os.mkdir(target_dir_path)
		print "Created:",target_dir_path,"\n"

	#switch to our new directory
	os.chdir(target_dir_path)

	#start the pages
	ds = DataStructures()
	ds.find_calendar()
	if ds.calendar_link:
		ds.read_calendar(ds.calendar_link)


	for part in ds.main_tray.find_all('td'):
		case = part['class'][0]
		print "#"*50
		try:
			func = cal_switch[case]
		except KeyError:
			pass
		else:
			func(part,ds.root_url)


