#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/

#define MODULO 10000007

int main(int argc, char* argv[]) {
	unsigned N, M;
	int rc;
	rc = scanf("%u %u", &N, &M);
	if(rc != 2 || !rc) {
		errno = EINVAL;
		perror("Enter only two integers > 0 on the first line.");
		exit(-1);
	}
	unsigned* result;
	result = (unsigned*) calloc(M, sizeof(unsigned));
	if(result == NULL) {
		perror("Failed to allocate result buffer.");
		exit(-1);
	}
	/*count number of times integer appears in sequence*/
	unsigned ii;
	long tmp;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%lu", &tmp);
		if(rc != 1) {
			perror("Error in reading input from command line");
			exit(-1);
		}
		if(tmp > M || tmp < 1) {
			errno = EINVAL;
			perror("input should be in [1,M]");
			exit(-1);
		}
		result[tmp-1]++;

	}
	for(ii = 0; ii < M; ++ii){
		printf("%d ",result[ii]);
	}
	printf("\n");
	return 0;
}
