#ifdef __cplusplus
extern "C" {
#endif

#ifndef _YAM_QUICKSORT
#define _YAM_QUICKSORT 

#define QUICKSORT_INIT(TYPE) \
	void yam_quicksort_##TYPE(TYPE * list, unsigned left, unsigned right, unsigned (*less_than)(TYPE*, TYPE*)) {\
		/*find recursive case when len(list) > 3*/							\
		unsigned ii, jj;													\
		TYPE* pivot;														\
		if(left+2 < right){													\
			/*find median of three for pivot*/								\
			unsigned center;												\
			center = left + (right - left)/2;								\
			TYPE tmp;														\
			if(less_than(list+center, list+left)) {							\
				tmp = list[center];											\
				list[left] = list[center];									\
				list[center] = tmp;											\
			}																\
			if(less_than(list+right, list+left)) {							\
				tmp = list[left];											\
				list[left] = list[right];									\
				list[right] = tmp;											\
			}																\
			if(less_than(list+right, list+center)) {						\
				tmp = list[center];											\
				list[center] = list[right];									\
				list[right] = tmp;											\
			}																\
			/*																\
			* now the pivot is in the center, move							\
			* the pivot to the right-1 position								\
			*/																\
			tmp = list[right - 1];											\
			list[right-1] = list[center];									\
			list[center] = tmp;												\
			/*pivot is the pointer to the array elements*/					\
			pivot = list + right - 1;										\
			/*begin partitioning array*/									\
			ii = left;														\
			jj = right - 1;													\
			for(;;) {														\
				while(++ii, less_than(list+ii, pivot));						\
				while(--jj, less_than(pivot, list+jj));						\
				if( ii < jj) {												\
					/*swap the values*/										\
					tmp = list[ii];											\
					list[ii] = list[jj];									\
					list[jj] = tmp;											\
				} else {													\
					break;													\
				}															\
			}																\
			/*restore pivot*/												\
			tmp = list[right - 1];											\
			list[right-1] = list[ii];										\
			list[ii] = tmp;													\
			/*now recurse on smaller paritions*/							\
			yam_quicksort_##TYPE(list, left, ii - 1, less_than); 			\
			yam_quicksort_##TYPE(list, ii + 1, right, less_than); 			\
		} else {															\
			/* 																\
			* perform simple insertion sort, we recycle the pivot variable 	\
			* here it is really just a temp var to save on swaps 			\
			*/																\
			unsigned offset;														\
			offset = (right - left) * sizeof(TYPE);							\
			for(ii = 1; ii <= offset; ++ii) {								\
				jj = ii;													\
				pivot = list + ii;											\
				while(jj > 0 && less_than(pivot, list+jj-1)) {				\
					list[jj] = list[jj-1];									\
					--jj;													\
				}															\
				/*get the actual value of the pivot to copy in*/			\
				list[jj] = *pivot;											\
			}																\
		}																	\
	}

#define yam_quicksort(TYPE, list, left, right, fnptr) \
	yam_quicksort_##TYPE(list, left, right, fnptr)

#endif

#ifdef __cplusplus
}
#endif
