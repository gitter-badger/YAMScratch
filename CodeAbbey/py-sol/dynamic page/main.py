import flask
import urllib2
import re
import time
#from google.appengine.ext import db
from google.appengine.ext import ndb

import random

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

@app.route('/query-string')
def parse_query():
	#flask.request
	#return flask.render_template()
	return "Hello there"

@app.errorhandler(404)
def page_not_found(e):
    return 'Sorry, nothing at this URL.', 404
