#! /bin/bash
#link to the java package for parsing html
javac -cp ~/Documents/GitHub/jsoup/target/classes/org/:.  -d ./build/ \
	WebInspector.java 