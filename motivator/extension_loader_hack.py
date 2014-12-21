import sqlite3
import sys
import os

#the sublime text .tmLanguage does not have a descernable pattern yet

#use this to manually stage the data for insertion


if len(sys.argv) == 3:
	database_name = sys.argv[2]
	package_name = sys.argv[1]

	valid = {"yes":True,"y":True,"": True,
			 "no":False, "n":False}
	if not ".sqlite" in database_name:
		#test for valid sqlite database name
		print "ASSUMING extension {}.sqlite ".format(database_name)
		database_name += ".sqlite"
	if not os.path.exists(database_name):
		while True:
			#run until we get a valid response
			resp = raw_input("Database \"{}\" does not exist. Do you want to create it? [Y/n]".format(database_name)).lower()
			if resp in valid:
				if not valid[resp]:
					#we did not want to create database
					print "Exiting..."
					database_name = None
					break
				else:
					print "Creating... {}".format(database_name)
					break
	if not database_name:
		exit()
	conn = sqlite3.connect(database_name)
	cur = conn.cursor()
	#parameterise the table name
	exten_tbname = "extensions"
	#check for presence of table
	cur.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='{}'".format(exten_tbname))
	empty_test = cur.fetchone()
	if empty_test is None:
		print "Creating TABLE \"{}\"".format(exten_tbname)
		conn.execute('CREATE TABLE %s (language text,extension text)' % exten_tbname)
	#now that the table exists


	while True:
		a = raw_input(">>")
		try:
			exec(a)
		except Exception as e:
			print e

else:
	print "USAGE:"
	print "extension_loader.py <sublime package> <database>"