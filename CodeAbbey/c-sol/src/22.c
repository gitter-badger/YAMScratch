#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

int main(int argc, char const *argv[])
{
	/*compute the time needed to print M papers using
	two printers with different speeds X and Y*/
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	unsigned long X, Y, M, K, delX, delY;
	unsigned ii;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf(" %lu %lu %lu ", &X, &Y, &M);
		if(rc != 3 || errno != 0) {
			fprintf(stderr, "input read failed on line %u\n", ii+2);
			fflush(stderr);
			perror("");
			exit(-1);
		}
		/*we produce papers at rate 1/X + 1/Y,
		* which gives us this expression for papers produced
		* t = M * X * Y /(X+Y)
		* k is the ceiling of the solution of this expression,
		* we then seek to find the smalled time after K that is evenly divided
		* by X or Y*/
		if( (M* X* Y) % (X+Y) == 0) {
			K = (M * X * Y) / (X+Y);
		} else {
			K = (int)ceil( ((double)(M * X* Y))/((double)(X+Y)));
			/*check for the case where K is exactly */
			delX = K % X;
			if(delX) delX = X - delX;
			delY = K % Y;
			if(delY) delY = Y - delY;
			/*add the minimum of the times to finish the last document*/
			K += (delX < delY) ? delX : delY;
		}
		printf("%lu ", K);
	}
	printf("\n");
	return 0;
}
