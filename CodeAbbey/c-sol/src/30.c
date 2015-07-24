#define _GNU_SOURCE
#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/

#include "wrapped.h"

int main(int argc, char* argv[]) {
	size_t nbytes;
	ssize_t bytes_read;
	char* lineptr, * front, * back;
	lineptr = NULL;
	nbytes = 0;
	bytes_read = wrap_getline(&lineptr, &nbytes, stdin);
	/*reverse the string in place*/
	/*accounting for the terminating null and strip any newline*/
	back = lineptr + --bytes_read;
	while(*back == '\n') {
		*back-- = '\0';
		bytes_read--;
	}
	front = lineptr;
	unsigned ii;
	char tmp;
	if(back)
	for(ii = 0; ii <= bytes_read/2; ++ii) {
		tmp = *front;
		*front++ = *back;
		*back-- = tmp; 
	}
	printf("%s\n",lineptr );
	free(lineptr);
	return 0;
}
