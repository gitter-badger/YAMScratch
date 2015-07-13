#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
	/*number of solutions to girls and pigs problem
	* given N legs and M breasts, girls have 2 legs and
	* 2 breasts, while pigs have 4 legs and K breasts*/
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	unsigned ii, L, B, K, solutions;
	for(ii = 0; ii < N; ++ii) {
		solutions = 0;
		errno = 0;
		rc = scanf("%u %u", &L, &B);
		if(rc != 2) {
			fprintf(stderr, "input read error on line %d\n", ii+2);
			fflush(stderr);
			perror("");
			exit(-1);
		}
		/*one solution is always possible*/
		assert(B >= L);
		/*number of legs must be even*/
		assert(L%2 == 0);
		/*generate all possible numbers based on legs*/
		unsigned G, P;
		/*we must skip case when there are 0 girls watching the pigs*/
		if((L/2)%2) {
			G = 1;
			P = L/4;
		} else {
			G = 2;
			P = L/4-1;
		}
		for(; P > 0;) {
			/*now sift for values that solve breast equations*/
			K = B - G*2;
			if(K%P == 0) {
				K /= P;
				/*make sure that number of breasts on pigs is even*/
				if(K%2 == 0) {
					++solutions;
				}
			}
			--P;
			G += 2;
		}
		printf("%u ", solutions);
	}
	printf("\n");
	return 0;
}
