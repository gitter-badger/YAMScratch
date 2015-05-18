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
	Vector_t(int)* vec = newVector(int);

	bytes_read = wrap_getline(&lineptr, &nbytes, stdin);

	/*account for the terminating null char from getline*/
	bytes_read++;
	tokenizeLine(int, vec, int_from_string, lineptr, bytes_read, DELIMETER);
	if(vec->elms < 1) {
		vector_destroy(int, vec);
		errno = EINVAL;
		perror("Enter at one integer > 0");
		exit(-1);
	}
	unsigned N;
	N = vec->items[0];
	Vector_t(int)* result = newVector(int);

	unsigned ii, tmp;
	for(ii = 0; ii < N; ii++) {
		vector_clear(int, vec);
		free(lineptr);
		lineptr = NULL;
		nbytes = 0;
		bytes_read = wrap_getline(&lineptr, &nbytes, stdin);
		bytes_read++;
		tokenizeLine(int, vec , int_from_string, lineptr, bytes_read, DELIMETER);
		if(vec->elms >= 2){
			tmp = vec->items[0] + vec->items[1];
			vector_push_back(int, result, tmp);	
		}
	}
	free(lineptr);
	vector_destroy(int, vec);

	for(ii = 0; ii < result->elms; ii++){
		printf("%d ", result->items[ii]);
	}
	printf("\n");
	vector_destroy(int, result);

	return 0;
}
