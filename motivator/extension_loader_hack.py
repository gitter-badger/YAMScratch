import sqlite3
import sys
import os

#the sublime text .tmLanguage does not have a descernable pattern yet

#use this to manually stage the data for insertion


if len(sys.argv) == 3:
	database_name = sys.argv[2]
	package_name = sys.argv[1]

	def validate_database_name(db_name):
		valid = {"yes":True,"y":True,"": True,
				 "no":False, "n":False}
		if not ".sqlite" in db_name:
			#test for valid sqlite database name
			print "ASSUMING extension {}.sqlite ".format(db_name)
			db_name += ".sqlite"
		if not os.path.exists(db_name):
			while True:
				#run until we get a valid response
				resp = raw_input("Database does not exist. Do you want to create one? [Y/n]").lower()
				if resp in valid:
					if not valid[resp]:
						print "not valid"
						continue
					else:
						print "Creating... {}".format(db_name)
						return db_name
		else:
			return db_name

	database_name = validate_database_name(database_name)
	if not database_name:
		pass



			


	conn = sqlite3.connect(database_name)
	cur = conn.cursor()
	cur.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='extensions'")
	empty_test = cur.fetchone()
	if empty_test is None:
		cur.execute('CREATE TABLE extensions (language text,extension text)')
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