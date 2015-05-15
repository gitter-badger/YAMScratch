#ifndef _PARSE_CODE_ABBEY_
#define _PARSE_CODE_ABBEY_

#define INPUT_ARRAY_INIT(name) 										\
	typedef struct { 												\
		name * items; 												\
		size_t size; 												\
	} ia_##name##_t; 												\
	static inline ia_##name##_t *init_##name() { 					\
		return (ia_##name##_t*)calloc(1, sizeof(ia_##name##_t)); 	\
	} 																\
	static inline void destroy_##name(ia_##name##_t *p) { 			\
		if(p) { 													\
			free(p); 												\
		} 															\
	}

#define InArray(name) ia_##name##_t

/*operations*/
#define parseLine(name, fn_ptr, tokens) 0

#endif
