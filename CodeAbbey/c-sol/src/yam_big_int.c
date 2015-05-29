#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "parseCodeAbbeyInput.h"
#include "yam_big_int.h"

/*Big Int is little Endian, stores least significant in lowest adress space*/

void big_int_from_str(BigInt* dest, char* src) {
	vector_clear(char, dest);
	char curs, * end;
	unsigned length = strlen(src);
	end = src + length - 1 ;
	unsigned ii;
	for(ii = 0; ii < length; ++ii) {
		curs = *end--;
		if((curs >= '0') && (curs <= '9')) {
			vector_push_back(char, dest, (curs - '0'));
		}
	}
}

char* big_int_to_str(BigInt* src, size_t* bytes_written) {
	char* dest, * curs;
	dest = NULL;
	if(src != NULL){
		dest = (char*)malloc((src->elms + 1)*sizeof(char));
		if(dest != NULL) {
			curs = dest;
			unsigned ii;
			for(ii = 0; ii < src->elms; ++ii) {
				*curs++ = src->items[src->elms - ii - 1] + '0';
			}
			/*null terminate*/
			*curs = '\0';
			/*returns length up to but not including null terminator*/
			*bytes_written = src->elms;
		}
		return dest;
	}
	else {
		errno = EINVAL;
		return NULL;
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
	/*the items are equal and so they are not less than*/
	return 0;
}

void big_int_add_heap(BigInt* A, BigInt* B, BigInt* result) {
	BigInt_clear(result);
	BigInt* tmp;
	if(A->elms > B->elms) {
		tmp = A;
		A = B;
		B = tmp;
	}
	unsigned ii, jj, carry;
	carry = 0;
	for(ii = 0; ii < A->elms; ++ii) {
		carry += A->items[ii];
		carry += B->items[ii];
		/*this is base 10*/
		vector_push_back(char, result, (carry % 10));
		carry /= 10;
	}
	/*continue with the remaining digits of B*/
	for(jj = ii; jj < B->elms; ++jj) {
		carry += B->items[jj];
		/*this is base 10*/
		vector_push_back(char, result, (carry % 10));
		carry /= 10;
	}
	if(carry) {
		vector_push_back(char, result, carry);
	}
}

BigInt big_int_add_stack(BigInt* A, BigInt* B) {
	BigInt x;
	bzero(&x, sizeof(BigInt));
	/*switch the pointers so that A fewer or the same digits as B*/
	BigInt* tmp;
	if(A->elms > B->elms) {
		tmp = A;
		A = B;
		B = tmp;
	}
	unsigned ii, jj, carry;
	carry = 0;
	for(ii = 0; ii < A->elms; ++ii) {
		carry += A->items[ii];
		carry += B->items[ii];
		/*this is base 10*/
		vector_push_back(char, &x, (carry % 10));
		carry /= 10;
	}
	/*continue with the remaining digits of B*/
	for(jj = ii; jj < B->elms; ++jj) {
		carry += B->items[jj];
		/*this is base 10*/
		vector_push_back(char, &x, (carry % 10));
		carry /= 10;
	}
	if(carry) {
		vector_push_back(char, &x, carry);
	}
	return x;
}
