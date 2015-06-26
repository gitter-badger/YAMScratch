#include <errno.h>
#include <stdlib.h>

#include "brainfuck_interpreter.h"
/*remember the Vector_t macro was imported in the header for this file*/

signed _destroy_tape_debug(struct TapeNodeDebug* cursor) {
	struct TapeNodeDebug* left, * right, * tmp;
	left = cursor->prev;
	right = cursor->next;
	/*clean up left side*/
	while(left != NULL) {
		tmp = left;
		left = tmp->prev;
		free(tmp);
	}
	while(right != NULL) {
		tmp = right;
		right = tmp->next;
		free(tmp);
	}
	free(cursor);
	return 0;
}

signed brainfuck_evaluate_file(FILE* fp, int debug) {
	return 0;
}

signed brainfuck_evaluate_buffer(char* src, size_t nbytes, int debug) {
	return 0;
}

signed _parse_buffer(char** buff, size_t* nbytes) {
	return 0;
}

signed _eval_buffer(char* src, size_t nbytes, struct TapeNode* cursor,
					Vector_t(long)* stack, FILE* in_stream, FILE* out_stream) {
	return 0;
}
/*only adds to atape, will not remove any nodes*/
signed _eval_buffer_debug(char* src, size_t nbytes, struct TapeNodeDebug* cursor,
						  Vector_t(long)* stack, FILE* in_stream, FILE* out_stream) {
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
	signed long prev_index;
	signed long loop_nesting_level;
	loop_nesting_level = 0;
	signed long old_nesting_level;
	/*end is the element one past end of buffer*/
	instr_end = src + nbytes;
	instr_begin = src;
	instr_ptr = src;
	/*uses short circuit evaluation, but make sure we don't walk off end of buffer*/
	while(instr_ptr != instr_end && instr_offset < nbytes) {
		switch(*instr_ptr) {
			case '+': /*increment current value of cell*/
				if(cursor->cell + 1 < cursor->cell) {
					fprintf(out_stream, "%ld (%lu): %c | a[%ld] Overflow!\n", instr_count, instr_offset, *instr_ptr, cursor->index);
				}
				cursor->cell++;
				fprintf(out_stream, "%ld (%lu): %c | a[%ld]= %ld\n",instr_count, instr_offset, *instr_ptr, cursor->index, cursor->cell );
				instr_ptr++;
				instr_offset++;
				break;
			case '-': /*decrement current value of cell*/
				if(cursor->cell - 1 > cursor->cell) {
					fprintf(out_stream, "%ld (%lu): %c | a[%ld] Underflow!\n",instr_count, instr_offset, *instr_ptr, cursor->index);
				}
				cursor->cell--;
				fprintf(out_stream,"%ld (%lu): %c | a[%ld] = %ld\n",instr_count, instr_offset, *instr_ptr, cursor->index, cursor->cell);
				instr_ptr++;
				instr_offset++;
				break;
			case '>': /*increment the memory cell under the pointer*/
				/*look ahead and allocate a new block if necessary*/
				prev_index = cursor->index;
				if(cursor->next == NULL) {
					errno = 0;
					struct TapeNodeDebug* node = (struct TapeNodeDebug*)calloc(1, sizeof(TapeNodeDebug));
					/*right now I don't know all the failure modes 
					* so just indicate failure mode and return  */
					if(errno != 0) {
						perror("failed to allocate new tape data struct\n");
						return (-1);
					}
					cursor->next = node;
					node->prev = cursor;
					cursor = node;
				} else {
					cursor = cursor->next;
				}
				/*we dont care as much about overflows in the data tape index*/
				data_offset++;
				/*DANGER this line overwrites the exisiting index for preallocated arrays*/
				cursor->index = prev_index + 1;
				fprintf(out_stream, "%ld (%lu): %c | array pos. now %ld\n", instr_count, instr_offset, *instr_ptr, cursor->index);
				instr_ptr++;
				instr_offset++;
				break;
			case '<': /*decrement the memory cell under the pointer*/
				/*look behind and allocate a new block if necessary*/
				prev_index = cursor->index;
				if(cursor->prev == NULL) {
					errno = 0;
					struct TapeNodeDebug* node = (struct TapeNodeDebug*)calloc(1, sizeof(TapeNodeDebug));
					/*right now I don't know all the failure modes 
					* so just indicate failure mode and return  */
					if(errno != 0) {
						perror("failed to allocate new tape data struct\n");
						return (-1);
					}
					cursor->prev = node;
					node->next = cursor;
					cursor = node;
				} else {
					cursor = cursor->prev;
				}
				/*we dont care as much about overflows in the data tape index*/
				data_offset--;
				/*DANGER this line overwrites the exisiting index for preallocated arrays*/
				cursor->index = prev_index - 1;
				fprintf(out_stream, "%ld (%lu): %c | array pos. now %ld\n", instr_count, instr_offset, *instr_ptr, cursor->index);
				instr_ptr++;
				instr_offset++;
				break;
			case '[': /*JZ to just past matching ]*/
				old_nesting_level = loop_nesting_level;
				loop_nesting_level++;
				size_t old_instr_offset;
				old_instr_offset = instr_offset;
				if(cursor->cell == 0) {
					/*search for the matching tag in the buffer*/
					/*this is a strict interpreter, we don't store tags for jumps*/
					while(*instr_ptr++ != ']' || old_nesting_level != loop_nesting_level) {
						/*check for buffer overflow, it is legal c to point one past end
						* of buffer but we cannot dereference. instr_end is one past end
						of buffer so we can use it to check */
						instr_offset++;
						if(instr_ptr == instr_end) {
							fprintf(out_stream, "reached end of program without matching ]\n");
							return -8;
						}
						/*because the pointer looks ahead one, we can update the nesting levels for the
						* conditional in the while loop*/
						if(*instr_ptr == '[') {
							loop_nesting_level++;
						} else if(*instr_ptr == ']') {
							loop_nesting_level--;
						}
					}
					fprintf(out_stream, "found matching ] at instr_offset = %lu\n", instr_offset );
				} else {
					fprintf(out_stream, "%ld (%lu): %c | a[%ld] = %ld, loop nesting level %ld\n",instr_count, instr_offset, *instr_ptr, cursor->index, cursor->cell, loop_nesting_level-1);
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
				old_nesting_level = loop_nesting_level;
				loop_nesting_level--;
				if(cursor->cell != 0) {
					/*
					* we check the instr_ptr one more time, by post decrementing just so that 
					* we can catch the case where it is the first character in the buffer
					* which is not a valid program 
					*/
					while(*instr_ptr != '[' || old_nesting_level != loop_nesting_level) {
						instr_ptr--;
						if(instr_offset == 0) {
							/*if the offset is zero, it means that we did not find a matching offset*/
							return -9;
						}
						if(*instr_ptr == '[') {
							loop_nesting_level++;
						} else if(*instr_ptr == ']') {
							loop_nesting_level--;
						}
						instr_offset--;
					}
					fprintf(out_stream, "%ld (%lu): %c | matching bracket found\n",instr_count, instr_offset, *instr_ptr);

				} else {
					fprintf(out_stream, "%ld (%lu): %c | a[%ld] = %ld, loop nesting level %ld\n",instr_count, instr_offset, *instr_ptr, cursor->index, cursor->cell, loop_nesting_level);
					/*increment the instruction pointer*/
					instr_ptr++;
				}
				break;
			case ',': /*Input a character and store it in the cell at the pointer*/
				errno = 0;
				rc = fscanf(in_stream, "%c", &in_char);
				if(rc == 1) {
					cursor->cell = in_char;
				} else if(rc == EOF) {
					if(errno == 0) {
						/*nothing was wrong, there was just no characters to read*/
							/*we will pass this value on the the brainfuck tape
							* and keep running */
							cursor->cell = -1;
							/*this assignment is for below printing only*/
							in_char = EOF;
					} else {
					perror("input read of single char failed\n");
					return (-1);
					}
				} else {
					fprintf(stderr, "Input read of single char failed, instr_offset = %lu\n", instr_offset);
					perror("input read of single char failed\n");
					return (-1);
				}
				/*print quickly*/
				fprintf(out_stream, "%ld (%lu): %c | read in %c (%d)\n",instr_count, instr_offset, *instr_ptr, in_char, (signed)in_char);
				fprintf(out_stream, "%ld (%lu): %c | a[%ld] = %ld\n", instr_count, instr_offset, *instr_ptr, cursor->index, cursor->cell);
				instr_ptr++;
				instr_offset++;
				break;
			case '.':
				fprintf(out_stream, "%ld (%lu): %c | %c", instr_count, instr_offset, *instr_ptr, (char)cursor->cell);
				instr_ptr++;
				instr_offset++;
				break;
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
					return (-1);
				}
				/*print quickly*/
				fprintf(out_stream, "%ld (%lu): %c | read in %ld\n", instr_count, instr_offset, *instr_ptr, in_signed );
				fprintf(out_stream, "%ld (%lu): %c | a[%ld] = %ld\n", instr_count, instr_offset, *instr_ptr, cursor->index, in_signed);
				cursor->cell = in_signed;
				instr_ptr++;
				instr_offset++;
				break;
			case ':':
				fprintf(out_stream, "%ld (%lu): %c | Output %ld\n",instr_count, instr_offset, *instr_ptr, cursor->cell);
				instr_ptr++;
				instr_offset++;
				break;
			case '#': /*pushes the current value in cell under pointer to stack*/
				vector_push_back(long, stack, cursor->cell);
				fprintf(out_stream, "%ld (%lu): %c | Push down %ld, size = %u\n",instr_count, instr_offset, *instr_ptr, cursor->cell, stack->elms);
				instr_ptr++;
				instr_offset++;
				break;
			case '$': /*pops the value from stack and overwrites the cell under the pointer*/
				if(stack->elms == 0) {
					/*stack is empty*/
					fprintf(stderr, "%ld (%lu): %c | Cannot pop, Stack Empty\n", instr_count, instr_offset, *instr_ptr );
					return -6;
				}
				fprintf(stderr, "%ld (%lu): %c | a[%ld] <= %ld \n", instr_count, instr_offset, *instr_ptr, cursor->index, cursor->cell);
				cursor->cell = vector_pop(long, stack);
				instr_ptr++;
				break;
		}
		instr_count++;
	}
	return 0;
}
