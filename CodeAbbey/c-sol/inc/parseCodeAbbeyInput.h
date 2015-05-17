#ifndef _PARSE_CODE_ABBEY_
#define _PARSE_CODE_ABBEY_

#include <errno.h>

#ifndef VECTOR_INIT
#define VECTOR_INIT(TYPE) 														\
	typedef struct { 															\
		TYPE* items; 															\
		unsigned _size; 														\
		unsigned elms;															\
	} vec_##TYPE##_t; 															\
	static inline vec_##TYPE##_t *init_##TYPE() { 								\
		return (vec_##TYPE##_t*)calloc(1, sizeof(vec_##TYPE##_t)); 				\
	} 																			\
	static inline void destroy_##TYPE(vec_##TYPE##_t *p) { 						\
		if(p) {																	\
			if(p->items){free(p->items);}										\
			free(p); 															\
		} 																		\
	}																			\
	void push_back_##TYPE ( vec_##TYPE##_t** ref_out_ptr, const TYPE val ) {	\
		vec_##TYPE##_t* out_ptr;												\
		out_ptr = *ref_out_ptr;													\
		unsigned new_size;														\
		/*resize the array to accomodate more elements*/						\
		if(out_ptr->elms == out_ptr->_size) {									\
			if(out_ptr->elms) {													\
		 		new_size = 2 * out_ptr->elms;									\
		 	} else {															\
		 		new_size = 2 ;													\
		 	}																	\
			TYPE* new_ptr = realloc(out_ptr->items, new_size * sizeof(TYPE));	\
			if(new_ptr == NULL) {												\
				if(errno == ENOMEM) {											\
					perror("Not enough memory to allocate new element");		\
					exit(-1);													\
				} else {														\
 				 	printf("Inderminate error");								\
 			 		exit(-1);													\
				}																\
			}																	\
			out_ptr->items = new_ptr;											\
			out_ptr->_size = new_size;											\
		}																		\
		out_ptr->items[out_ptr->elms] = val;									\
		out_ptr->elms += 1;														\
	}
#endif

/*operations*/

#define Vector_t(TYPE) vec_##TYPE##_t
#define newVector(TYPE) init_##TYPE()
#define vector_push_back(TYPE, out_ptr, val) push_back_##TYPE ( out_ptr, val)


/*=========================================================
			INPUT PARSER TEMPLATE
  =========================================================*/

#define TOKENIZE_INIT(TYPE) \
	void tokenize_##TYPE(Vector_t(TYPE)* out_ptr, TYPE (*fn_ptr)(char *), 		\
								char* line_ptr, char delimeter ) {				\
																				\
	}																			\

/*
* TYPE - the type of the token
* out_ptr - pointer to result structure
* fn_ptr - a function pointer that the accepts the input token string
* 			and returns a variable of type TYPE
* line_ptr - a pointer to the input string, must be null terminated
* delimeter - character to delimit
*/
#define tokenizeLine(TYPE, out_ptr, fn_ptr, line_ptr, delimeter) \
	tokenize_##TYPE(out_ptr, fn_ptr, line_ptr, delimeter) {}\
	

#endif
