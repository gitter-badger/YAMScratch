import sqlite3
import sys
import os

#the sublime text .tmLanguage does not have a descernable pattern yet

#this utility is inteneded to parse a sublime package for language definitions


if len(sys.argv) == 3:
	conn = sqlite3.connect("mtvr_db.sqlite")

else:
	print "USAGE:"
	print "extension_loader.py <sublime package> <database>"