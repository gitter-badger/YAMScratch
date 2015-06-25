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

signed _eval_buffer(char* src, size_t nbytes, struct TapeNode* cursor,
					Vector_t(int)* stack, FILE* in_stream, FILE* out_stream) {
	return 0;
}
/*only adds to atape, will not remove any nodes*/
signed _eval_buffer_debug(char* src, size_t nbytes, struct TapeNodeDebug* cursor,
						  Vector_t(int)* stack, FILE* in_stream, FILE* out_stream) {
	/*keep track of where in the data buffer we are*/
	size_t instr_offset;
	instr_offset = 0;
	/*keep track of where in the tape we are, using the cursor position */
	signed long data_offset;
	data_offset = cursor->index;
	/*declare some variables to take input from fscanf*/
	char in_char;
	unsigned long in_unsigned;
	signed long in_signed;

	/*declare the internal variables*/
	char* instr_ptr;
	const char* instr_end, * instr_begin;
	int rc;
	unsigned long instr_count;
	instr_count = 0;
	/*end is the element one past end of buffer*/
	instr_end = src + nbytes;
	instr_begin = src;
	instr_ptr = src;
	/*uses short circuit evaluation, but make sure we don't walk off end of buffer*/
	while(instr_ptr != instr_end && instr_offset < nbytes) {
		switch(*instr_ptr) {
			case '+': /*increment current value of cell*/
				if(cursor->cell + 1 < cursor->cell) {
					fprintf(out_stream, "\t| a[%ld] Overflow!\n", cursor->index);
				}
				cursor->cell++;
				fprintf(out_stream, "%ld (%lu): %c | a[%ld]= %ld\n",instr_count, instr_offset, *instr_ptr, cursor->index, cursor->cell );
				instr_ptr++;
				instr_offset++;
				break;
			case '-': /*decrement current value of cell*/
				if(cursor->cell - 1 > cursor->cell) {
					fprintf(out_stream, "\t| a[%ld] Underflow!\n", cursor->index);
				}
				cursor->cell--;
				fprintf(out_stream,"%ld (%lu): %c | a[%ld] = %ld\n",instr_count, instr_offset, *instr_ptr, cursor->index, cursor->cell);
				instr_ptr++;
				instr_offset++;
				break;
			case '>': /*increment the memory cell under the pointer*/
				/*look ahead and allocate a new block if necessary*/
				if(cursor->next == NULL) {
					errno = 0;
					struct TapeNodeDebug* node = (struct TapeNodeDebug*)malloc(sizeof(TapeNodeDebug));
					/*right now I don't know all the failure modes 
					* so just indicate failure mode and exit */
					if(!errno) {
						perror("failed to allocate new tape data struct\n");
						exit(-1);
					}
					cursor->next = node;
					node->prev = cursor;
					cursor = node;
				} else {
					cursor = cursor->next;

				}
				/*we dont care as much about overflows in the data tape index*/
				data_offset++;
				cursor->index++;
				fprintf(out_stream, "%ld (%lu): %c | array pos. now %ld\n", instr_count, instr_offset, *instr_ptr, cursor->index);
				instr_ptr++;
				instr_offset++;
				break;
			case '<': /*decrement the memory cell under the pointer*/
				/*look behind and allocate a new block if necessary*/
				if(cursor->prev == NULL) {
					errno = 0;
					struct TapeNodeDebug* node = (struct TapeNodeDebug*)malloc(sizeof(TapeNodeDebug));
					/*right now I don't know all the failure modes 
					* so just indicate failure mode and exit */
					if(!errno) {
						perror("failed to allocate new tape data struct\n");
						exit(-1);
					}
					cursor->prev = node;
					node->next = cursor;
					cursor = node;
				} else {
					cursor = cursor->prev;
				}
				/*we dont care as much about overflows in the data tape index*/
				data_offset--;
				fprintf(out_stream, "%ld (%lu): %c | array pos. now %ld\n", instr_count, instr_offset, *instr_ptr, cursor->index);
				instr_ptr++;
				instr_offset++;
				break;
			case '[': /*JZ to just past matching ]*/
				size_t old_instr_offset;
				old_instr_offset = instr_offset;
				if(cursor->cell != 0) {
					/*search for the matching tag in the buffer*/
					/*this is a strict interpreter, we don't store tags for jumps*/
					while(*instr_ptr++ != ']') {
						/*check for buffer overflow, it is legal c to point one past end
						* of buffer but we cannot dereference*/
						instr_offset++;
						if(instr_ptr == instr_end) {
							fprintf(out_stream, "reached end of program without matching ]\n");
							break;
						}
					}
					fprintf(out_stream, "found matching ] at instr_offset = %lu\n", instr_offset );
				} else {
					/*
					* in the other case the instruction pointer is incremented
					* one past the closing ], so we have have to explicitly handle
					* the other case where we move into the loop body, increasing nesting level
					*/
					instr_ptr++;
					instr_offset++;
				}
				break;
			case ']': /*JNZ to matching [*/
				if(cursor->cell != 0) {
					while(*(--instr_ptr) != '[') {
						instr_offset--;
						if(instr_offset == 0) {
							/*make sure*/
						}
					}
				} else {
					/*increment the instruction pointer*/
					fprintf(stderr, "HI there\n" );
					instr_ptr++;
				}
			case ',': /*Input a character and store it in the cell at the pointer*/
				errno = 0;
				rc = fscanf(in_stream, "%c", &in_char);
				if(rc != 1) {
					if(errno == 0) {
						/*if errno is not set, we set it*/
						errno = EINVAL;
					}
					fprintf(stderr, "Input read of single char failed, instr_offset = %lu\n", instr_offset);
					perror("input read of single char failed\n");
					exit(-1);
				}
				/*print quickly*/
				fprintf(out_stream, "%ld (%lu): %c | read in %c (%d)\n",instr_count, instr_offset, *instr_ptr, in_char, (unsigned)in_char);
				fprintf(out_stream, "%ld (%lu): %c | a[%ld] = %u\n", instr_count, instr_offset, *instr_ptr, cursor->index, (unsigned)in_char);
				cursor->cell = in_char;
				instr_ptr++;
				instr_offset++;
				break;
			case '.':
			case ';':
				errno = 0;
				rc = fscanf(in_stream, " %ld", &in_signed);
				if(rc != 1) {
					if(errno == 0) {
						/*if errno is not set, we set it*/
						errno = EINVAL;
					}
					fprintf(stderr, "rc = %d\n",rc );
					fprintf(stderr, "Input read of single long integer failed, instr_offset = %lu\n", instr_offset);
					perror("input read of single long int failed\n");
					exit(-1);
				}
				/*print quickly*/
				fprintf(out_stream, "%ld (%lu): %c | read in %ld\n", instr_count, instr_offset, *instr_ptr,in_signed );
				fprintf(out_stream, "%ld (%lu): %c | a[%ld] = %ld\n", instr_count, instr_offset, *instr_ptr, cursor->index, in_signed);
				cursor->cell = in_signed;
				instr_ptr++;
				instr_offset++;
				break;
			case ':':
			case '#': /*pushes the current value in cell under pointer to stack*/
			case '$': /*pops the value from stack and overwrites the cell under the pointer*/
				break;
		}
		instr_count++;
	}
	return 0;
}
