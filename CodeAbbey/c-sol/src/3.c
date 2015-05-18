#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <string.h> /*provides strsep*/

#include "parseCodeAbbeyInput.h"
#include "wrapped.h"

VECTOR_INIT(int)
/*TOKENIZE must come after VECTOR declaration*/
TOKENIZE_INIT(int)


#define DELIMETER ' '


int main(int argc, char* argv[]) {
	/*setup our parser template*/
	int (*int_from_string)(char*);
	int_from_string = &wrap_atoi;
	size_t nbytes;
	ssize_t bytes_read;
	char* lineptr;
	lineptr = NULL;
	nbytes = 0;
	Vector_t(int)* first_line = newVector(int);

	bytes_read = wrap_getline(&lineptr, &nbytes, stdin);

	/*account for the terminating null char from getline*/
	bytes_read++;
	tokenizeLine(int, first_line, int_from_string, lineptr, bytes_read, DELIMETER);

	if(first_line->elms < 1) {
		vector_destroy(int, first_line);
		errno = EINVAL;
		perror("Enter at one integer > 0");
		exit(-1);
	}
	int N;
	N = first_line->items[0];
	printf("N = %d\n",N );

	free(lineptr);
	vector_destroy(int, first_line);
	return 0;
}
