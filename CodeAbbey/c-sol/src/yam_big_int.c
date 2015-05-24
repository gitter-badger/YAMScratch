#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <assert.h>

#include "parseCodeAbbeyInput.h"
#include "yam_big_int.h"

/*Big Int is little Endian, stores least significant in lowest adress space*/

void big_int_from_str(BigInt* dest, char* src) {
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

char* big_int_to_str(BigInt* src, size_t* bytes_written) {
	char* dest;
	dest = NULL;
	if(src != NULL){
		dest = (char*)realloc(dest, (src->elms + 1)*sizeof(char));
		if(dest == NULL) {
			return 0;
		}
		unsigned ii;
		for(ii = 0; ii < src->elms; ++ii) {
			*dest++ = src->items[ii] + '0';
		}
		/*null terminate*/
		*dest = '\0';
		/*returns length up to but not including null terminator*/
		*bytes_written = src->elms;
		return dest;
	}
	else {
		dest = NULL;
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
