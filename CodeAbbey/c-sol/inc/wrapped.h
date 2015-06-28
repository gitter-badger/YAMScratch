#ifndef _WRAPPERS_
#define _WRAPPERS_

#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>

ssize_t wrap_getline(char** lineptr, size_t* nbytes, FILE * stream) {
	ssize_t bytes_read;
	bytes_read = getline(lineptr, nbytes, stream);
	if(bytes_read < 0) {
		/*this checks for when there is only EOF */
		errno = EINVAL;
		perror("Must input a number");
		exit(EINVAL);
	} else if(bytes_read == 1) {
		/*check for newline ignoring specific EOL*/
		if(*(*lineptr) == '\n') {
			errno = EINVAL;
			perror("Please input a number");
			exit(EINVAL);
		}
	}
	return bytes_read;
}

int wrap_atoi(char* str) {
	return atoi(str);
}

double wrap_atof(char* str) {
	return atof(str);
}

char* string_duplicate(char* input) {
	char* out;
	unsigned len;
	len = strlen(input);
	out = (char*)malloc(sizeof(char)*(len+1));
	return strcpy(out, input);
}

#endif
