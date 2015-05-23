#ifndef _YAM_BIG_INT_
#define _YAM_BIG_INT_ 
#include <strings.h>

#include "parseCodeAbbeyInput.h"

/*we will use a vector as our big integer*/
VECTOR_INIT(char)

typedef Vector_t(char) BigInt;

#define newBigInt newVector(char)
#define BigInt_destroy(ptr) vector_destroy(char, ptr)
#define BigInt_clear(ptr) vector_clear(char, ptr)

void big_int_from_str(BigInt* dest, const char* src) {
	vector_clear(char, dest);
	char curs;
	while((curs = *src++)) {
		if((curs >= '0') && (curs <= '9')) {
			vector_push_back(char, dest, (curs - '0'));
		}
	}
}

size_t big_int_to_str(char** dest, BigInt* src) {
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

#endif
