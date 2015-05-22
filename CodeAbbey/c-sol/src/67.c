#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <math.h>

#include "parseCodeAbbeyInput.h"
#include "wrapped.h"
#include "yam_big_int.h"

/*hack to get around macro tokenization*/
typedef BigInt* BigInt_ptr;
VECTOR_INIT(BigInt_ptr)

int main(int argc, char* argv[]) {
	unsigned N;
	int rc;
	rc = scanf("%u", &N);
	if(rc != 1 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}

	Vector_t(BigInt_ptr)* fib_memo = newVector(BigInt_ptr);
	/*setup our parser template*/
	size_t nbytes;
	ssize_t bytes_read;
	char* lineptr;
	unsigned ii;
	BigInt* fib_num;
	for(ii = 0; ii < N; ++ii) {
		lineptr = NULL;
		nbytes = 0;
		errno = 0;
		bytes_read = wrap_getline(&lineptr, &nbytes, stdin);
		if(bytes_read <= 0) {
			perror("invalid input, nothing on line");
			exit(-1);
		}
		fib_num = NULL;
		fib_num = newBigInt;
		
		printf("Input: %s\n",lineptr);
		free(lineptr);
	}
	/*clean up the vector of fibonacci numbers*/
	for(ii = 0; ii < fib_memo->elms; ++ii) {
	}
	return 0;
}
