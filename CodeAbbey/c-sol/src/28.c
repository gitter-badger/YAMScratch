#define _GNU_SOURCE
#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <string.h> /*provides strsep*/
#include <math.h> /*provides pow()*/

#include "parseCodeAbbeyInput.h"
#include "wrapped.h"

/*hack to get around spaces in typename*/
typedef char* char_ptr;

VECTOR_INIT(int)
VECTOR_INIT(char_ptr)
VECTOR_INIT(double)
/*TOKENIZE must come after VECTOR declaration*/
TOKENIZE_INIT(int)
TOKENIZE_INIT(char_ptr)
TOKENIZE_INIT(double)


#define DELIMETER ' '
#define MIN(a,b) a < b ? a : b


int main(int argc, char* argv[]) {
	/*setup our parser template*/
	size_t nbytes;
	ssize_t bytes_read;
	char* lineptr;
	lineptr = NULL;
	nbytes = 0;
	Vector(int)* vec = newVector(int);
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

	Vector(char_ptr)* result = newVector(char_ptr);
	Vector(double)* v2 = newVector(double);

	unsigned ii;
	char* tmp;
	float BMI;
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
		if(v2->elms >= 2){
			BMI = v2->items[0] / (pow(v2->items[1], 2));
			if(BMI < 18.5){
				tmp = "under";
			} else if(BMI < 25.0) {
				tmp = "normal";
			} else if(BMI < 30.0) {
				tmp = "over";
			} else {
				tmp = "obese";
			}
			vector_push_back(char_ptr, result, tmp);	
		}
	}
	free(lineptr);
	vector_destroy(double, v2);
	for(ii = 0; ii < result->elms; ii++){
		printf("%s ", result->items[ii]);
	}
	printf("\n");
	vector_destroy(char_ptr, result);
	return 0;
}
