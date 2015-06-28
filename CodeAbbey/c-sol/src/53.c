#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
	unsigned N;
	int rc;
	/*M is rows, N is columns*/
	rc = scanf("%u ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	char* result;
	result = (char*)malloc(N * sizeof(char));
	if(result == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	}
	unsigned ii;
	char r1,c1,r2,c2;
	for(ii = 0; ii < N; ++ii) {
		rc = scanf("%c%c %c%c ", &c1, &r1, &c2, &r2);
		if(rc != 4) {
			perror("input read error");
			exit(-1);
		}
		result[ii] = 'Y';
		if(c1 == c2) {
		} else if (r1 == r2) {
			/*they are on the same row*/
		} else if(abs(r2-r1) == abs(c2-c1)) {
			/*they are on a diagonal*/
		} else {
			result[ii] = 'N';
		}
	}
	for(ii = 0; ii < N; ++ii) {
		printf("%c ",result[ii]);
	}
	printf("\n");
	free(result);
	return 0;
}
