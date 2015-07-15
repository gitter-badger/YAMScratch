#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	/*convert card value to index*/
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	char* suites[4] = {"Clubs", "Spades", "Diamonds", "Hearts"};
	char* ranks[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
	unsigned ii, C;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf(" %u ", &C);
		printf("%s-of-%s ",ranks[C%13], suites[C/13]);
	}
	printf("\n");
	return 0;
}
