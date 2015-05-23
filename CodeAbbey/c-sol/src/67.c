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

/*hack to get around macro tokenization*/
typedef BigInt* BigInt_ptr;
VECTOR_INIT(BigInt_ptr)

#define DELIMETER ' '

void compute_fibonacci(BigInt* fib_n, Vector_t(BigInt_ptr)* fib_memo) {
	/*the biggest element is pushed to the back*/
	while( ! big_int_equality(fib_n, fib_memo->items[fib_memo->elms-1])) {
		BigInt* fib_next;
		fib_next = newBigInt;
		
		vector_push_back(BigInt_ptr, fib_memo, fib_next);
	}
}

unsigned find_fibonacci_index(BigInt* fib_n, Vector_t(BigInt_ptr)* fib_memo) {
	/*check if the input is in the memo structure already by binary search*/
	unsigned memo_index, found;
	found = 0;
	unsigned left, right;
	left = 0;
	right = fib_memo->elms - 1;
	while(right >= left){

	}
	if(found) {
		return memo_index;
	} else {
		compute_fibonacci(fib_n, fib_memo);
		/*make sure that we succeded*/
		assert(big_int_equality(fib_n, fib_memo->items[fib_memo->elms-1]) == 1);	
	}

}

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
	/*initialize the structure with the base case*/
	fib_num = NULL;
	fib_num = newBigInt;
	big_int_from_str(fib_num, "0");
	vector_push_back(BigInt_ptr, fib_memo, fib_num);
	fib_num = newBigInt;
	big_int_from_str(fib_num, "1");
	vector_push_back(BigInt_ptr, fib_memo, fib_num);


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

		BigInt test; 
		test = big_int_add_stack(fib_num, fib_num);
		printf("%d\n", test.elms);

		bytes_written = big_int_to_str(&new_rep, &test);

		printf("%s\n", new_rep);
		/*printf("%s",lineptr); */
		free(lineptr);
		free(new_rep);

	}
	/*clean up the vector of fibonacci numbers*/
	for(ii = 0; ii < fib_memo->elms; ++ii) {
		BigInt_destroy(fib_memo->items[ii]);
	}
	vector_destroy(BigInt_ptr, fib_memo);
	return 0;
}
