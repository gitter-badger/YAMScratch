#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <complex.h>
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

double _Complex obj(double _Complex A, double _Complex B, double _Complex C, 
			double _Complex X, double _Complex Y) {
	double _Complex two;
	two = 2 + 0.0 * _Complex_I;
	double _Complex exponent;
	exponent = - cpow((X+A), two) - cpow((Y+B), two);
	return cpow((X-A), two) + cpow((Y-B), two) + C * cexp(exponent) ;
}

signed grad(double a, double b, double c, double x, double y,
			double* x_prime, double* y_prime) {
	/*internally use complex numbers for computation*/
	double _Complex A, B, C;
	A = a + 0.0 * _Complex_I;
	B = b + 0.0 * _Complex_I;
	C = c + 0.0 * _Complex_I;
	double h;
	h = 1e-10;
	double _Complex del_x, del_y, dfdx, dfdy, x_tmp, y_tmp;
	x_tmp = x + 0.0 * _Complex_I;
	y_tmp = y + 0.0 * _Complex_I;
	del_x = x + (h * _Complex_I);
	del_y = y + (h * _Complex_I);
	dfdx = obj(A, B, C, del_x, y_tmp);
	dfdy = obj(A, B, C, x_tmp, del_y);
	/*compute derivative of using complex step*/
	*x_prime = cimag(dfdx) / h;
	*y_prime = cimag(dfdy) / h;
	return 0;
}

int main(int argc, char const *argv[])
{
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	long* result;
	errno = 0;
	result = (long*)malloc(N * sizeof(long));
	if(result == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	}
	unsigned ii;
	double A, B, C, x_t, y_t, x_p, y_p, tmp;
	/*find the coefficients for the format*/
	rc = scanf(" %lf %lf %lf ", &A, &B, &C);
	if(rc != 3) {
		perror("failed to read coefficients");
		exit(-1);
	}
	/*find the direction of steepest descent for given points (x,y)*/
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%lf %lf", &x_t, &y_t);
		if(rc != 2) {
			perror("Error in reading input from command line");
			exit(-1);
		}
		grad(A, B, C, x_t, y_t, &x_p, &y_p);
		tmp = atan2(y_p , x_p) * (180 / M_PI);
		/*add 180 degrees to get direction of steepest descent, then round*/
		printf("%d ", (signed) (tmp + 180 + 0.5));
	}
	printf("\n");
	return 0;
}
