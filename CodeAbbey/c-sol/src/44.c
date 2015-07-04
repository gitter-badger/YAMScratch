#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define DIE_SIDES 6

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
	/*
	** find the total sum of a 2d6 roll using large random number
	** each line of input is two random numbers for dice rools
	*/
	unsigned ii;
	unsigned long A, B;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf(" %lu %lu ", &A, &B);
		if(rc != 2) {
			fprintf(stderr, "input read error at line %u\n", ii +2 );
			fflush(stderr);
			perror("errno: ");
			exit(-1);
		}
		/*
		** each line crosses 2(T-1) other lines, there are N lines
		** and each intersection is made up of two lines, so we divide
		** by two to prevent double counting of intersections
		*/
		result[ii] = ((A % DIE_SIDES) + (B % DIE_SIDES) + 2);
	}

	for(ii = 0; ii < N; ++ii) {
		printf("%d ",result[ii]);
	}
	printf("\n");
	free(result);
	return 0;
}
