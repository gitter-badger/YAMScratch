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
	Vector_t(int)* foo = newVector(int);

	bytes_read = wrap_getline(&lineptr, &nbytes, stdin);

	printf("Read: %s\n",lineptr );
	printf("%p\n", int_from_string );
	printf("Bytes read %d\n", bytes_read );
	/*account for the terminating null char from getline*/
	bytes_read++;
	tokenizeLine(int, foo, int_from_string, lineptr, bytes_read, DELIMETER);

	printf("%d\n", foo->elms );
	vector_destroy(int, foo);

	return 0;
}
