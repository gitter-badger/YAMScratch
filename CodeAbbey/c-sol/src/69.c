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
	unsigned ii, jj;
	unsigned long A, fib_n_2, fib_n_1, fib_n;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf(" %lu ", &A);
		if(rc != 1) {
			fprintf(stderr, "input read error at line %u\n", ii +2 );
			fflush(stderr);
			perror("errno: ");
			exit(-1);
		}
		if(A == 0) {
			result[ii] = 0;
			continue;
		} else if(A == 1) {
			result[ii] = 1;
			continue;
		}
		/*base case, it is impossible for us to have a number less than 1*/
		fib_n_2 = 0;
		fib_n_1 = 1;
		jj = 1;
		do {
			fib_n = fib_n_1 + fib_n_2;
			fib_n %= A;
			/*update the reccurance*/
			fib_n_2 = fib_n_1;
			fib_n_1 = fib_n;
			++jj;
		} while(fib_n);
		result[ii] = jj;
	}

	for(ii = 0; ii < N; ++ii) {
		printf("%d ",result[ii]);
	}
	printf("\n");
	free(result);
	return 0;
}
