#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
	unsigned M, N;
	int rc;
	/*M is rows, N is columns*/
	rc = scanf("%u %u ", &M, &N);
	if(rc != 2) {
		perror("failed to read input");
		exit(-1);
	}
	/*allocate buffer for result*/
	unsigned long result;
	unsigned long* lookup;
	lookup = (unsigned long*)malloc(M*N*sizeof(unsigned long));
	if(lookup == NULL) {
		perror("allocation of lookup table failed");
		exit(-1);
	}
	unsigned ii, jj;
	char tmp;
	for(ii = 0; ii < M; ++ii) {
		for(jj = 0; jj < N; ++jj) {
			rc = scanf(" %c ", &tmp);
			if(rc != 1) {
				fprintf(stderr, "input line = %d\n", ii+2 );
				fflush(stderr);
				perror("input read error");
				exit(-1);
			}
			switch(tmp) {
				/*if first character is not an @ then we will access array out 
				* of bounds on later calls*/
				case '@':
					assert(ii == 0 && jj == 0);
					*lookup = 1;
					break;
				case 'X':
					lookup[jj + N*ii] = 0;
					break;
				default:
					if(ii == 0) {
						/*first row*/
						lookup[jj] = lookup[jj-1];
					} else if (jj == 0) {
						/*first column*/
						lookup[N*ii] = lookup[N*(ii-1)];
					} else {
						lookup[jj + N*ii] = lookup[jj-1 + N*ii] + lookup[jj + N*(ii-1)];
					}
			}
			/*printf("%ld ",lookup[jj + N*ii]);*/
		}
		/*printf("\n");*/
	}
	result = lookup[N*M -1];
	free(lookup);
	printf("%ld\n", result);
	return 0;
}
