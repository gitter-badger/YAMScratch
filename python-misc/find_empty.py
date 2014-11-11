import sys
import os

if len(sys.argv) != 2:
	print "USAGE find_empty.py <root dir>"
else:
	print "dir",sys.argv[1]
	for root,dirs,files in os.walk(sys.argv[1]):
		for di in dirs:
			for r,d,fs in os.walk(os.path.join(root+di)):
				if len(fs) == 0:
					path,filename = os.path.split(r)
					f = open("empty_dirs.txt","a+")
					f.write(filename+'\n')
					f.close()


