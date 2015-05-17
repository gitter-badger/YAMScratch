#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <string.h> /*provides strsep*/

#include "parseCodeAbbeyInput.h"
#include "wrapped.h"

VECTOR_INIT(int)
/*TOKENIZE must come after VECTOR declaration*/
TOKENIZE_INIT(int)


#define SIX 6


int main(int argc, char* argv[]) {
	/*setup our parser template*/
	int (*int_from_string)(char*);
	int_from_string = &wrap_atoi;
	size_t nbytes;
	ssize_t bytes_read;
	char* lineptr;
	lineptr = NULL;
	nbytes = 0;

	bytes_read = wrap_getline(&lineptr, &nbytes, stdin);
	printf("%s\n",lineptr );

	unsigned a;
	a = (*int_from_string)(lineptr);
	printf("%d\n\n",a );

	Vector_t(int) foo = *(newVector(int));
	printf("elements: %d\n", foo.elms );
	printf("size: %d\n", foo._size );
	printf("items: %p\n", foo.items );

	Vector_t(int) * foo_ptr;
	foo_ptr = &foo;
	printf("%p\n",foo_ptr );
	vector_push_back(int, &foo_ptr, 7);
	printf("%p\n",foo_ptr );

	printf("elements: %d\n", foo_ptr->elms);
	printf("size: %d\n", foo_ptr->_size );
	vector_push_back(int, &foo_ptr, 9);
	vector_push_back(int, &foo_ptr, 10);
	vector_push_back(int, &foo_ptr, 11);
	vector_push_back(int, &foo_ptr, 12);
	printf("elements: %d\n", foo_ptr->elms );
	printf("size: %d\n", foo_ptr->_size );
	unsigned ii;
	for(ii = 0; ii < foo_ptr->elms; ii++) {
		printf("items[%d] = %d\n", ii, foo_ptr->items[ii]);
	}
	return 0;
}
