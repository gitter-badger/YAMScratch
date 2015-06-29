#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
	unsigned N;
	int rc;
	/*M is rows, N is columns*/
	rc = scanf("%u ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	double* result;
	result = (double*)malloc(N * sizeof(double));
	if(result == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	}
	unsigned ii;
	double S, A, B;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%lf %lf %lf ", &S, &A, &B);
		if(rc != 3) {
			perror("input read error");
			exit(-1);
		}
		result[ii] = (S * A) / (A+B);
	}
	for(ii = 0; ii < N; ++ii) {
		printf("%15.15f ",result[ii]);
	}
	printf("\n");
	free(result);
	return 0;
}
