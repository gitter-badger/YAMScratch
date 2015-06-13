#ifdef __cplusplus
extern "C" {
#endif

#ifndef _YAM_QUICKSORT
#define _YAM_QUICKSORT 

#define QUICKSORT_INIT(TYPE) \
	void yam_quicksort_##TYPE(TYPE * list, unsigned left, unsigned right, \
	 					 unsigned (less_than*)(TYPE*, TYPE*)) {
		/*find recursive case when len(list) > 3*/
		if(left+2 < right){
			unsigned center;
			center = left + (right - left)/2;
			TYPE tmp;
			TYPE* pivot;
			if(less_than(list+center, list+left)) {
				tmp = list[center];
				list[left] = list[center];
				list[center] = tmp;
			}
			if(less_than(list+right, list+left)) {
				tmp = list[left];
				list[left] = list[right];
				list[right] = tmp;
			}
			if(less_than(list+right, list+center)) {
				tmp = list[center];
				list[center] = list[right];
				lis[right] = tmp;
			}
			/*
			* now the pivot is in the center, move
			* the pivot to the right-1 position
			*/
			tmp = list[right - 1]
		} else {
			/* perform simple insertion sort */
			int offset;
			offset = (right - left) * sizeof(TYPE);
		}


	}

#define yam_quicksort(TYPE, list, left, right, fnptr) \
	yam_quicksort_##TYPE(list, left, right, fnptr)

#endif

#ifdef __cplusplus
}
#endif