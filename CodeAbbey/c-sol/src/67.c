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
	/*be sure to each up the trailing whitespace on first line*/
	rc = scanf("%u ", &N);
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
		big_int_from_str(fib_num, lineptr);
		char* new_rep;
		new_rep = NULL;
		size_t bytes_written;
		bytes_written = big_int_to_str(&new_rep, fib_num);

		printf("%s\n", new_rep );
		printf("%s",lineptr);
		free(lineptr);
		free(new_rep);
		vector_push_back(BigInt_ptr, fib_memo, fib_num);
	}
	/*clean up the vector of fibonacci numbers*/
	for(ii = 0; ii < fib_memo->elms; ++ii) {
		BigInt_destroy(fib_memo->items[ii]);
	}
	vector_destroy(BigInt_ptr, fib_memo);
	return 0;
}
