#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "yam_sorting.h"

QUICKSORT_INIT(unsigned)

#define LEN 5

unsigned less_than(unsigned* a, unsigned* b) {
	return (*a < *b) ? 1 : 0;
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
	unsigned (*less)(unsigned*, unsigned*);
	less = &less_than;
	unsigned ii, jj, A, B, C, D, E;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf(" %u %u %u %u %u", &A, &B, &C, &D, &E);
		if (rc != 5) {
			fprintf(stderr, "failed to read input line %u\n", ii+2);
			fflush(stderr);
			perror("");
			exit(-1);
		}
		unsigned rolls[LEN] = {A, B, C, D, E};
		/*sort the inputs*/
		yam_quicksort(unsigned, rolls, 0, LEN-1, less);
		unsigned accum, straight, pairs, threes, fours ;
		accum = 0;
		straight = 1;
		pairs = 0;
		threes = 0;
		fours = 0;
		for(jj = 1; jj < LEN; ++jj){
			if(rolls[jj] == rolls[jj-1]) {
				straight = 0;
				++accum;
			} else {
				if(rolls[jj] < rolls[jj-1]) {
					straight = 0;
				}
				if(accum == 1) {
					pairs++;
				} else if(accum == 2) {
					threes++;
				} else if(accum == 3) {
					fours++;
				}
				accum = 0;
			}
		}
		if(accum == 1) {
			pairs++;
		} else if(accum == 2) {
			threes++;
		} else if(accum == 3) {
			fours++;
		}
		if(straight) {
			if(rolls[0] == 2) {
				printf("big-straight ");
			} else {
				printf("small-straight ");
			}
		} else {
			/*check for pairs, threes houses and yacht*/
			if(accum == 4) {
				printf("yacht ");
			} else if(pairs == 2) {
				printf("two-pairs ");
			} else if(pairs == 1 && threes ==1) {
				printf("full-house ");
			} else if(pairs == 1) {
				printf("pair ");
			} else if(threes == 1) {
				printf("three ");
			} else if(fours == 1) {
				printf("four ");
			} else {
				printf("none ");
			}
		}
	}
	printf("\n");
	return 0;
}
