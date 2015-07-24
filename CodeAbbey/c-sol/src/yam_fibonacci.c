
#include "yam_big_int.h"
#include "yam_fibonacci.h"

Vector(BigInt_ptr)* init_fib_memo() {
	Vector(BigInt_ptr)* memo = newVector(BigInt_ptr);
	BigInt* fib_num;
	/*initialize the structure with the base case*/
	fib_num = NULL;
	fib_num = newBigInt;
	big_int_from_str(fib_num, "0");
	vector_push_back(BigInt_ptr, memo, fib_num);
	fib_num = newBigInt;
	big_int_from_str(fib_num, "1");
	vector_push_back(BigInt_ptr, memo, fib_num);
	return memo;
}

void destroy_fib_memo(Vector(BigInt_ptr)* p) {
	/*free each of the underlying BigInt objects*/
	unsigned ii;
	for(ii = 0; ii < p->elms; ++ii){
		if(p->items[ii] != NULL) {
			free(p->items[ii]);
		}
	}
	/*at the end, free the container itself*/
	vector_destroy(BigInt_ptr, p);
}

void fill_in_up_to_fibonacci(BigInt* fib_n, Vector(BigInt_ptr)* fib_memo) {
	char* new_rep;
	size_t nbytes;
	nbytes = 0;
	/*the biggest element is pushed to the back*/
	while( !(big_int_equality(fib_n, fib_memo->items[fib_memo->elms-1]))) {
		BigInt* fib_next;
		fib_next = newBigInt;
		big_int_add_heap(fib_memo->items[fib_memo->elms-1],fib_memo->items[fib_memo->elms-2], fib_next);
		new_rep = big_int_to_str(fib_next, &nbytes);
		vector_push_back(BigInt_ptr, fib_memo, fib_next);
	}
}

signed find_fibonacci_index(BigInt* fib_n, Vector(BigInt_ptr)* fib_memo) {
	/*check if the input is in the memo structure already by binary search*/
	unsigned left, right, mid;
	left = 0;
	right = fib_memo->elms - 1;
	while(right >= left){
		mid = left + (right - left)/2;
		if(big_int_equality(fib_memo->items[mid], fib_n)) {
			return mid;
		} else if(big_int_less_than(fib_memo->items[mid], fib_n)) {
			left = mid + 1;
		} else {
			right = mid - 1 ;
		}
	}
	return -1;
}
