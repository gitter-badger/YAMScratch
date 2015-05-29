#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <math.h>

unsigned GCD(unsigned A, unsigned B) {
	while(A != 0 && B != 0) {
		if(A > B) {
			A %= B;
		} else {
			B %= A;
		}
	}
	if(A) return A;
	else return B;
}

int main(int argc, char* argv[]) {
	unsigned N;
	int rc;
	rc = scanf("%u", &N);
	if(rc != 1 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}
	unsigned* gcd, *lcm;
	gcd = (unsigned*)calloc(N, sizeof(unsigned));
	if(gcd == NULL) {
		perror("Failed to allocate result buffer.");
		exit(-1);
	}
	lcm = (unsigned*)calloc(N, sizeof(unsigned));
	if(lcm == NULL) {
		perror("Failed to allocate result buffer.");
		exit(-1);
	}
	/*find LCM and GCD*/
	unsigned ii, a, b;
	long tmp;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf(" %u %u ", &a, &b );
		if(rc != 2 || !rc) {
			if(!errno) errno = EINVAL;
			fprintf(stderr,"Input error on line %d\n", ii+2);
			perror("Did not find two integers");
			exit(-1);
		}
		gcd[ii] = GCD(a, b);
		tmp = a * b;
		lcm[ii] = tmp / gcd[ii];
	}
	for(ii = 0; ii < N; ++ii) {
		printf("(%u %u) ", gcd[ii], lcm[ii]);
	}
	printf("\n");
	return 0;
}
