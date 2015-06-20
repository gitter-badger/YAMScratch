#include <errno.h>
#include <stdlib.h>

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
	size_t instruction_offset;
	instruction_offset = 0;
	/*keep track of where in the tape we are, using the cursor position */
	signed long data_offset;
	data_offset = cursor->index;
	/*declare some variables to take input from scanf*/
	char in_char;
	unsigned long in_unsigned;

	/*declare the internal variables*/
	char* instr_ptr;
	int rc;

	instr_ptr = src;
	/*dereference the instruction pointer each time*/
	switch(*instr_ptr) {
		case '+': /*increment current value of cell*/
		case '-': /*decrement current value of cell*/
		case '>': /*increment the memory cell under the pointer*/
		case '<': /*decrement the memory cell under the pointer*/
		case '[': /*JZ to just past matching ]*/
		case ']': /*JNZ to matching [*/
		case ',': /*Input a character and store it in the cell at the pointer*/
			errno = 0;
			rc = scanf("%c", &in_char);
			if(rc != 1) {
				if(errno == 0) {
					/*if errno is not set, we set it*/
					errno = EINVAL;
				}
				fprintf(stderr, "Input read of single char failed, instruction_offset = %lu", instruction_offset);
				perror("input read of single char failed\n");
				exit(-1);
			}
			printf("input  = %c\n", in_char);
			printf("a[%ld] = %u", data_offset, (unsigned)in_char);
			break;
		case '.':
		case ';':
		case ':':
		case '#': /*pushes the current value in cell under pointer to stack*/
		case '$': /*pops the value from stack and overwrites the cell under the pointer*/
			break;
	}
	return 0;
}
