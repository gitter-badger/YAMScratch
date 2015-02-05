import urllib
import urllib2
import sys
import os
from StringIO import StringIO
import gzip

from bs4 import BeautifulSoup

url = "http://www.scorec.rpi.edu/~shephard/"

header = {'User-Agent': "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.4 (KHTML, like Gecko) Chrome/22.0.1250.0 Iron/22.0.2150.0 Safari/537.4"}
header['Host'] = "www.scorec.rpi.edu"
header['Accept'] = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
header['Connection'] = "keep-alive"
header['Pragma'] = "no-cache"
header['Cache-Control'] = "no-cache"
header['Accept-Language'] = "en,en-GB,q=0.5"
header['Accept-Encoding'] = "gzip,deflate"

data = urllib.urlencode({})

req = urllib2.Request(url,data,header)
response = urllib2.urlopen(req)
page = response.read(response)
if response.info().get('Content-Encoding') == 'gzip':
			vuff = StringIO(page)
			f = gzip.GzipFile(fileobj=vuff)
			data = f.read()
			soup = BeautifulSoup(data)
else:
	soup = BeautifulSoup(page)

print soup.prettify()
