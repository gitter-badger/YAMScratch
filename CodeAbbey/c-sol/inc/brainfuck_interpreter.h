#ifdef __cplusplus
extern "C" {
#endif

#ifndef _YAM_BRAINFUCK_INTERPRETER_
#define _YAM_BRAINFUCK_INTERPRETER_

#include <stdio.h>

/*use the vector as the stack for the extended mode*/
#include "parseCodeAbbeyInput.h"
VECTOR_INIT(int)

/*use a linked list for the data tape*/
struct TapeNode {
	struct TapeNode* next;
	struct TapeNode* prev;
	signed long cell;
};

/*
* The intention is to provide me with a debugger as I learn brainfuck,
* so it is very likely that I will give invalid instructions
* like infinite loops, so we will try to expose as much information as possible
* in the debug version as I will be the only one using it, and I need all
* the help I can get:
* include the index in the memory so the memory debugger can tell us where we are
* in the brainfuck data tape
* */
struct TapeNodeDebug {
	struct TapeNodeDebug* next;
	struct TapeNodeDebug* prev;
	signed long cell;
	signed long index;
};


signed brainfuck_evaluate_file(FILE* fp, int debug);

signed brainfuck_evaluate_buffer(char* src, size_t nbytes, int debug);

/*private implementation of interpreter*/
signed _eval_buffer(char* src, size_t nbytes, struct TapeNode* cursor,
					Vector_t(int)* stack, FILE* in_stream, FILE* out_stream);

/*
* debug version will check for overflows and underflows data cells and emmit warning
* to stderr, debug version also outputs action
*/
signed _eval_buffer_debug(char* src, size_t nbytes, struct TapeNodeDebug* cursor,
						  Vector_t(int)* stack, FILE* in_stream, FILE* out_stream);

#endif

#ifdef __cplusplus
}
#endif
