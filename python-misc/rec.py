import re
import copy
import sys

if len(sys.argv) != 2:
	print "\nUsage:\n  rec.py filename\n"
else:
	filename = sys.argv[1]
	with open(filename,'r') as f:
		all_lines = []
		for l in f:
			all_lines.append(l)

	#split the file on all characters
	temp_line = []
	for line in all_lines:
		exp = re.compile(r'[\s\?,\.!\'"\(\)&\$\+%\-#]')
		temp_line = re.split(exp, line)
		new_list = []
		for iii in temp_line:
			if not(iii == ''): # remove all empty strings
				new_list.append(iii)
		line_pal = []
		large_pal = []
		for Uindex,uuu in enumerate(new_list):
			candidate = uuu.lower().decode('utf-8') #lower case and unicode
			if Uindex == len(new_list)-1:
				if candidate == candidate[::-1]:
					line_pal.append(candidate) # check the very last word
			else:
				for lll in range(Uindex+1,len(new_list)):
					if candidate == candidate[::-1]:
						#then we have a palindrome
						line_pal.append([candidate,uuu,new_list[lll-1]])
					candidate = candidate+new_list[lll].lower().decode('utf-8')
					#we add something new and try again
		temp_len = 0
		for thing in line_pal: # return only largest palindrome phrase
			if len(thing[0]) > temp_len:
				temp_len = len(thing[0])
				large_pal = copy.deepcopy(thing)
		foo = copy.deepcopy(line)
		max_num = 2*len(large_pal[0])- len(large_pal[1])-len(large_pal[2])
		foo = re.compile(large_pal[1]+r'.{0,'+str(max_num)+'}'+large_pal[2])
		for found in re.findall(foo,line):
			print re.findall(foo,found)[0].decode('utf-8')

