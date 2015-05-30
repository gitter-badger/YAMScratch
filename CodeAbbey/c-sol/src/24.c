#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <assert.h>

unsigned joe(unsigned N, unsigned K) {
	unsigned n, g;
	g = 0;
	for(n = 2; n <= N; ++n) {
		g = (g+K)%n;
	}
	return g;
}
#define UPPER_LIMIT 10000

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
	unsigned ii, tmp, accum;
	for(ii = 0; ii < N; ++ii) {
		tmp = 0;
		/*allocate an array to hold all 4 digit keys*/
		unsigned* keys = (unsigned*)calloc(UPPER_LIMIT, sizeof(unsigned));
		scanf("%u", &accum);
		assert(accum < UPPER_LIMIT);
		while(keys[accum] == 0) {
			keys[accum]++;
			++tmp;
			accum *= accum;
			/*truncate the first and last two digits*/
			accum %= 1000000;
			accum /= 100;
		}
		result[ii] = tmp;
	}
	for(ii = 0; ii < N; ++ii){
		printf("%u ", result[ii]);
	}
	printf("\n");
	return 0;
}
