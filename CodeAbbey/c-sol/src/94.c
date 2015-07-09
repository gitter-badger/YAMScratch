#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

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
	unsigned* result;
	errno = 0;
	result = (unsigned*)malloc(N * sizeof(unsigned));
	if(result == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	}
	/*find the first fibonacci number divisible by A*/
	unsigned ii;
	unsigned long A;
	char terminator;
	for(ii = 0; ii < N; ++ii) {
		result[ii] = 0;
		errno = 0;
		for(;;) {
			/*keep scanning until we reach end of line*/
			rc = scanf("%lu[^\n]%c", &A, &terminator);
			printf("%lu %x %d\n", A, terminator, rc);
			if(rc != 2) {
				perror("matching on input failed");
				exit(-1);
			}
			result[ii] += (A * A);
			if(terminator == '\n') {
				/*here we will move on to next line*/
				break;
			}
		}
	}
	for(ii = 0; ii < N; ++ii) {
		printf("%d ",result[ii]);
	}
	printf("\n");
	free(result);
	return 0;
}
