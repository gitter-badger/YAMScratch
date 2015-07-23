#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

#include "yam_vector.h"
VECTOR_INIT(char)
/*use the vector to hold the variable length input from command line*/

int main(int argc, char const *argv[])
{
	/*rotate string by slice*/
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	unsigned long K, len;
	unsigned ii;
	Vector_t(char)* buffer;
	buffer = newVector(char);
	/*reserve reasonable amount of space*/
	vector_resize(char, buffer, 128);

	char c;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf(" %u ", &K);

		for(;;) {
			errno = 0;
			c = getc(stdin);

			if(c == '\n') {
				break;
			} else if(c == EOF){
				break;
			}
		}
		printf("%lu ", K);
	}
	printf("\n");
	return 0;
}
