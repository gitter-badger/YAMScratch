#ifdef __cplusplus
extern "C" {
#endif

#ifndef _YAM_BRAINFUCK_INTERPRETER_
#define _YAM_BRAINFUCK_INTERPRETER_

/*use a linked list for the tape*/
typedef struct {


} TapeNode;

signed brainfuck_evaluate(FILE* fp);

signed brainfuck_evaluate(char* src);

signed brainfuck_evaluate_internal(char* src, size_t nbytes, );

#endif

#ifdef __cplusplus
}
#endif
