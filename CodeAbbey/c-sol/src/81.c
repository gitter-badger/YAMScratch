#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/

int main(int argc, char* argv[]) {
	unsigned N;
	int rc;
	rc = scanf("%u ", &N);
	if(rc != 1 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}
	unsigned* result = (unsigned*)calloc(N, sizeof(unsigned));
	if(result == NULL){
		perror("failed to allocate ouput array");
		exit(-1);
	}
	unsigned ii, kk;
	uint32_t tmp, mask;
	mask = 1<<31;
	for(ii = 0; ii < N; ++ii) {
		/*load this into fixed width*/
		scanf("%d", &tmp);
		for(kk = 0; kk < 32; ++kk){
			if((tmp<<kk)&mask) {
				++result[ii];
			}
		}
	}
	for(ii = 0; ii < N; ++ii){
		printf("%u ", result[ii]);
	}
	printf("\n");
	return 0;
}
