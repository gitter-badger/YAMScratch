#ifdef __cplusplus
extern "C" {
#endif

#ifndef YAM_DEQUE
#define YAM_DEQUE 1

#include <stdio.h>

#define PAGE_SIZE(TYPE)														\
	(((sizeof (TYPE)) < 512) ? ( (512 % (sizeof (TYPE))) ? (512 / (sizeof (TYPE)))+1 : 512 / (sizeof (TYPE)) ) : 1)

#define DEQUE_INIT(TYPE)													\
	struct deque_node_##TYPE;												\
	typedef struct deque_node_##TYPE {										\
		struct deque_node_##TYPE * next;									\
		struct deque_node_##TYPE * prev;									\
		TYPE page[PAGE_SIZE(TYPE)];											\
		unsigned offset;													\
	} deque_node_##TYPE;													\
	/*head node*/															\
	typedef struct {														\
		deque_node_##TYPE* begin;											\
		deque_node_##TYPE* end;												\
		unsigned _begin_offset;												\
		unsigned _end_offset;												\
		unsigned _size;														\
	} deque_##TYPE;															\
	static void print_page_size_##TYPE(){									\
		printf("page size is %lu\n", PAGE_SIZE(TYPE));						\
	}


#endif

#define print_page_size(TYPE) print_page_size_##TYPE()

#ifdef __cplusplus
}
#endif
