#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/*
			  state C
			{alan alive}
1 - pA - pB    /
 +-----+      / pA
 |     |     /
 +-> state A
	{both alive}
			\
			 \ pB
			  \
			  state B
			  {bob alive}

	| (1-pA-pB) 0  0 |  
M = |    pB     1  0 |
 	|    pA     0  1 |
*/

int main(int argc, char const *argv[])
{
	/*compute state of markov process*/
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	signed long A, B, C;
	unsigned ii, pA, pB;
	double [3][3] markov;

	for(ii = 0; ii < N; ++ ii) {
		errno = 0;
		rc = scanf(" %u %u", &pA, &pB);
		if(rc != 2 || errno != 0) {
			fprintf(stderr, "input read failed on line %u\n", ii+2);
			fflush(stderr);
			perror("");
			exit(-1);
		}
	}
	printf("\n");
	return 0;
}
