#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/

#include "code_abbey_utility.h"
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
	long* input;
	input = (long*)malloc(N*sizeof(long));
	if(input == NULL) {
		perror("failed to allocate input buffer");
		exit(-1);
	}
	/*find average of array*/
	unsigned ii;
	long tmp, checksum;
	checksum = 0;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%ld", &tmp);
		if(rc != 1) {
			perror("Error in reading input from command line");
			exit(-1);
		}
		input[ii] = tmp;
		checksum += tmp;
		checksum *= 113;
		checksum %= MODULO;
	}
	/*check that utility function is the same*/
	long othersum;
	array_checksum(input, (size_t)N, &othersum);
	printf("%ld\n", othersum);
	printf("%ld\n", checksum);
	free(input);
	return 0;
}
