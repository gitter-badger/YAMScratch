#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <math.h>

unsigned joe(unsigned N, unsigned K) {
	unsigned n, g;
	g = 0;
	for(n = 2; n <= N; ++n) {
		g = (g+K)%n;
	}
	return g;
}

int main(int argc, char* argv[]) {
	unsigned N, K;
	int rc;
	rc = scanf("%u %u", &N, &K);
	if(rc != 2 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}
	unsigned result;
	/*add one to correct for zero indexing*/
	result = joe(N, K) + 1;
	printf("%u\n", result);
	return 0;
}
