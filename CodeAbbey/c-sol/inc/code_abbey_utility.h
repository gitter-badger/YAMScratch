#ifdef __cplusplus
extern "C" {
#endif

#ifndef CODE_ABBEY_UTILITY
#define CODE_ABBEY_UTILITY
#include <stdio.h>

signed array_checksum(long const * array, size_t size, long* checksum);
/*sort in place*/

#define selection_sort_init(TYPE) \
	extern signed selection_sort_##TYPE(TYPE * array, size_t size, int (*cmp)(TYPE *,  TYPE *)) { \
		/*sanity checks*/	\
		if(size < 2) {	\
			return 0;	\
		}	\
		if(array == NULL) {	\
			return -1;	\
		}	\
		TYPE tmp;	\
		size_t ii, jj, max;	\
		/*make N passes through array*/	\
		for(ii = 1; ii < size; ++ii) {	\
			max = 0;	\
			for(jj = 1; jj <= size - ii; ++jj) {	\
				/*if element less than max*/	\
				if((*cmp)(array+jj, array+max)) {	\
				} else {	\
					max = jj;	\
				}	\
			}	\
			printf("%ld ", max );	\
			/*swap maximum element with end element*/	\
			tmp = array[size-ii];	\
			array[size-ii] = array[max];	\
			array[max] = tmp;	\
		}	\
		return 0;	\
	}

#define selection_sort(TYPE, array, size, cmp) selection_sort_##TYPE(array, size, cmp)

#endif

#ifdef __cplusplus
}
#endif
