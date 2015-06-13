#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <assert.h>

#include "yam_sorting.h"

typedef struct
{
	int val;
	unsigned index;
} IndexStore ;

QUICKSORT_INIT(IndexStore)

unsigned less_than(IndexStore* a, IndexStore* b) {
	return (a->val < b->val) ? 1 : 0;
}

int main(int argc, char* argv[]) {
	unsigned (*cmp_ptr)(IndexStore*, IndexStore*);
	cmp_ptr = &less_than;
	unsigned N;
	int rc;
	rc = scanf("%u ", &N);
	if(rc != 1 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}
	IndexStore* result = (IndexStore*)calloc(N, sizeof(IndexStore));
	unsigned ii, tmp;
	for(ii = 0; ii < N; ++ii) {
		scanf("%d", &(result[ii].val));
		result[ii].index = ii + 1;

	}
	yam_quicksort(IndexStore, result, 0, N-1, cmp_ptr);
	for(ii = 0; ii < N; ++ii){
		printf("%u ", result[ii].index);
	}
	printf("\n");
	return 0;
}
