#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
	/*play mastermind with 4 digit secret code*/
	int rc;
	unsigned N, secret;
	errno = 0;
	rc = scanf("%u %u", &secret, &N);
	if(rc != 2) {
		perror("failed to read first line of input");
		exit(-1);
	}
	/*buffers for storing the values, include room for null char*/
	char ds[5], dg[5];
	/*make sure that we do not have more than 4 digits*/
	assert( secret < 10000);
	errno = 0;
	rc = sprintf(ds, "%d", secret);
	if(rc < 0 || errno != 0) {
		perror("sprintf failed to separate digits of secret number");
		exit(-1);
	}

	unsigned ii, jj, guess, val1, val2;
	/*read N guesses of secrect code from command line*/
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf("%u", &guess);
		if(rc != 1) {
			fprintf(stderr, "Input read failed on line %d\n", ii +2);
			perror("");
			exit(-1);
		}
		errno = 0;
		rc = sprintf(dg, "%d", guess);
		if(rc < 0 || errno !=0) {
			perror("sprintf failed to separate digits of guess");
			exit(-1);
		}
		val1 = 0;
		val2 = 0;
		for(jj = 0; jj < 4; ++jj) {
			if(dg[jj] == ds[jj]) {
				++val1;
			} else {
				if(dg[jj] == ds[0]) {
					++val2;
				} else if(dg[jj] == ds[1]) {
					++val2;
				} else if(dg[jj] == ds[2]) {
					++val2;
				} else if(dg[jj] == ds[3]) {
					++val2;
				}
			}
		}
		printf("%d-%d ", val1, val2);
	}
	printf("\n");
	return 0;
}
