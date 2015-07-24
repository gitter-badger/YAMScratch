#define _GNU_SOURCE
#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <string.h> /*provides strsep*/


#include "wrapped.h"

#define BASE 10

int main(int argc, char* argv[]) {
	unsigned N;
	int rc;
	rc = scanf("%u", &N);
	if(rc != 1 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}
	/*set up the result array, we are sure of N > 0*/
	unsigned* result;
	result = (unsigned*)malloc(sizeof(unsigned)*N);
	if(result == NULL) {
		perror("Failed to allocate result buffer.");
		exit(-1);
	}
	unsigned ii, jj, A, B, C, exponent;
	long base_power, tmp;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%u %u %u", &A, &B, &C);
		if(rc != 3) {
			perror("Error in reading input from command line");
			exit(-1);
		}
		tmp = A * B + C;
		/*find the mod to seperate the digits*/
		exponent = 0;
		base_power = 1;
		while( base_power <= tmp) {
			base_power *= BASE;
			exponent++;
		}
		unsigned running_sum;
		running_sum = 0;
		/*we sweep from the 10^(n+1) place to the 10^(1) place*/
		for(jj = exponent; jj > 0; --jj) {
			base_power /= BASE;
			running_sum += (tmp / base_power);
			tmp = tmp % base_power;
		}
		result[ii] = running_sum;
	}

	for(ii = 0; ii < N; ++ii){
		printf("%d ", result[ii]);
	}
	printf("\n");
	return 0;
}
