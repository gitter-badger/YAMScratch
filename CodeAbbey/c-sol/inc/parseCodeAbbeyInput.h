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
	void push_back_##TYPE(vec_##TYPE##_t* out_ptr, const TYPE val) {			\
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
	void clear_##TYPE(vec_##TYPE##_t * p) {										\
		if(p != NULL){															\
			if(p->items) {														\
				free(p->items);													\
				p->items = NULL;												\
			}																	\
			p->_size = 0;														\
			p->elms = 0;														\
		}																		\
	}																			\
	void resize_##TYPE(size_t n, vec_##TYPE##_t *p) {							\
		/*check for null*/														\
		if(p != NULL){															\
			TYPE* new_items = realloc(p->items, n);							\
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
	TYPE pop_##TYPE(vec_##TYPE##_t *p) {										\
		if(p != NULL) {															\
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
#define newVector(TYPE) init_##TYPE()
#define vector_push_back(TYPE, out_ptr, val) push_back_##TYPE ( out_ptr, val)
#define vector_clear(TYPE, ptr) clear_##TYPE(ptr)
#define vector_destroy(TYPE, ptr) destroy_##TYPE(ptr)
#define vector_resize(TYPE, ptr, n) resize_##TYPE(n, ptr)
#define vector_pop(TYPE, ptr) pop_##TYPE(ptr)

/*=========================================================
			INPUT PARSER TEMPLATE
  =========================================================*/

#define TOKENIZE_INIT(TYPE) \
	void tokenize_##TYPE(Vector_t(TYPE)* out_ptr, TYPE (*fn_ptr)(char *), 		\
						 char* line_ptr, ssize_t n_bytes, char delimeter ) {	\
		unsigned ii;															\
		unsigned token_len;														\
		char* token, tmp;														\
		token_len = 0;															\
		TYPE value;																\
		value = 0;																\
		for(ii = 0; ii < n_bytes; ii++){										\
			switch (line_ptr[ii]) {												\
			case ' ': /* FALLTHROUGH */											\
			case '\t':/* FALLTHROUGH */											\
			case '\r':/* FALLTHROUGH */											\
			case '\n':/* FALLTHROUGH */											\
			case '\v':/* FALLTHROUGH */											\
			case '\f':															\
			case EOF:															\
				token_len = 0;													\
				tmp = line_ptr[ii];												\
				line_ptr[ii] = '\0'; /*make a token string by null terminating*/\
				value = (*fn_ptr)(token);										\
				line_ptr[ii] = tmp; 											\
				vector_push_back(TYPE, out_ptr, value);							\
				break;															\
			default:															\
				/*for when delimeter is not whitespace*/						\
				if(line_ptr[ii] == delimeter) {									\
					token_len = 0;												\
					tmp = line_ptr[ii];											\
					/*make a token string by null terminating*/					\
					line_ptr[ii] = '\0'; 										\
					value = (*fn_ptr)(token);									\
					line_ptr[ii] = tmp; 										\
					vector_push_back(TYPE, out_ptr, value);						\
				} else {														\
					token_len++;												\
				}																\
				break;															\
			}																	\
			/*started new token*/												\
			if(token_len == 1) {												\
				token = line_ptr + ii;											\
			}																	\
		}																		\
	}																			

/*
* TYPE - the type of the token
* out_ptr - pointer to result structure
* fn_ptr - a function pointer that the accepts the input token string
* 			and returns a variable of type TYPE
* line_ptr - a pointer to the input string, must be null terminated
* delimeter - character to delimit
*/
#define tokenizeLine(TYPE, out_ptr, fn_ptr, line_ptr, n_bytes, delimeter) \
	tokenize_##TYPE(out_ptr, fn_ptr, line_ptr, n_bytes, delimeter)\
	
#endif
