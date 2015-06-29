#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

unsigned long linear_congruent_gen(unsigned long A, unsigned long C,
								unsigned long M, unsigned long Xcurr) {
	return (A * Xcurr + C ) % M;
}

int main(int argc, char const *argv[])
{
	unsigned N;
	int rc;
	errno = 0;
	rc = scanf(" %u ", &N);
	if(rc != 1 || (errno != 0)) {
		perror("failed to read input");
		exit(-1);
	}
	unsigned long* result;
	errno = 0;
	result = (unsigned long*)malloc(N * sizeof(unsigned long));
	if(result == NULL) {
		perror("failed to allocate output buffer\n");
		exit(-1);
	}
	/*compute the Nth generator output give X0 */
	unsigned ii, jj;
	unsigned long A, C, M, X0, Xnext;
	unsigned iterations;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf(" %lu %lu %lu %lu %u ", &A, &C, &M, &X0, &iterations);
		if(rc != 5) {
			printf("Input read error on line %d \n",ii+2);
			perror("");
			exit(-1);
		}
		Xnext = X0;
		for(jj = 0; jj < iterations; ++jj) {
			Xnext = linear_congruent_gen(A, C, M, Xnext);
		}
		result[ii] = Xnext;
	}
	for(ii = 0; ii < N; ++ii) {
		fprintf(stdout, "%lu ",result[ii]);
	}
	fprintf(stdout, "\n");
	free(result);
	return 0;
}
