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
	/*find average of array*/
	unsigned ii;
	long tmp, checksum;
	checksum = 0;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%lu", &tmp);
		if(rc != 1) {
			perror("Error in reading input from command line");
			exit(-1);
		}
		checksum += tmp;
		checksum *= 113;
		checksum %= MODULO;
	}
	printf("%ld\n", checksum);
	return 0;
}
