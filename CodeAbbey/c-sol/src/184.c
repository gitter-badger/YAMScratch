#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	errno = 0;
	unsigned long* result;
	result = (unsigned long*)malloc(N * sizeof(unsigned long));
	if(result == NULL) {
		perror("failed to allocate result buffer\n");
		exit(-1);
	}
	/*optimal strategy calculator for matches taking game
	* 'n' is normal play where player taking last matches wins
	* 'i' is inverted play where last match taker losses*/
	char game_mode;
	unsigned long M, K;
	unsigned long ii;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf(" %lu %lu %c ", &M, &K, &game_mode);
		if(rc != 3) {
			printf("Input read failure in input line\n" );
		}
		switch (game_mode) {
			/*zero matches is invalid game state*/
			assert(M > 0);
			case 'n':
				/*simple case where we can win first move*/
				if(K < M) {
					result[ii] = M % (K+1);
				} else {
					result[ii] = K;
				}
				break;
			case 'i':
				/*simple case where we can take all but last one on first move*/
				if(K+1 < M) {
					result[ii] = (M-1) % (K+1);
				} else {
					result[ii] = M-1;
				}
				break;
			default:
				/*this should never happen so indicate error and exit*/
				fprintf(stdout, "Game mode '%c' unrecognized, should be [n/i]\n", game_mode);
				fflush(stdout);
				exit(-1);
		}
	}
	/*print out the results*/
	for(ii = 0; ii < N; ++ii) {
		fprintf(stdout, "%lu ", result[ii]);
	}
	fprintf(stdout,"\n");
	free(result);
	return 0;
}
