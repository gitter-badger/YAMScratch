#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <math.h>

#define ALLOWABLE_ERROR 1e-8

struct ObjectiveConstants {
	long double A;
	long double B;
	long double C;
	long double D;
};

long double objective(const struct ObjectiveConstants* constants, long double x) {
	long double result;
	result = constants->A * x;
	result += constants->B * sqrtl(pow(x, 3));
	result -= constants->C * exp((-x/ 50.0));
	return result - constants->D;
}

int main(int argc, char* argv[]) {
	unsigned N;
	int rc;
	rc = scanf("%u ", &N);
	if(rc != 1 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}
	long double* result = (long double*)calloc(N, sizeof(long double));
	if(result == NULL){
		perror("failed to allocate ouput array");
		exit(-1);
	}
	/*
	* find the solution to an objective function on the interval [0, 100]
	* using binary search
	*/
	unsigned ii;
	long double tmp, x, left, right;

	struct ObjectiveConstants K;
	for(ii = 0; ii < N; ++ii) {
		left = 0;
		right = 100;
		/*load this into fixed width*/
		rc = scanf("%Lf %Lf %Lf %Lf", &K.A, &K.B, &K.C, &K.D);
		if(rc != 4 || rc == 0) {
			perror("input read failure");
			exit(-1);
		}
		/*
		* A, B, C will be positive so the function will be monotonically 
		* increasing 
		*/
		do {
			x = left + ((right - left)/2.0);
			tmp = objective(&K, x);
			if( tmp < 0) {
				left = x;
			} else {
				right = x;
			}
		} while(fabsl(tmp) > ALLOWABLE_ERROR);
		result[ii] = x;
	}
	for(ii = 0; ii < N; ++ii){
		printf("%16.16Lf ", result[ii]);
	}
	printf("\n");
	return 0;
}
