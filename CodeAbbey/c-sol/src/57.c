#include <stdlib.h>
#include <stdio.h>
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
	double* result;
	errno = 0;
	result = (double*)malloc(N * sizeof(double));
	if(result == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	}
	/*
	** Smooth array of temperature data by averaging each value
	** with its two neighbors, not including the end values
	*/
	unsigned ii;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		/*read input so we can perform cal*/
		rc = scanf(" %lf ", result+ii);
		if(rc != 1) {
			free(result);
			fprintf(stderr, "input read error at line %u\n", ii +2 );
			fflush(stderr);
			perror("errno: ");
			exit(-1);
		}
	}
	double prev, tmp;
	prev = result[0];
	for(ii = 1; ii < N-1; ++ii) {
		tmp = prev + result[ii] + result[ii+1];
		tmp /= 3.0;
		/*save the current value since it will be replaced*/
		prev = result[ii];
		result[ii] = tmp;
	}
	for(ii = 0; ii < N; ++ii) {
		printf("%15.15lf ",result[ii]);
	}
	printf("\n");
	free(result);
	return 0;
}
