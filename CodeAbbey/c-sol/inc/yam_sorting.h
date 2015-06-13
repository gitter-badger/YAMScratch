#ifdef __cplusplus
extern "C" {
#endif

#ifndef _YAM_QUICKSORT
#define _YAM_QUICKSORT 

#define QUICKSORT_INIT(TYPE) \
	void yam_quicksort_##TYPE(TYPE * list, unsigned left, unsigned right, unsigned (*less_than)(TYPE*, TYPE*)) {\
		/*find recursive case when len(list) > 3*/							\
		printf("left = %d, right = %d\n",left, right); 	\
		unsigned ii, jj, kk;												\
		TYPE* pivot;														\
		TYPE tmp;															\
		if(left+9 < right){													\
			for(kk = left; kk <= right; ++kk) { printf("%d ",list[kk]);} \
			printf("\n Done \n" ); 	\
			/*find median of three for pivot*/								\
			unsigned center;												\
			center = left + (right - left)/2;								\
			if(less_than(list+center, list+left)) {							\
				swap(list, center, left, tmp)								\
			}																\
			if(less_than(list+right, list+left)) {							\
				swap(list, right, left, tmp) 								\
			}																\
			if(less_than(list+right, list+center)) {						\
				swap(list, right, center, tmp) 								\
			}																\
			/*																\
			* now the pivot is in the center, move							\
			* the pivot to the right-1 position								\
			*/																\
			swap(list, center, right-1, tmp)								\
			/*pivot is the pointer to the array elements*/					\
			pivot = list + right - 1;										\
			printf("pivot = %d\n", *pivot); 	\
			/*begin partitioning array*/									\
			ii = left;														\
			jj = right - 1;													\
			for(;;) {														\
				while(++ii, less_than(list+ii, pivot));						\
				while(--jj, less_than(pivot, list+jj));						\
				if( ii < jj) {												\
					/*swap the values*/										\
					swap(list, ii, jj, tmp)									\
				} else {													\
					break;													\
				}															\
			}																\
			/*restore pivot*/												\
			swap(list, ii, right - 1, tmp)									\
			/*now recurse on smaller paritions*/							\
			for(kk = left; kk <= right; ++kk) { printf("%d ",list[kk]);} \
			yam_quicksort_##TYPE(list, left, ii - 1, less_than); 			\
			yam_quicksort_##TYPE(list, ii + 1, right, less_than); 			\
		} else {															\
			printf("calling insertion sort\n"); \
			/* 																\
			* perform simple insertion sort, we recycle the pivot variable 	\
			* here it is really just a temp var to save on swaps 			\
			*/																\
			for(ii = left+1; ii <= right; ++ii) {							\
				jj = ii;													\
				tmp = list[ii];											\
				printf("j = %d, A[i] = %d A[j-1] = %d\n", jj, tmp, list[jj -1] ); \
				while(jj > 0 && less_than(&tmp, list+jj-1)) {				\
					list[jj] = list[jj-1];									\
					printf("swapped element\n");	\
					--jj;													\
				}															\
				/*get the actual value of the pivot to copy in*/			\
				list[jj] = tmp;											\
			}																\
		}																	\
	}

#define swap(list, a, b, tmp) 			\
	tmp = list[a];						\
	list[a] = list[b];					\
	list[b] = tmp;


#define yam_quicksort(TYPE, list, left, right, fnptr) \
	yam_quicksort_##TYPE(list, left, right, fnptr)

#endif

#ifdef __cplusplus
}
#endif
