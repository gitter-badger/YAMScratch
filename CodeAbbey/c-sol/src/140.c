#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <gmp.h>

#define DEBUG 0

int main(int argc, char const *argv[])
{
	/*compute the number of Dyck words of length 2N, which
	* happens to be Catalan Numbers. Rest assured, we did not do
	* it the easy way here, instead only deriving the arithmatic recursion
	* and manually computing all iterations until we get to the desired*/
	unsigned N;
	int rc;
	unsigned ii, jj, kk;
	/*M is rows, N is columns*/
	rc = scanf(" %u ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	/*we cannot have zero elements so exit if N is zero*/
	if(N < 1) {
	}
	mpz_t* next, * prev, * swap_tmp;
	next = (mpz_t*)malloc(N * sizeof(mpz_t));
	if(next == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	} else {
		for(ii = 0; ii < N; ++ii) {
			mpz_init(next[ii]);
		}
	}
	prev = (mpz_t*)malloc(N * sizeof(mpz_t));
	if(prev == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	} else {
		/*initialize the array*/
		for(ii = 0; ii < N; ++ii) {
			mpz_init(prev[ii]);
		}
	}
	/*initalize the tree for lengths 1 is 0*/
	mpz_set_ui(prev[0], 0);
	mpz_set_ui(next[0], 0);
	/*build up the tree dangling ends in the loop*/
	if(N > 2) {
		mpz_t tmp;
		mpz_init(tmp);
		mpz_set_ui(next[1],1);
		for(ii = 2; ii < N; ++ii) {
			/*now swap the pointers*/
			swap_tmp = next;
			next = prev;
			prev = swap_tmp;
			/*for each terminating bucket of the tree
			* compute this iterations number of buckets*/
			for(jj = 1; jj < ii; ++jj) {
				/*sum up all eligible previous buckets*/
				mpz_set_ui(tmp, 0);
				for(kk = jj-1; kk < ii; ++kk) {
					mpz_add(tmp, tmp ,prev[kk]);
				}
				mpz_set(next[jj],tmp);
			}
			mpz_set_ui(next[ii], 1);
		}
		mpz_clear(tmp);
	} else if(N == 2) { /*trival cases*/
		mpz_set_ui(next[1], 1);
	} else if(N == 1) {
		mpz_set_ui(next[0], 1);
	}
	mpz_t running_sum;
	mpz_init(running_sum);
	mpz_set_ui(running_sum, 0);
	mpz_t cell_product;
	mpz_init(cell_product);
	for(ii = 0; ii < N; ++ii) {
		mpz_set_ui(cell_product, ii+1);
		mpz_mul(cell_product, cell_product, next[ii]);
		mpz_add(running_sum,running_sum, cell_product);
		if(DEBUG) {
			mpz_out_str(stdout, 10, next[ii]);
			printf(" ");
		}
	}
	if(DEBUG) {printf("\n");}
	mpz_out_str(stdout, 10, running_sum);
	printf("\n");
	mpz_clear(cell_product);
	mpz_clear(running_sum);
	/*clean up the arrays of mpz_t*/
	for(ii = 0; ii < N; ++ii) {
		mpz_clear(prev[ii]);
		mpz_clear(next[ii]);
	}
	free(next);
	free(prev);
	return 0;
}
