#define _GNU_SOURCE
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <stdint.h>
#include <string.h>

#include "code_abbey_utility.h"
/*work around until detection macro*/
#define IS_LITTLE_ENDIAN 1
#define FIVE_BIT_MASK 0x1f
#define BASE32_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"

int encode32_buffer(char* input, size_t in_length, char** output, size_t* n_bytes) {
	/** encode32_buffer converts a buffer of chars to base32 encoding
	* 
	* If *output is set to NULL and *n_bytes is set to 0 before call, then
	* encode32_buffer() will allocate a buffer to store the encoded output,
	* this output buffer should be freed by user after failure if not NULL, 
	* which signifies the error was caught and handled internally
	*
	*  Alternatively, *output can be a pointer to a malloc(3)-allocated
	* buffer in_length bytes in size, if the buffer is not large enough to
	* hold the base32 encoding, the buffer will be resized with realloc(3),
	* updating *output and *n_bytes.
	*
	* The end result of a call to encode32_buffer() is that *output and *n_bytes
	* have been updated to reflect the output buffer address and buffer size
	* respetively.
	*/

	/*sanity checks*/
	if(input == NULL) return -1;
	if(in_length == 0) return 0;
	/*compute the required size of conversion buffer*/
	size_t conv_buff_sz, out_buff_sz;
	unsigned padding_bytes;
	padding_bytes = 5 - (in_length % 5);
	conv_buff_sz = in_length + padding_bytes;
	/*output buffer is 8/5 times conv_buff_sz*/
	out_buff_sz = (conv_buff_sz / 5) * 8;
	/*check if output buffer is large enough and realloc accordingly*/

	/*make conv_buff_sz divisible by eight to allow aliasing as uint64_t*/
	printf("conv_buff_sz = %u\n", conv_buff_sz);
	conv_buff_sz += (conv_buff_sz % 8) ? (8 - conv_buff_sz % 8) : 0;
	printf("conv_buff_sz = %u\n", conv_buff_sz);
	/*allocate an internal buffer for conversion*/
	char* conv_buff;
	errno = 0;
	conv_buff = malloc(conv_buff_sz * sizeof (*conv_buff));
	if(conv_buff == NULL || errno !=0) return -1;

	/*if the ouput buffer is not large enough, it is realloced to correct size and
	* output ptr and n_bytes is updated*/
	char* base32 = BASE32_ALPHABET;
	unsigned ii, jj;
	size_t n_chunks; /*full size chunks in input buffer*/

	/*if the system is little endian, low adresses hold most significant
	* therefore we place buffer into*/

	free(conv_buff);
	return 0;
}

int decode32_buffer(char* input, size_t in_length, char** output, size_t* n_bytes) {
	/*sanity check*/
	if(in_length == 0) {
		*n_bytes = 0;
		*output = NULL;
		return 0;
	}
	if(input == NULL) return -1;

	char* base32 = BASE32_ALPHABET;

	return 0;
}

int main(int argc, char* argv[]) {
	/*Encode and decoder for ascii streams to base32*/
	int rc;
	unsigned N;
	errno = 0;
	rc = scanf(" %u ", &N);
	if(rc != 1 || errno != 0) {
		perror("input read failed on first line");
		exit(-1);
	}
	char* lineptr, * out_buffer;
	size_t nbytes, out_buff_sz;
	ssize_t bytes_read;
	lineptr = NULL;
	out_buffer = NULL;
	nbytes = 0;
	out_buff_sz = 0;

	unsigned ii;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		bytes_read = getline(&lineptr, &nbytes, stdin);
		if(bytes_read == -1) {
			perror("input read failure");
			if(lineptr != NULL) {
				free(lineptr);
			}
			if(out_buffer != NULL) {
				free(out_buffer);
			}
			exit(-1);
		}
		/*encode lines 1, 3, 5...*/
		if((ii%2)) {
			/*account for newlines*/
			decode32_buffer(lineptr, bytes_read - 1, &out_buffer, &out_buff_sz);
		} else {
			encode32_buffer(lineptr, bytes_read - 1, &out_buffer, &out_buff_sz);
		}

	}
	printf("\n");
	return 0;
}
