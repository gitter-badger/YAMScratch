#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "yam_vector.h"
VECTOR_INIT(char)

#define BUFF_SIZE 100

int main(int argc, char const *argv[])
{
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	unsigned* result;
	errno = 0;
	result = (unsigned*)malloc(N * sizeof(unsigned));
	if(result == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	}
	char* input, *buff;
	errno = 0;
	buff = (char*)malloc(BUFF_SIZE * sizeof(char));
	if(buff == NULL) {
		perror("failed to allocate read buffer");
		exit(-1);
	}
	Vector(char)* stack;
	stack = newVector(char);
	unsigned ii, jj;
	char tmp;
	for(ii = 0; ii < N; ++ii) {
		vector_clear(char, stack);
		/*read each line as stream of chunks*/
		/*use the result as a flag variable*/
		result[ii] = 1;
		for(;;) {
			errno = 0;
			input = fgets(buff, BUFF_SIZE, stdin);
			if(errno != 0) {
				perror("failed to read input");
				exit(-1);
			}
			if(input == NULL) {
				if(ferror(stdin) != 0) {
					fprintf(stderr, "Read failure, ferror = %d\n", ferror(stdin));
				}
				/*reached eof before any new characters could be read*/
				break;
			}
			unsigned bytes_read;
			bytes_read = strlen(input);
			/*autonoma accepting input walking over buffer*/
			for(jj = 0; (jj < bytes_read) && result[ii]; ++jj) {
				switch(input[jj]) {
					case '<': /*FALL THROUGH*/
					case '{': /*FALL THROUGH*/
					case '[': /*FALL THROUGH*/
					case '(':
						vector_push_back(char, stack, input[jj]);
						break;
					case '>':
					case '}':
					case ']':
						/*
						* take advantage of fact that these three are offset by one
						* char from matching bracket in ascii table
						*/
						if(stack->elms != 0) {
							tmp = vector_pop(char, stack); 
						} else {
							result[ii] = 0;
							break;
						}
						if(tmp + 2 != input[jj]) {
							result[ii] = 0;
						}
						break;
					case ')':
						if(stack->elms != 0) {
							tmp = vector_pop(char, stack); 

						} else {
							result[ii] = 0;
							break;
						}
						if(tmp != '(') {
							result[ii] = 0;
						}
						break;
				}
			}
			/*don't break, as we want to continue to read all chunks until we get 
			* to a newline */
			/*check if we read the last chunk accounting for null character*/
			if(bytes_read < BUFF_SIZE -1) {
				break;
			}
			/*if the last character in the buffer is a newline, we quit as well*/
			if(input[bytes_read-1] == '\n') {
				break;
			}
		}
		/*if there are still items in the stack, it means not all brackets were
		* matched, which is a failure */
		if(result[ii] != 0 && stack->elms != 0) {
			result[ii] = 0;
		}
	}
	for(ii = 0; ii < N; ++ii) {
		printf("%d ",result[ii]);
	}
	printf("\n");
	free(buff);
	free(result);
	return 0;
}
