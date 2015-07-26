#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/

#define STEPS 100

int main(int argc, char* argv[]) {
	/*compute the location of a string of length L
	* bouncing around in a terminal of width W and height H*/
	int rc;
	unsigned W, H, L;
	errno = 0;
	rc = scanf(" %u %u %u ", &W, &H, &L);
	if(rc != 3 || errno != 0) {
		perror("input read failed on first line");
		exit(-1);
	}
	unsigned ii, X_pos, Y_pos, x_flag, y_flag;
	/*starting location, assume this is valid*/
	X_pos = 0;
	Y_pos = 0;
	/*use flags to keep track of velocity*/
	x_flag = 0; /*movind right*/
	y_flag = 0; /*moving down*/
	/*output the original postion*/
	printf("%d %d ", X_pos, Y_pos);
	for(ii = 0; ii < STEPS; ++ii) {
		if((x_flag)) {
			if(X_pos > 0) {
				--X_pos;
			} else {
				x_flag = 0;
				++X_pos;
			}
		} else {
			/*x_flag is zero and moving right*/
			if(X_pos < (W - L)) {
				++X_pos;
			} else {
				x_flag = 1; /*start moving to the left*/
				--X_pos;
			}
		}
		/*update Y component*/
		if((y_flag)) {
			if(Y_pos > 0) {
				--Y_pos;
			} else {
				y_flag = 0;
				++Y_pos;
			}
		} else {
			/*y_flag is zero and moving down */
			if(Y_pos < H - 1 ) {
				++Y_pos;
			} else {
				y_flag = 1; /*start moving  up*/
				--Y_pos;
			}
		}
		printf("%d %d ", X_pos, Y_pos);
	}
	printf("\n");
	return 0;
}
