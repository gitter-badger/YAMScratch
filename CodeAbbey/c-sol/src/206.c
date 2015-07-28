#define _GNU_SOURCE
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "code_abbey_utility.h"
/*work around until detection macro*/
#define IS_LITTLE_ENDIAN 1
#define FIVE_BIT_MASK 0x1f
#define NUM_CHUNK_BITS 5
#define BASE32_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"

size_t encode32_buffer(char* input, size_t M, char** output, size_t* N) {
	/** encode32_buffer converts a buffer of chars to base32 encoding
	* 
	* If *output is set to NULL and *N is set to 0 before call, then
	* encode32_buffer() will allocate a buffer to store the encoded output,
	* this output buffer should be freed by user after failure if not NULL, 
	* which signifies the error was caught and handled internally
	*
	*  Alternatively, *output can be a pointer to a malloc(3)-allocated
	* buffer M bytes in size, if the buffer is not large enough to
	* hold the base32 encoding, the buffer will be resized with realloc(3),
	* updating *output and *N.
	*
	* The end result of a call to encode32_buffer() is that *output and *N
	* have been updated to reflect the output buffer address and buffer size
	* respetively.
	*
	* On sucess encode32_buffer will return the number of bytes written to the
	* output buffer, not including the terminating '\0'
	*/
	/*sanity checks*/
	if(input == NULL) return 0;

	if(M == 0) return 0;
	unsigned long ii, jj;
	/*compute the required size of conversion buffer*/
	size_t conv_buff_sz, out_buff_sz, out_bytes_written;
	unsigned padding_bytes;
	padding_bytes = (M % 5) ? 5 - (M % 5): 0;
	conv_buff_sz = M + padding_bytes;
	/*output buffer is 8/5 times conv_buff_sz + 1 for terminating newline*/
	out_buff_sz = ((conv_buff_sz / 5) * 8) + 1;
	/*check if output buffer is large enough and realloc accordingly*/
	errno = 0;
	if(*output == NULL) {
		*output = malloc(out_buff_sz);
		*N = out_buff_sz;
		if(*output == NULL) {
			/*this is an error*/
			return 0;
		}
	} else if(*N == 0) {
		realloc(output, out_buff_sz);
		*N = out_buff_sz;
	} else if(out_buff_sz > *N) {
		realloc(*output, out_buff_sz);
		*N = out_buff_sz;
	}
	/*glom all of the errno checks for the preceding allocations here*/
	if(errno != 0) {
		return 0;
	}

	/*if the system is little endian, low adresses hold most significant
	* therefore we place input buffer into conversion directly aligned
	* so that aliasing conv_buff to uint64_t* will allow reading off
	* first 5 bit chunks using mask 0x1f 
	* input: [ 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | ... ] 	
	* conv:  [ 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | ...]
	*		 ^			uint64_t			 ^
	*		 |_______________________________|
	* if system is big endian, we must offset input buffer when copying
	* into the conversion buffer by 3 bytes so that same masking scheme 
	* works
	* input:    offset   [ 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | ... ] 	
	* conv:  [ 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | ...]
	*		 ^			uint64_t			 ^
	*		 |_______________________________|
	*/
	if(!(IS_LITTLE_ENDIAN)) {
		conv_buff_sz += 3; /*account for padding*/
	}
	/*make conv_buff_sz divisible by eight to allow aliasing as uint64_t*/
	conv_buff_sz += (conv_buff_sz % 8) ? (8 - conv_buff_sz % 8) : 0;

	/*allocate an internal buffer for conversion*/
	char* conv_buff;
	errno = 0;
	conv_buff = calloc(conv_buff_sz , sizeof (*conv_buff));
	if(conv_buff == NULL || errno !=0) return 0;

	if(IS_LITTLE_ENDIAN) {
		/*copy the input buffer in reverse*/
		char* tmp_cursor;
		for(ii = 0; ii < M; ++ii) {
			conv_buff[ii] = input[M-1-ii];
		}
		/*write in padding characters for message*/
		for(ii = 0; ii < padding_bytes; ++ii) {
			conv_buff[M+ii] = '0' + padding_bytes;
		}
	} else {
		/*offset by 3 bytes*/
		memcpy( (conv_buff+3), input, M);
		/*write in padding characters for message*/
		for(ii = 0; ii < padding_bytes; ++ii) {
			conv_buff[M+3+ii] = '0' + padding_bytes;
		}
	}

	char* base32 = BASE32_ALPHABET;
	size_t n_chunks; /*full size chunks in input buffer*/
	n_chunks = (M + padding_bytes) / NUM_CHUNK_BITS;
	char* conv_cursor, * out_cursor;
	conv_cursor = conv_buff;
	out_cursor = *output;
	if(IS_LITTLE_ENDIAN) {
		/* Little endian*/
		for(ii = 0; ii < n_chunks; ++ii) {
			printf("chunk = %.5s\n", conv_cursor);
			for(jj = 0; jj < 8; ++jj) {
				printf("byte %d = %x \n", jj, conv_cursor[jj]);
			}
			/*alias the pointer and read each 5 bit chunck as one byte of output*/
			for(jj = 0; jj < 8; ++jj) {
				uint64_t tmp;
				tmp = (*(uint64_t*)conv_cursor);
				printf("tmp = %lx\n", tmp);
				tmp &= (uint64_t)FIVE_BIT_MASK << (NUM_CHUNK_BITS * (7 - jj));
				tmp >>= (NUM_CHUNK_BITS * (7 - jj));
				assert(tmp < 32);
				printf("key = %u\n", tmp);
				*out_cursor++ = base32[tmp];
			}
			/*advance the buffer pointer by 5 bytes for each chunk*/
			conv_cursor+= NUM_CHUNK_BITS;
		}
		/*null terminate the output buffer*/
		*out_cursor = '\0';
	} else {
		/*Big endian*/
		for(ii = 0; ii < n_chunks; ++ii) {
			printf("chunk = %.5s\n", conv_cursor);
			for(jj = 0; jj < 8; ++jj) {
				printf("byte %d = %x \n", jj, conv_cursor[jj]);
			}
			/*alias the pointer and read each 5 bit chunck as one byte of output*/
			for(jj = 0; jj < 8; ++jj) {
				uint64_t tmp;
				tmp = (*(uint64_t*)conv_cursor);
				printf("tmp = %lx\n", tmp);
				tmp &= ((uint64_t)FIVE_BIT_MASK << (jj*NUM_CHUNK_BITS));
				tmp >>= (jj * NUM_CHUNK_BITS);
				assert(tmp < 32);
				printf("key = %u\n", tmp);
				*out_cursor++ = base32[tmp];
			}
			/*advance the buffer pointer by 5 bytes for each chunk*/
			conv_cursor+= NUM_CHUNK_BITS;
		}
		/*null terminate the output buffer*/
		*out_cursor = '\0';
	}
	free(conv_buff);
	return 0;
}

int decode32_buffer(char* input, size_t M, char** output, size_t* N) {
	/*sanity check*/
	if(M == 0) {
		*N = 0;
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
			printf("DECODE_PLACEHOLDER\n");
			//decode32_buffer(lineptr, bytes_read - 1, &out_buffer, &out_buff_sz);
		} else {
			printf("ENCODE\n");
			encode32_buffer(lineptr, bytes_read - 1, &out_buffer, &out_buff_sz);
			printf("[%s]\n", out_buffer);
		}

	}
	if(out_buffer != NULL) free(out_buffer);
	if(lineptr != NULL) free(lineptr);
	printf("\n");
	return 0;
}
