#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <gmp.h>

signed binomial_coeff(mpz_t* N, mpz_t* K, mpz_t* result) {
	/*some sanity checks*/
	mpz_set_ui(*result, 1);
	if(mpz_cmp_ui(*K, 0) == 0) {
		return 0;
	}
	/*make sure N and K are both positive*/
	if( mpz_cmp_ui(*N, 0) < 0) {
		return -1;
	}
	if( mpz_cmp_ui(*K, 0) < 0) {
		return -1;
	}
	mpz_t* A, * cN, * cK, * I;
	A = malloc(3 * sizeof(mpz_t));
	cN = &A[0];
	cK = &A[1];
	I = &A[2];
	mpz_init(*cN);
	mpz_init(*cK);
	mpz_init(*I);
	mpz_set(*cN, *N);
	mpz_set(*cK, *K);
	mpz_set_ui(*result, 1);
	for(mpz_set_ui(*I, 1); mpz_cmp(*K, *I) >= 0; mpz_add_ui(*I, *I, 1)) {
		mpz_mul(*result, *result, *cN);
		mpz_cdiv_q(*result, *result, *I);
		mpz_sub_ui(*cN, *cN, 1);
	}
	mpz_clear(*cN);
	mpz_clear(*cK);
	mpz_clear(*I);
	free(A);
	return 0;
}

int main(int argc, char const *argv[])
{
	/*encrypt a message with casear shift of K*/
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	mpz_t M, K, result;
	mpz_init(M);
	mpz_init(K);
	mpz_init(result);
	unsigned long ii, m, k;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf("%lu %lu", &m, &k);
		if(rc != 2) {
			fprintf(stderr, "input read failure on line %lu\n", ii +2);
			fflush(stderr);
			perror("");
			exit(-1);
		}
		mpz_set_ui(M, m);
		mpz_set_ui(K, k);
		mpz_set_ui(result, 0);
		binomial_coeff(&M, &K, &result);
		mpz_out_str(stdout, 10, result); /*print to stdout in base 10*/
		printf(" ");
	}
	printf("\n");
	mpz_clear(M);
	mpz_clear(K);
	mpz_clear(result);
	return 0;
}
