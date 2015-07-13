#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <gmp.h>

signed binomial_coeff(mpz_t* N, mpz_t* K, mpz_t* result) {
	/*some sanity checks*/
	if(mpz_cmp_ui(*K, 0) == 0) {
		mpz_set_ui(*result, 1);
		return 0;
	}
	/*make sure N and K are both positive*/
	if( mpz_cmp_ui(*N, 0) < 0) {
		return -1;
	}
	if( mpz_cmp_ui(*K, 0) < 0) {
		return -1;
	}
	mpz_t* A, * T, * L, * S;
	A = malloc(3 * sizeof(mpz_t));
	T = &A[0];
	L = &A[1];
	S = &A[2];
	mpz_init(*T); /*numerator (top)*/
	mpz_init(*L); /*larger of denominator*/
	mpz_init(*S); /*smaller of denominator*/
	/*use T as a temporary value to determine if (N-K) or K is L*/
	mpz_set(*L, *K);
	mpz_set(*T, *N);
	/*set S to N-K*/
	mpz_sub(*S, *T, *L);
	/*if S is acutally > Ls, swap the two*/
	if(mpz_cmp(*S, *L) > 0) {
		mpz_t* tmp;
		tmp = L;
		L = S;
		S = tmp;
	}
	/* compute the factorial of the numerator which is (N!/L!)*/
	mpz_set_ui(*result, 1);
	for(;mpz_cmp(*T, *L) > 0; mpz_sub_ui(*T, *T, 1)) {
		mpz_mul(*result, *result, *T);
	}
	/*compute S!, storing it in T*/
	mpz_set_ui(*T, 1);
	for(;mpz_cmp_ui(*S, 1) > 0; mpz_sub_ui(*S, *S, 1)) {
		mpz_mul(*T, *T, *S);
	}
	/*divide the numerator by S!*/
	mpz_cdiv_q(*result, *result, *T);
	mpz_clear(*T);
	mpz_clear(*L);
	mpz_clear(*S);
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
