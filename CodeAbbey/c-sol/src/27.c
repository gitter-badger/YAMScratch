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
	long* result;
	result = (long*) calloc(N, sizeof(long));
	if(result == NULL) {
		perror("Failed to allocate result buffer.");
		exit(-1);
	}
	/*read in the array*/
	unsigned ii, swaps, passes;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%lu", result+ii);
		if(rc != 1 || !rc) {
			if(!errno) errno = EINVAL;
			perror("Enter failed to read integer");
			exit(-1);
		}
	}
	/*bubble sort the result array*/
	char pass_flag;
	swaps = 0;
	passes = 0;
	pass_flag = 1;
	long tmp;
	while(pass_flag) {
		passes++;
		pass_flag = 0;
		for(ii = 0; ii < N-1; ++ii) {
			if(result[ii] > result[ii+1]) {
				tmp = result[ii];
				result[ii] = result[ii+1];
				result[ii+1] = tmp;
				swaps++;
				pass_flag = 1;
			}
		}
	}
	printf("%d %d\n", passes, swaps);
	return 0;
}
