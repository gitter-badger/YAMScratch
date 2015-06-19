
#include "brainfuck_interpreter.h"
/*remember the Vector_t macro was imported in the header for this file*/

signed brainfuck_evaluate_file(FILE* fp, int debug) {
	return 0;
}

signed brainfuck_evaluate_buffer(char* src, size_t nbytes, int debug) {
	return 0;
}

signed _eval_buffer(char* src, size_t nbytes, struct TapeNode* cursor, Vector_t(int)* stack) {
	return 0;
}
/*only adds to atape, will not remove any nodes*/
signed _eval_buffer_debug(char* src, size_t nbytes, struct TapeNodeDebug* cursor, Vector_t(int)* stack ) {
	/*keep track of where in the data buffer we are*/
	unsigned long instruction_offset;
	/*keep track of where in the */
	signed long  data_offset;
	char* instr_ptr;

	instr_ptr = src;
	/*dereference the instruction pointer each time*/
	switch(*instr_ptr) {
		case '+': /*increment current value of cell*/
		case '-':
		case '>':
		case '[':
		case ']':
		case ',':
		case '.':
		case ';':
		case ':':
		case '#':
		case '$':
			break;
	}
	return 0;
}
