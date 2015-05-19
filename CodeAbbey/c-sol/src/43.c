#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <string.h> /*provides strsep*/
#include <math.h> /*provides pow()*/

#include "parseCodeAbbeyInput.h"
#include "wrapped.h"


VECTOR_INIT(int)
VECTOR_INIT(double)
/*TOKENIZE must come after VECTOR declaration*/
TOKENIZE_INIT(int)
TOKENIZE_INIT(double)


#define DICE_MAX 6
#define DELIMETER ' '

int main(int argc, char* argv[]) {
	/*setup our parser template*/
	size_t nbytes;
	ssize_t bytes_read;
	char* lineptr;
	lineptr = NULL;
	nbytes = 0;
	Vector_t(int)* vec = newVector(int);
	bytes_read = wrap_getline(&lineptr, &nbytes, stdin);
	/*account for the terminating null char from getline*/
	bytes_read++;
	tokenizeLine(int, vec, &wrap_atoi, lineptr, bytes_read, DELIMETER);
	if(vec->elms < 1) {
		vector_destroy(int, vec);
		errno = EINVAL;
		perror("Enter at one integer > 0");
		exit(-1);
	}
	unsigned N;
	N = vec->items[0];
	vector_destroy(int, vec);

	Vector_t(int)* result = newVector(int);
	Vector_t(double)* v2 = newVector(double);

	unsigned ii;
	float tmp;
	/*compute the BMI*/
	for(ii = 0; ii < N; ii++) {
		vector_clear(double, v2);
		free(lineptr);
		lineptr = NULL;
		nbytes = 0;
		bytes_read = wrap_getline(&lineptr, &nbytes, stdin);
		/*account for the terminating null char from getline*/
		bytes_read++;
		tokenizeLine(double, v2 , &wrap_atof, lineptr, bytes_read, DELIMETER);
		if(v2->elms >= 1){
			tmp = (unsigned) (v2->items[0] * DICE_MAX);
			vector_push_back(int, result, ++tmp);	
		}
	}
	free(lineptr);
	vector_destroy(double, v2);
	for(ii = 0; ii < result->elms; ii++){
		printf("%d ", result->items[ii]);
	}
	printf("\n");
	vector_destroy(int, result);
	return 0;
}
