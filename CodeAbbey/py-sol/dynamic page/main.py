import flask
import urllib2
import re
import time
#from google.appengine.ext import db
from google.appengine.ext import ndb

import random
import urlparse

app = flask.Flask(__name__)
app.config['DEBUG'] = False

class Employee(ndb.Model):
	name = ndb.StringProperty(indexed = True)


@app.route('/')
def index():
    return flask.render_template('index.html')

@app.route('/random-page/<int:value>')
def build_random_page(value=0):
	out_val = value * random.randint(10, 1000)
	#The check does not like the rendered template, it expects only the text
	#return flask.render_template('secret_number.html', secret_value = out_val)
	return "Random value is {0}".format(out_val)

@app.route('/horoscope-result', methods = ['GET', 'POST'])
def parse_query():
	#configure lucky number generator
	A, B, C = (36, 48, 11)
	name = flask.request.args.get('name')
	year = flask.request.args.get('year')
	month = flask.request.args.get('month')
	day = flask.request.args.get('day')

	#sanity checks first
	if len(year) > 4:
		return "Incorrect parameters were passed!"
	if len(month) > 2:
		return "Incorrect parameters were passed!"
	if len(day) > 2:
		return "Incorrect parameters were passed!"
	try:
		year = int(year)
	except ValueError:
		return "Incorrect parameters were passed!"
	try:
		month = int(month)
	except ValueError:
		return "Incorrect parameters were passed!"
	try:
		day = int(day)
	except ValueError:
		return "Incorrect parameters were passed!"

	#print name, year, month, day
	lucky = (len(name) *A + month * B + day*C + year) % 31 + 1
	#return flask.render_template()
	return "Your lucky day is {0}".format(lucky)

@app.route('/horoscope')
def gen_form():
	#get the base url of what we are on on
	target_page = flask.request.host_url +'horoscope-result'
	#target_page = "http://dynamic-page.appspot.com/karl"
	return flask.render_template('horoscope.html', target_page = target_page)

@app.errorhandler(404)
def page_not_found(e):
    return 'Sorry, nothing at this URL.', 404
