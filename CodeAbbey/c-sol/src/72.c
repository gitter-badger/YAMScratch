#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "code_abbey_utility.h"

/*BUFF SIZE must be at least 2 to allow for null termination*/
#define BUFF_SIZE 100

int main(int argc, char const *argv[])
{
	unsigned long N, X0;
	int rc;
	errno = 0;
	rc = scanf(" %lu %lu ", &N, &X0);
	if(rc != 2) {
		perror("failed to read input");
		exit(-1);
	}
	char* buffer;
	errno = 0;
	buffer = (char*)malloc(BUFF_SIZE * sizeof(char));
	if(buffer == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	}
	char* consonants, * vowels;
	consonants = "bcdfghjklmnprstvwxz";
	vowels = "aeiou";
	/*seed the linear generator*/
	unsigned long A, C, M;
	A = 445UL;
	C = 700001UL;
	M = 2097152UL;
	unsigned ii, jj, K;
	unsigned long Xcurr;
	Xcurr = X0;
	unsigned flag;
	for(ii = 0; ii < N; ++ii) {
		flag = 1;
		errno = 0;
		rc = scanf(" %u ", &K);
		if(rc != 1) {
			fprintf(stderr, "input read error at line %u\n", ii +2 );
			fflush(stderr);
			perror("errno: ");
			exit(-1);
		}
		/*process chunks until we have produced K chars*/
		for(;K > 0;) {
			/*walk over buffer generating the characters*/
			for(jj = 0; (jj < BUFF_SIZE -1) && K > 0; ++jj, --K) {
				Xcurr = linear_congruent_gen(A, C, M, Xcurr);
				//printf("%lu\n", Xcurr);
				if(flag == 1) {
					buffer[jj] = consonants[Xcurr%19];
				} else {
					buffer[jj] = vowels[Xcurr%5];
				}
				flag = (flag) ? 0 : 1;
			}
			/*null terminate the buffer*/
			buffer[jj] = '\0';
			printf("%s", buffer);
		}
		/*print a space separator*/
		printf(" ");
	}
	printf("\n");
	free(buffer);
	return 0;
}
