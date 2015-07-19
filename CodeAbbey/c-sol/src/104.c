#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

int main(int argc, char const *argv[])
{
	/*compute area of triangle using its points*/
	int rc;
	unsigned N;
	errno = 0;
	rc = scanf("%u ", &N);
	if(rc != 1) {
		perror("failed to read first line of input");
		exit(-1);
	}

	unsigned ii;
	double x1, y1, x2, y2, x3, y3, A;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf("%lf %lf %lf %lf %lf %lf", &x1, &y1, &x2, &y2, &x3, &y3);
		if(rc != 6 || errno != 0) {
			fprintf(stderr, "input read failed on line %u\n", ii+2);
			fflush(stderr);
			perror("");
			exit(-1);
		}
		A = (x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2))/2;
		printf("%15.15lf ", fabs(A) );
	}
	printf("\n");
	return 0;
}
