#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

#define PI 3.14159265

int main(int argc, char const *argv[])
{
	/*find roots of quadratic equation*/
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	double x0, y0, x1, y1, ang_h, ang_m;
	unsigned ii, hour, minute;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf(" %u%*c%u", &hour, &minute);
		if(rc != 2 || errno != 0) {
			perror("inpute reading failed");
			exit(-1);
		}
		/*overflow rotation*/
		hour %= 12;
		ang_h = 90 - (30.0 * (double)minute / 60.0);
		ang_h -= 30.0 * (double)hour;
		ang_m = 90 - (6.0 * (double)minute);
		/*since reguarlly the */
		/*location of tip of minute hand r = 6 centered at (10, 10)*/
		x0 = 10.0 + 6.0 * cos(ang_h * PI / 180);
		y0 = 10.0 + 6.0 * sin(ang_h * PI / 180);
		x1 = 10.0 + 9.0 * cos(ang_m * PI/ 180);
		y1 = 10.0 + 9.0 * sin(ang_m * PI/ 180);
		printf("%15.15lf %15.15lf %15.15lf %15.15lf ", x0, y0, x1, y1);
	}
	printf("\n");
	return 0;
}
