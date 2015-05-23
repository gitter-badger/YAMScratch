#ifndef _YAM_BIG_INT_
#define _YAM_BIG_INT_ 
#include <strings.h>
#include <stdlib.h>

#include "yam_big_int.h"
#include "parseCodeAbbeyInput.h"

/*Big Int is little Endian, stores least significant in lowest adress space*/

void big_int_from_str(BigInt* dest, const char* src) {
	vector_clear(char, dest);
	char curs, * end;
	unsigned length = strlen(src);
	end = src + length;
	while((curs = *src++)) {
		if((curs >= '0') && (curs <= '9')) {
			vector_push_back(char, dest, (curs - '0'));
		}
	}
}

char* big_int_to_str(BigInt* src, size_t* bytes_written,) {
	char* string;
	if(src != NULL){
		*dest = realloc(*dest, (src->elms + 1)*sizeof(char));
		string = *dest;
		if(dest == NULL) {
			return 0;
		}
		unsigned ii;
		for(ii = 0; ii < src->elms; ++ii) {
			*string++ = src->items[ii] + '0';
		}
		/*null terminate*/
		*string = '\0';
		/*returns length up to but not including null terminator*/
		return src->elms;
	}
	else {
		*dest = NULL;
		return 0;
	}
}

int big_int_equality(BigInt* A, BigInt* B) {
	/*for now we assume there are no leading zeros*/
	if(A->elms != B->elms) {
		return 0;
	}
	/*
	* jump to the end where there is the most significant bits
	* and walk back from there. This may not be required for equality testing,
	* it depends on which bits are more likely to change for numbers of
	* equal digits and is probably application dependent
	*/
	unsigned ii, offset;
	offset = A->elms - 1;
	for(ii= 0; ii <= offset; ++ii) {
		if(A->items[offset-ii] != B->items[offset-ii]){
			return 0;
		}
	}
	/*all of the elements are the same so we return true*/
	return 1;

}

int big_int_less_than(BigInt* A, BigInt* B) {
	/*for now we assume there are no leading zeros*/
	if(A->elms > B->elms) {
		return 0;
	} else if(A->elms < B->elms) {
		return 1;
	} else {
		unsigned ii, offset;
		offset = A->elms - 1;
		for(ii = 0; ii <= offset; ++ii) {
			if(A->items[offset-ii] > B->items[offset-ii]) {
				return 0;
			} else if(A->items[offset-ii] < B->items[offset-ii]) {
				return 1;
			}
		}
	}
	/*catch all*/
	errno = EINVAL;
	return 0;
}

void big_int_add_heap(const BigInt* A, const BigInt* B, BigInt* result) {
	BigInt_clear(result);

}

BigInt big_int_add_stack(const BigInt* A, const BigInt* B) {
	printf("Here\n");
	BigInt x;
	bzero(&x, sizeof(BigInt));
	unsigned ii;
	for(ii = 0; ii < 50; ++ii) {
		printf("pushing back %d\n",x.elms );

		vector_push_back(char, &x, 1);
	}
	return x;
}


void compute_fibonacci(BigInt* fib_n, Vector_t(BigInt_ptr)* fib_memo) {
	char* new_rep;
	/*the biggest element is pushed to the back*/
	while( !(big_int_equality(fib_n, fib_memo->items[fib_memo->elms-1]))) {
		BigInt* fib_next;
		fib_next = newBigInt;

		big_int_add_heap(fib_memo->items[fib_memo->elms-1],fib_memo->items[fib_memo->elms-2], fib_next);
		bytes_written = big_int_to_str(&new_rep, fib_num);
		printf("Created: %s\n",new_rep);
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

#endif
