#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	unsigned N;
	int rc;
	rc = scanf("%u", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	/*allocate buffer for result*/
	char* result;
	result = (char*)calloc(N, sizeof(char));
	if(result == NULL) {
		perror("failed to allocate output buffer");
		exit(-1);
	}
	unsigned ii, A, B, C;
	long compare, hypotenuse;
	/*read in three lengths of the triangle*/
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%u %u %u ", &A, &B, &C);
		if(rc != 3) {
			fprintf(stderr, "input line = %d\n", ii+2 );
			fflush(stderr);
			perror("input read error");
			exit(-1);
		}
		/*
		* find the longest side of the triangle as the
		* as the hypotenuse canidate 
		*/
		if(C < B) {
			B ^= C;
			C ^= B;
			B ^= C;
		} if(C < A) {
			A ^= C;
			C ^= A;
			A ^= C;
		}
		/*force an integer promotion*/
		compare = A*A;
		compare += B*B;
		hypotenuse = C*C;
		if(hypotenuse == compare) {
			result[ii] = 'R';
		} else if(hypotenuse < compare) {
			result[ii] = 'A';
		} else if(hypotenuse > compare) {
			result[ii] = 'O';
		}
	}
	for(ii = 0; ii < N; ++ii) {
		printf("%c ", result[ii]);
	}
	printf("\n");
	return 0;
}
