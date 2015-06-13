#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/

/*bad version of quicksort to produce those bloody partitions
in the order they want them in*/
enum State {LEFT, RIGHT};

void bad_qsort(int * list, unsigned left, unsigned right) { 
	printf("%d-%d ",left, right);
	fflush(stdout); 
	unsigned ii, jj; 
	int pivot; 
	int tmp;
	ii = left;
	jj = right;
	enum State dir;
	dir = LEFT;
	pivot = list[left];
	while(ii < jj){ 
		if(dir == LEFT) {
			if(list[jj] > pivot) {
				--jj;
			} else {
				list[ii] = list[jj];
				++ii;
				dir = RIGHT;
			}
		} else {
			if(list[ii] < pivot) {
				++ii;
			} else {
				list[jj] = list[ii];
				--jj;
				dir = LEFT;
			}
		}
	}
	list[ii] = pivot;
	if(ii -left > 1) {
		bad_qsort(list, left, ii - 1); 
	}
	if(right - ii > 1 ) {
		bad_qsort(list, ii + 1, right); 
	}
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
	int* result = (int*)calloc(N, sizeof(int));
	unsigned ii;
	for(ii = 0; ii < N; ++ii) {
		scanf("%d", result+ii);
	}
	bad_qsort(result, 0, N-1);
	printf("\n");
	return 0;
}
