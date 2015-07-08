#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int compare_long(long* A, long* B) {
	return (*A < *B) ? 1 : 0;
}

extern signed selection_sort_long(long * array, size_t size, int (*cmp)(long *, long *)) { if(size < 2) { return 0; } if(array == ((void *)0)) { return -1; } long tmp; size_t ii, jj, max; for(ii = 1; ii < size; ++ii) { max = 0; for(jj = 1; jj <= size - ii; ++jj) { if((*cmp)(array+jj, array+max)) { } else { max = jj; } } printf("%ld ", max ); tmp = array[size-ii]; array[size-ii] = array[max]; array[max] = tmp; } return 0; }

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
	long* result;
	errno = 0;
	result = (long*)malloc(N * sizeof(long));
	if(result == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	}
	unsigned ii;
	long tmp;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%ld", &tmp);
		if(rc != 1) {
			perror("Error in reading input from command line");
			exit(-1);
		}
		result[ii] = tmp;
	}
	int (*less_than)(long*, long*);
	less_than = &compare_long;
	selection_sort_long(result, N, less_than);
	printf("\n");
	return 0;
}
