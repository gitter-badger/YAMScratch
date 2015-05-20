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
	/*find average of array*/
	unsigned ii, kk, tmp, running_sum;
	for(ii = 0; ii < N; ++ii) {
		kk = 0;
		running_sum = 0;
		rc = scanf("%u", &tmp);
		if(rc != 1) {
			perror("Error in reading input from command line");
			exit(-1);
		}
		while(tmp) {
			running_sum += tmp;
			kk++;
			rc = scanf("%u", &tmp);
			if(rc != 1) {
				perror("Error in reading input from command line");
				exit(-1);
			}
		}
		result[ii] = (unsigned)((double) running_sum / (double) kk + 0.5 );
	}

	for(ii = 0; ii < N; ++ii){
		printf("%d ", result[ii]);
	}
	printf("\n");
	return 0;
}
