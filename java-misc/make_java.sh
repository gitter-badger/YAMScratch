#! /bin/bash
#link to the java package for parsing html

_FILES=( WebInspector )
FILES=( "${_FILES[@]/%/\.java}" )
BUILD=( "${_FILES[@]/%/\.class}" )

javac 	-cp ~/Documents/GitHub/jsoup/target/classes/org/:.  \
		-g 				\
		-d  ./build/ 	\
	$FILES

jar -cf "WebInspector.jar" ./build