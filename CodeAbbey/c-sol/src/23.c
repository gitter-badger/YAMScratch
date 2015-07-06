#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "yam_vector.h"
VECTOR_INIT(long)
#include "code_abbey_utility.h"

int main(int argc, char const *argv[])
{
	int rc;
	long tmp;
	Vector_t(long)* result;
	result = newVector(long);
	unsigned ii, swaps;
	ii = 0;
	swaps = 0;
	for(;;) {
		errno = 0;
		rc = scanf(" %ld", &tmp);
		if(rc != 1) {
			fprintf(stderr, "input read error\n");
			fflush(stderr);
			perror("errno: ");
			exit(-1);
		}
		if(tmp == -1) {
			/*signal that we have reached end of input*/
			break;
		}
		vector_push_back(long, result, tmp);
		/*now check if we would swap the array*/
		if(ii > 0) {
			if(result->items[ii-1] > result->items[ii]) {
				++swaps;
				tmp = result->items[ii];
				result->items[ii] = result->items[ii-1];
				result->items[ii-1] = tmp;
 			}
		}
		++ii;
	}
	printf("%d ", swaps);
	long checksum;
	array_checksum(result->items, result->elms, &checksum);
	printf("%ld\n", checksum);
	return 0;
}
