#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	/*find roots of quadratic equation*/
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	signed long A, B, C;
	unsigned ii;
	for(ii = 0; ii < N; ++ii) {
		
	}
	printf("\n");
	return 0;
}
