#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/

#define MODULO 10000007

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
	/*compute the collatz sequence for each input integer*/
	unsigned ii;
	long tmp;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%lu", &tmp);
		/*perform the computation in place*/
		while(tmp != 1) {
			result[ii]++;
			if(tmp % 2) {
				tmp *= 3;
				++tmp;
			} else {
				tmp >>= 1;
			}
		}
	}
	for(ii = 0; ii < N; ++ii){
		printf("%d ",result[ii]);
	}
	printf("\n");
	return 0;
}
