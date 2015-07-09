/*make sure that we have the getline() extension provided by GNU C library*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	unsigned* result;
	errno = 0;
	result = (unsigned*)malloc(N * sizeof(unsigned));
	if(result == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	}
	/*find the first fibonacci number divisible by A*/
	unsigned ii;
	unsigned long A;
	char * lineptr;
	lineptr = NULL;
	size_t nbytes;
	nbytes = 0;
	for(ii = 0; ii < N; ++ii) {
		result[ii] = 0;
		errno = 0;
		ssize_t bytes_read;
		bytes_read = getline(&lineptr, &nbytes, stdin);
		if(bytes_read < 0) {
			/*this checks for when there is only EOF */
			errno = EINVAL;
			perror("Must input a number");
			exit(-1);
		} else if(bytes_read == 1) {
			/*check for empty line which should not happen*/
			if(*lineptr == '\n') {
				errno = EINVAL;
				perror("Lines must have numbers on them");
				exit(-1);
			}
		}
		FILE * stream;
		stream = fmemopen(lineptr, bytes_read, "r");
		for(;;) {
			rc = fscanf(stream," %lu ", &A);
			if(rc == EOF) {
				break;
			}
			result[ii] += A * A;
		}
		fclose(stream);
	}
	free(lineptr);
	for(ii = 0; ii < N; ++ii) {
		printf("%d ",result[ii]);
	}

	printf("\n");
	free(result);
	return 0;
}
