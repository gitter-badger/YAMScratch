#define _GNU_SOURCE
#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <string.h> /*provides strsep*/

#include "parseCodeAbbeyInput.h"
#include "wrapped.h"

VECTOR_INIT(signed)
/*TOKENIZE must come after VECTOR declaration*/
TOKENIZE_INIT(signed)


#define DELIMETER ' '
#define MIN(a,b) a < b ? a : b


int main(int argc, char* argv[]) {
	/*setup our parser template*/
	int (*int_from_string)(char*);
	int_from_string = &wrap_atoi;
	size_t nbytes;
	ssize_t bytes_read;
	char* lineptr;
	lineptr = NULL;
	nbytes = 0;
	Vector(signed)* vec = newVector(signed);
	bytes_read = wrap_getline(&lineptr, &nbytes, stdin);
	/*account for the terminating null char from getline*/
	bytes_read++;
	tokenizeLine(signed, vec, int_from_string, lineptr, bytes_read, DELIMETER);
	if(vec->elms < 1) {
		vector_destroy(signed, vec);
		errno = EINVAL;
		perror("Enter at one integer > 0");
		exit(-1);
	}
	unsigned ii;
	unsigned N;
	signed tmp;
	/*constructor should guaratee there is at least one element*/
	N = vec->items[0];
	if(vec->elms != N+1) {
		fprintf(stderr, "Not enough input arguments\n");
		vector_destroy(signed, vec);
		exit(-1);
	}
	Vector(signed)* result = newVector(signed);
	/*convert Farenheit to Celsius*/
	for(ii = 1; ii < vec->elms; ++ii) {
		tmp = vec->items[ii] - 32;
		tmp = (signed)((float) tmp  * 5.0 / 9.0 + 0.5);
		vector_push_back(signed, result, tmp);
	}

	for(ii = 0; ii < result->elms; ++ii) {
		printf("%d ", result->items[ii] );
	}
	printf("\n");
	vector_destroy(signed, vec);
	/*hack to get around spaces in typename*/

	return 0;
}
