#ifndef _YAM_BIG_INT_
#define _YAM_BIG_INT_ 

#include "parseCodeAbbeyInput.h"

/*we will use a vector as our big integer*/
VECTOR_INIT(char)

typedef Vector_t(char) BigInt;

#define newBigInt = newVector(char)
#define BigInt_destroy(ptr) vector_destroy(char, ptr)

void big_int_from_str(BigInt* dest, char* src) {
	vector_clear(char, dest);
	char curs;
	while((curs = *src++)) {
		if((curs >= '0') && (curs <= '9')) {
			vector_push_back(char, dest, (curs - '0'));
		}
	}
}

size_t big_int_to_str(char* dest, BigInt* src) {
	if(src != NULL){
		dest = calloc(src->elms + 1, sizeof(char));
		if(dest == NULL) {
			return 0;
		}
		unsigned ii;
		for(ii = 0; ii < src->elms; ++ii) {
			*dest++ = src->items[ii] + '0';
		}
		/*returns length up to but not including null terminator*/
		return src->elms;
	}
	else {
		dest = NULL;
		return 0;
	}
}



#endif
