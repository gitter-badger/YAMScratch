#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/

#include "code_abbey_utility.h"

int main(int argc, char* argv[]) {
	unsigned N;
	int rc;
	rc = scanf("%u", &N);
	if(rc != 1 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}
	/*find average of array*/
	unsigned ii;
	unsigned long A, B, M;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf(" %lu %lu %lu ", &A, &B, &M);
		if(rc != 3) {
			perror("Error in reading input from command line");
			exit(-1);
		}
		printf("%lu ", modular_exponentiation(A, B, M) );
	}
	printf("\n");
	return 0;
}
