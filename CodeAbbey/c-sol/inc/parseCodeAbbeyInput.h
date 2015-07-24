#ifdef __cplusplus
extern "C" {
#endif

#ifndef _PARSE_CODE_ABBEY_
#define _PARSE_CODE_ABBEY_

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "yam_vector.h"
/*=========================================================
			INPUT PARSER TEMPLATE
  =========================================================*/

#define TOKENIZE_INIT(TYPE) \
	void tokenize_##TYPE(Vector(TYPE)* out_ptr, TYPE (*fn_ptr)(char *), 		\
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

#ifdef __cplusplus
}
#endif
