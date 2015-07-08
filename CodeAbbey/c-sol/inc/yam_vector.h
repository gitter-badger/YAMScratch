#ifdef __cplusplus
extern "C" {
#endif

#ifndef YAM_VECTOR
#define YAM_VECTOR

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef VECTOR_INIT
#define VECTOR_INIT(TYPE) 														\
	typedef struct { 															\
		TYPE* items; 															\
		unsigned _size; 														\
		unsigned elms;															\
	} vec_##TYPE##_t; 															\
	static inline vec_##TYPE##_t *init_vec_##TYPE() { 							\
		return (vec_##TYPE##_t*)calloc(1, sizeof(vec_##TYPE##_t)); 				\
	} 																			\
	static inline void destroy_vec_##TYPE(vec_##TYPE##_t *p) { 					\
		if(p) {																	\
			if(p->items){free(p->items);}										\
			free(p); 															\
		} 																		\
	}																			\
	static void push_back_##TYPE(vec_##TYPE##_t* out_ptr, const TYPE val) {		\
		unsigned new_size;														\
		/*resize the array to accomodate more elements*/						\
		if(out_ptr->elms == out_ptr->_size) {									\
			if(out_ptr->elms) {													\
		 		new_size = 2 * out_ptr->elms;									\
		 	} else {															\
		 		new_size = 2 ;													\
		 	}																	\
			TYPE* new_ptr = (TYPE*)realloc(out_ptr->items, new_size * sizeof(TYPE));\
			if(new_ptr == NULL) {												\
				if(errno == ENOMEM) {											\
					perror("Not enough memory to allocate new element");		\
					exit(-1);													\
				} else {														\
 				 	perror("Inderminate error");								\
 			 		exit(-1);													\
				}																\
			}																	\
			out_ptr->items = new_ptr;											\
			out_ptr->_size = new_size;											\
		}																		\
		out_ptr->items[out_ptr->elms] = val;									\
		out_ptr->elms += 1;														\
	}																			\
	static void clear_##TYPE(vec_##TYPE##_t * p) {								\
		if(p != NULL){															\
			if(p->items) {														\
				free(p->items);													\
				p->items = NULL;												\
			}																	\
			p->_size = 0;														\
			p->elms = 0;														\
		}																		\
	}																			\
	static void resize_vec_##TYPE(size_t n, vec_##TYPE##_t *p) {				\
		/*check for null*/														\
		if(p != NULL){															\
			TYPE* new_items = (TYPE*)realloc(p->items, n);						\
			if(new_items == NULL) {												\
				if(errno == ENOMEM) {											\
					perror("Not enough memory to allocate new element");		\
					exit(-1);													\
				} else {														\
 				 	perror("Inderminate error");								\
 			 		exit(-1);													\
				}																\
			}																	\
			p->items = new_items;												\
			if(n < p->elms) {													\
				p->elms = n;													\
			}																	\
			p->_size = n;														\
		}																		\
	}																			\
	static TYPE pop_##TYPE(vec_##TYPE##_t *p) {									\
		if(p != NULL) {															\
			if(p->elms == 0) {													\
				return 0;														\
			}																	\
			TYPE tmp;															\
			tmp = p->items[p->elms -1];											\
			p->items[p->elms - 1] = 0;											\
			p->elms--;															\
			return tmp;															\
		} else {																\
			/*indicate failure*/												\
			errno = EDESTADDRREQ;												\
			return 0;															\
		}																		\
	}

#endif
/*operations*/
#define Vector_t(TYPE) vec_##TYPE##_t
#define newVector(TYPE) init_vec_##TYPE()
#define vector_push_back(TYPE, out_ptr, val) push_back_##TYPE ( out_ptr, val)
#define vector_clear(TYPE, ptr) clear_##TYPE(ptr)
#define vector_destroy(TYPE, ptr) destroy_vec_##TYPE(ptr)
#define vector_resize(TYPE, ptr, n) resize_vec_##TYPE(n, ptr)
#define vector_pop(TYPE, ptr) pop_##TYPE(ptr)

#endif

#ifdef __cplusplus
}
#endif
