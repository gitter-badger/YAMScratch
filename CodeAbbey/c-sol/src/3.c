#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <string.h> /*provides strsep*/

#include "parseCodeAbbeyInput.h"

/**/
TOKEN_ARRAY_INIT(int)
/**/

/*
typedef struct { 
	int* items; 
	unsigned _size; 
	unsigned elms; 
} ta_int_t; 
static inline ta_int_t *init_int() { 
	return (ta_int_t*)calloc(1, sizeof(ta_int_t)); 
} 
static inline void destroy_int(ta_int_t *p) { 
	if(p) { 
		if(p->items){free(p->items);} 
		free(p); 
	} 
} 
void push_back_int ( ta_int_t** ref_ptr, const int val ) {
	ta_int_t* out_ptr;
	out_ptr = *ref_ptr;
	unsigned new_size;
 	if(out_ptr->elms == out_ptr->_size) { 
 		if(out_ptr->elms) {
	 		new_size = (2 * out_ptr->elms * sizeof(int));
	 	} else {
	 		new_size = 2 * sizeof(int);
	 	}
 		int* new_ptr = realloc(out_ptr->items, new_size ); 
 		if(new_ptr == ((void *)0)) {
 		 	if( (*__errno_location ()) == 12) { 
 				perror("Not enough memory to allocate new element");
 			 	exit(-1); 
 			 } else {
 			 	printf("inderminate error");
 			 	exit(-1);
 			 }
 		}

 		out_ptr->items = new_ptr; 
	 	out_ptr->_size = new_size; 
 	}
 	*(out_ptr->items + out_ptr->elms) = val; 
 	out_ptr->elms += 1; 

 	printf("boo\n");
}
*/

#define SIX 6

int main(int argc, char* argv[]) {


	TokenArray(int) foo = *(NewTokenArray(int));
	printf("elements: %d\n", foo.elms );
	printf("size: %d\n", foo._size );
	printf("items: %x\n", foo.items );

	TokenArray(int) * foo_ptr;
	foo_ptr = &foo;
	printf("%p\n",foo_ptr );
	ta_push_back(int, &foo_ptr, 7);
	printf("%p\n",foo_ptr );
	printf("elements: %d\n", foo_ptr->elms );
	return 0;
}
