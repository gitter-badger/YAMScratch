#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
	int rc;
	unsigned A, I;
	unsigned long N;
	char * lineptr;
	size_t nbytes;
	ssize_t bytes_read;
	FILE * stream;
	lineptr = NULL;
	nbytes = 0;
	errno = 0;
	bytes_read = getline(&lineptr, &nbytes, stdin);
	if(bytes_read < 0) {
		/*this checks for when there is only EOF */
		perror("EOF reached before proper input could be processed");
		exit(-1);
	} else if(bytes_read == 1) {
		/*check for empty line which should not happen*/
		if(*lineptr == '\n') {
			perror("Lines should not be empty");
			exit(-1);
		}
	}
	stream = fmemopen(lineptr, bytes_read, "r");
	N = 0;
	I = 0;
	for(;;) {
		rc = fscanf(stream," %u ", &A);
		if(rc == EOF) {
			break;
		}
		/*A should be 1 or zero*/
		assert(A < 2);
		/*this stores I as big endian, position 0 is represented by the high bit*/
		I |= A<<N;
		++N;
	}
	if(N > 64) {
		/*we cannot represent more than 64 bits using long
		* so we must give an error in this implementation*/
		fprintf(stderr, "More than 64 states given by input\n");
		fflush(stderr);
		exit(-1);
	}
	fclose(stream);
	/*
	* Allocate a buffer to hold the N bitmasks, this
	* is purposefully zeroed in case there is no rule
	* for an index, so it will have no effect on latter 
	* computations. This is done because input rule order
	* is not defined.
	*/
	unsigned long* masks;
	masks = (unsigned long*)calloc(N, sizeof(unsigned long));
	/*read in the N mappings*/
	unsigned ii, jj;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		bytes_read = getline(&lineptr, &nbytes, stdin);
		if(bytes_read < 0) {
			/*this checks for when there is only EOF */
			perror("Must input a number");
			exit(-1);
		} else if(bytes_read == 1) {
			/*check for empty line which should not happen*/
			if(*lineptr == '\n') {
				perror("Lines must have numbers on them");
				exit(-1);
			}
		}
		stream = fmemopen(lineptr, bytes_read, "r");
		/*find the index of the rule, these can be out of order
		* and the last rule present will overwrite all previous ones
		* silently
		*/
		char colon;
		rc = fscanf(stream, "%u %c", &A, &colon);
		printf("rc = %d, %c\n", rc, colon);
		for(jj = 0;;) {
			rc = fscanf(stream," %u ", &A);
			if(rc == EOF) {
				break;
			}
			printf("A = %u\n", A );
			++jj;
		}
		fclose(stream);
	}
	free(lineptr);
	printf("\n");
	return 0;
}
