#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <math.h>

#define SECS_PER_DAY

int main(int argc, char* argv[]) {
	unsigned N;
	int rc;
	rc = scanf("%u", &N);
	if(rc != 1 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}
	unsigned* result;
	result = (unsigned*) calloc(N, sizeof(unsigned));
	if(result == NULL) {
		perror("Failed to allocate result buffer.");
		exit(-1);
	}
	/*read in the array*/
	unsigned ii;
	int day1, day2, hour1, hour2, min1, min2, sec1, sec2;
	int day3, hour3, min3, sec3;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%d %d %d %d %d %d %d %d", &day1, &hour1, &min1, &sec1, &day2, &hour2, &min2, &sec2);
		if(rc != 8 || !rc) {
			if(!errno) errno = EINVAL;
			perror("Enter failed to read integer");
			exit(-1);
		}
		sec3 = sec2- sec1;
		min3 = min2 - min1;
		hour3 = hour2 - hour1;
		day3 = day2 - day1;
		if(sec3 < 0) {
			sec3 += 60;
			min3--;
		}
		if(min3 < 0) {
			min3 += 60;
			hour3--;
		}
		if(hour3 < 0) {
			hour3 += 24;
			day3--;
		}
		if(day3 < 0) {
			errno = EINVAL;
			perror("second time stamp should be later than first timestamp");
			exit(-1);
		}
		printf("(%d %d %d %d) ", day3, hour3, min3, sec3);
	}
	printf("\n");
	return 0;
}
