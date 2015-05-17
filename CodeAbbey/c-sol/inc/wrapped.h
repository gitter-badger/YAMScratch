#ifndef _WRAPPERS_
#define _WRAPPERS_

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
	printf("in wrapper\n");
	return atoi(str);
}

#endif
