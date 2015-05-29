#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <math.h>
#include <assert.h>

#include "parseCodeAbbeyInput.h"
#include "wrapped.h"
#include "yam_big_int.h"
#include "yam_fibonacci.h"

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

	Vector_t(BigInt_ptr)* fib_memo = init_fib_memo();
	unsigned* result;
	result = (unsigned*)calloc(N, sizeof(unsigned));
	/*setup our parser template*/
	size_t nbytes;
	ssize_t bytes_read;
	char* lineptr;
	unsigned ii;
	signed index;
	BigInt* fib_num;
	/*read each fibonacci number in*/
	for(ii = 0; ii < N; ++ii) {
		lineptr = NULL;
		nbytes = 0;
		errno = 0;
		bytes_read = wrap_getline(&lineptr, &nbytes, stdin);
		if(bytes_read <= 0) {
			perror("invalid input, nothing on line");
			exit(-1);
		}
		fib_num = newBigInt;
		big_int_from_str(fib_num, lineptr);
		index = find_fibonacci_index(fib_num, fib_memo);
		/*if it does not exist yet, fill in the memo up to this one*/
		if(index < 0) {
			fill_in_up_to_fibonacci(fib_num, fib_memo);
			index = fib_memo->elms - 1;
		}
		/*printf("%s",lineptr); */
		result[ii] = index;
		free(lineptr);

	}
	for(ii = 0; ii < N; ++ii) {
		printf("%d ", result[ii]);
	}
	printf("\n");
	BigInt_destroy(fib_num);
	/*cleans up vector of BigInts*/
	destroy_fib_memo(fib_memo);
	return 0;
}
