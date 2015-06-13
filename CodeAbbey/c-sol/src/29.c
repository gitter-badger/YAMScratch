#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <assert.h>

#include "yam_sorting.h"

QUICKSORT_INIT(int)

unsigned less_than(int* a, int* b) {
	return (*a < *b) ? 1 : 0;
}

int main(int argc, char* argv[]) {
	unsigned (*cmp_ptr)(int*, int*);
	cmp_ptr = &less_than;
	unsigned N;
	int rc;
	rc = scanf("%u ", &N);
	if(rc != 1 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}
	int* result = (int*)calloc(N, sizeof(unsigned));
	unsigned ii;
	for(ii = 0; ii < N; ++ii) {
		scanf("%d", result+ii);

	}
	yam_quicksort(int, result, 0, N-1, cmp_ptr);
	for(ii = 0; ii < N; ++ii){
		printf("%u ", result[ii]);
	}
	printf("\n");
	return 0;
}
