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
	/*compute sum of arithmatic sequence*/
	unsigned ii;
	unsigned long A, B, M;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf("%lu %lu %lu", &A, &B, &M);
		if(rc != 3 || errno != 0) {
			free(result);
			fprintf(stderr, "Input read error on line\n", ii+2);
			perror("errno = ");
			exit(-1);
		}
		/*perform the computation in place*/
		result[ii] += A*M;
		result[ii] += (((long)pow(M,2) - M) / 2) * B; 
	}
	for(ii = 0; ii < N; ++ii){
		printf("%d ",result[ii]);
	}
	printf("\n");
	return 0;
}
