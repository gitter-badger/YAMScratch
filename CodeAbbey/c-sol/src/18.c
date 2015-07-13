#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	/*find square root by Babalonyan method*/
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	unsigned ii, jj, iterations;
	double r, d, v;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf("%lf %u", &v, &iterations);
		if(rc != 2) {
			fprintf(stderr, "input read error on line %d\n", ii+2);
			fflush(stderr);
			perror("");
			exit(-1);
		}
		r = 1;
		for(jj = 0; jj < iterations; ++jj) {
			d = v / r;
			r += d;
			r /= 2.0;
		}
		printf("%15.15lf ", r);
	}
	printf("\n");
	return 0;
}
