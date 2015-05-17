#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <string.h> /*provides strsep*/

#include "parseCodeAbbeyInput.h"

VECTOR_INIT(int)

#define SIX 6

int main(int argc, char* argv[]) {


	Vector_t(int) foo = *(newVector(int));
	printf("elements: %d\n", foo.elms );
	printf("size: %d\n", foo._size );
	printf("items: %x\n", foo.items );

	Vector_t(int) * foo_ptr;
	foo_ptr = &foo;
	printf("%p\n",foo_ptr );
	vector_push_back(int, &foo_ptr, 7);
	printf("%p\n",foo_ptr );
	printf("elements: %d\n", foo_ptr->elms );
	return 0;
}
