#ifdef __cplusplus
extern "C" {
#endif

#ifndef _bar_example_
#define _bar_example_

#include "macro.h"

/*
this typedef should expand to:
	foo_char_t
*/
typedef Foo_t(char) Bar;

#define newBar newFoo(char)
#define Bar_destroy(ptr) foo_destroy(char, ptr)

void Bar_from_string(Bar* dest, char* src);
char* Bar_to_string(Bar* src, size_t* nbytes);

#endif

#ifdef __cplusplus
}
#endif