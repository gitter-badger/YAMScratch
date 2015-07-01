#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

/*convienece struct to hold each term of L-system
*compose of A and B with
* A -> AB and B -> A where 
* A is one step forward and no change in dir (0,0) -> (0,1) dir -> dir
* B is one step back and to right, then turn CCW 90 degrees (0,0) -> (1,-1) dir -> (dir+3)%4*/
struct Lsystem {
	signed x;
	signed y;
	signed dir;
	unsigned index;
	unsigned long len;
};

signed calc_x_y(struct Lsystem* L_i_1, struct Lsystem* L_i_2,
			  unsigned* X, unsigned* Y) {
	/*transform the coordinates by appropriate directions*/
	switch(L_i_1->dir) {
		case 0:
			*X = L_i_1->x + L_i_2->x;
			*Y = L_i_1->y + L_i_2->y;
			break;
		case 1:
			*X = L_i_1->x + L_i_2->y;
			*Y = L_i_1->y - L_i_2->x;
			break;
		case 2:
			*X = L_i_1->x - L_i_2->x;
			*Y = L_i_1->y - L_i_2->y;
			break;
		case 3:
			*X = L_i_1->x - L_i_2->y;
			*Y = L_i_1->y + L_i_2->x;
			break;
		default: /*should never reach here*/
			return (-1);
			break;
	}
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
	/*upper limit is length of program is L_59, if N is larger then
	* we will just use modulo and proceed with calculating the
	* remainder
	*/
	/*if there are more battery steps than length of program*/
	#define LENGTH_L_59 2504730781961
	unsigned long T; /*number of program repeats*/
	T = 0;
	if(LENGTH_L_59 < N) {
		T = N / LENGTH_L_59;
		N = N % LENGTH_L_59;
	}
	struct Lsystem* L_s;
	L_s = (struct Lsystem*)malloc(60 * sizeof(struct Lsystem));
	if(L_s == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	}
	/*base cases for L_0 and L_1*/
	L_s[0].x = 0;
	L_s[0].y = 1;
	L_s[0].dir = 0;
	L_s[0].index = 0;
	L_s[0].len = 1;
	L_s[1].x = 1;
	L_s[1].y = 0;
	L_s[1].dir = 3;
	L_s[1].index = 1;
	L_s[1].len = 2;
	unsigned ii, jj;
	unsigned long R;
	unsigned X_p, Y_p, dir_p;
	/*generate the rest of the L_systems up to L_59 if necessary*/
	for(ii = 2; ii < 60; ++ii) {
		L_s[ii].index = ii;
		L_s[ii].len = L_s[ii-1].len + L_s[ii-2].len;
		rc = calc_x_y(&L_s[ii-1], &L_s[ii-2], &X_p, &Y_p);
		L_s[ii].x = X_p;
		L_s[ii].y = Y_p;
		L_s[ii].dir = (L_s[ii-1].dir + L_s[ii-2].dir)%4;
		if(N <= L_s[ii].len) {
			R = N - L_s[ii-1].len;
			break;
		}
	}
	/*now use global to keep track of where robot is*/
	struct Lsystem tmp_L_sys;
	tmp_L_sys.x = 0;
	tmp_L_sys.y = 0;
	tmp_L_sys.dir = 0;
	if(R != 0) {
		/*when R != 0 it means that L_s[ii].len > N,
		* therefore we move back one index to find where
		* L_s[ii-1].len < N so we can approach from below*/
		--ii;
	}
	do {
		rc = calc_x_y(&tmp_L_sys, &L_s[ii], &X_p, &Y_p);
		tmp_L_sys.x = X_p;
		tmp_L_sys.y = Y_p;
		tmp_L_sys.dir = (tmp_L_sys.dir + L_s[ii].dir) % 4;
		T = (T != 0) ? (T - 1) : 0;
	} while(T != 0);
	/*drive the remainder to zero*/
	while(R != 0) {
		/*
		* ii will be used to represent the greatest present so far
		* we will approach from left side, so the remainder will
		* always be postive decreasing*/
		jj = ii - 1;
		/*search downwards until we find the biggest L system to
		* eat up the remainder */
		while(L_s[jj].len > R) {
			--jj;
		}
		R -= L_s[jj].len;
		/*update the position*/
		rc = calc_x_y(&tmp_L_sys, &L_s[jj], &X_p, &Y_p);
		tmp_L_sys.x = X_p;
		tmp_L_sys.y = Y_p;
		tmp_L_sys.dir = (tmp_L_sys.dir + L_s[jj].dir) % 4;
	}
	/*print the answer to the position where the robot is when it dies*/
	printf("%d %d\n",tmp_L_sys.x, tmp_L_sys.y);
	free(L_s);
	return 0;
}
