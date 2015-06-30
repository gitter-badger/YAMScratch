#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "parseCodeAbbeyInput.h"

VECTOR_INIT(char)


int main(int argc, char const *argv[])
{
	
	unsigned long N;
	int rc;
	unsigned ii, jj, kk;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}

	Vector_t(char)* next, * prev;
	/*the base case of the L system*/

	/*generate the program recursively by switching back and forth*/
	return 0;
}
