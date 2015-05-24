#ifdef __cplusplus
extern "C" {
#endif

#ifndef _foo_example_
#define _foo_example_

#include <stdlib.h>

#define FOO_INIT(TYPE) 			\
	typedef struct { 			\
		TYPE* items; 			\
		unsigned number; 		\
	} foo_##TYPE##_t;			\
	static inline foo_##TYPE_t * init_##TYPE() { \
		return (foo_##TYPE_t*)calloc(1, sizeof(foo_##TYPE##_t)); \
	} \
	static inline void destroy_##TYPE(foo_##TYPE##_t *p) { \
		if(p) { \
			if(p->items) { \
				free(p->items); \
			} \
			free(p); \
		} \
	}

#define Foo_t(TYPE) foo_##TYPE##_t
#define newFoo(TYPE) init_##TYPE()
#define foo_destroy(TYPE, ptr) destroy_##TYPE(ptr)

#endif

ifdef __cplusplus	
}
#endif