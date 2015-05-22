#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <math.h>

int main(int argc, char* argv[]) {
	unsigned N;
	int rc;
	rc = scanf("%u", &N);
	if(rc != 1 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}
	unsigned* result;
	result = (unsigned*) calloc(N, sizeof(unsigned));
	if(result == NULL) {
		perror("Failed to allocate result buffer.");
		exit(-1);
	}
	/*read in the array*/
	unsigned ii;
	long A, B, C;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%lu %lu %lu", &A, &B, &C);
		if(rc != 3 || !rc) {
			if(!errno) errno = EINVAL;
			perror("Enter failed to read integer");
			exit(-1);
		}
		/*Duff's device for pure fun*/
		switch (A > B) {
			case 1: 	do {
				A ^= B;
				B ^= A;
				A ^= B; /*Fall through*/
			case 0:
			 	if(B > C) {
			 		B ^= C;
					C ^= B;
					B ^= C;
			 	}
			 			} while (A > B);
		}
		if(A+B >= C) {
			result[ii] = 1;
		}
	}
	for(ii = 0; ii < N; ++ii) {
		printf("%d ", result[ii]);
	}
	printf("\n");
	return 0;
}
