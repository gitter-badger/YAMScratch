#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

/*becuse we are using a long as a bitmask*/
#define MAX_BITS 64

struct MaskData {
	unsigned long key; /*key we use can use to compare objects*/
	size_t len; /*number of bits set in mask*/
	unsigned char* bits; /*array indicating which indices are set, is malloced and freed*/
};

void print_MaskData(struct MaskData* m) {
	printf("Key: %lu\n", m->key);
	/*printf("Len: %lu\n", m->len);*/
	printf("Bits: ");
	size_t ii;
	for(ii = 0; ii < m->len; ++ii) {
		printf("%u ", m->bits[ii]);
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	int rc;
	unsigned ii, jj, kk, A, I, E, T;
	unsigned long N;
	/*read first line with unknown length of data*/
	char * lineptr;
	size_t nbytes;
	ssize_t bytes_read;
	FILE * stream;
	lineptr = NULL;
	nbytes = 0;
	errno = 0;
	bytes_read = getline(&lineptr, &nbytes, stdin);
	if(bytes_read < 0) {
		/*this checks for when there is only EOF */
		perror("EOF reached before proper input could be processed");
		exit(-1);
	} else if(bytes_read == 1) {
		/*check for empty line which should not happen*/
		if(*lineptr == '\n') {
			perror("Lines should not be empty");
			exit(-1);
		}
	}
	stream = fmemopen(lineptr, bytes_read, "r");
	N = 0;
	I = 0;
	for(;;) {
		rc = fscanf(stream," %u ", &A);
		if(rc == EOF) {
			break;
		}
		/*A should be 1 or zero*/
		assert(A < 2);
		/*this stores I as big endian, position 0 is represented by the high bit*/
		I |= A<<N;
		++N;
	}
	if(N > MAX_BITS) {
		/*we cannot represent more than 64 bits using long
		* so we must give an error in this implementation*/
		fprintf(stderr, "More than 64 states given by input\n");
		fflush(stderr);
		exit(-1);
	}
	fclose(stream);
	/* Allocate a buffer to record which indices are set */
	unsigned* index_buffer;
	index_buffer = (unsigned*)malloc(N* sizeof(unsigned));
	if(index_buffer == NULL) {
		perror("failed to allocate a buffer");
		exit(-1);
	}
	/*
	* Store each masks, we will only read up to N masks from the file,
	* this is defined input format 
	*/
	struct MaskData* _masks;
	errno = 0;
	_masks = (struct MaskData*)malloc(N * sizeof(struct MaskData));
	if(_masks == NULL) {
		perror("failed to allocate masks buffer");
		exit(-1);
	}
	char* metadata;
	errno = 0;
	metadata = (char*)calloc(2*N, sizeof(char));
	if(metadata == NULL) {
		perror("failed to allocate metadata buffer");
		exit(-1);
	}
	/*ptr inside temporary buffer that creates */
	unsigned* cursor;
	/*read in the N masks*/
	for(ii = 0; ii < N; ++ii) {
		printf("iteration %u", ii);
		errno = 0;
		bytes_read = getline(&lineptr, &nbytes, stdin);
		if(bytes_read < 0) {
			/*this checks for when there is only EOF */
			break;
		} else if(bytes_read == 1) {
			/*check for empty line which should not happen*/
			if(*lineptr == '\n') {
				perror("Lines must have numbers on them");
				exit(-1);
			}
		}
		stream = fmemopen(lineptr, bytes_read, "r");
		/*find the index of the rule, these can be out of order
		* and the last rule present will overwrite all previous ones
		* silently
		*/
		char colon;
		/*make sure that each line begins with an index and then
		* the colon character delimeter follows*/
		rc = fscanf(stream, "%u %c", &E, &colon);
		assert(colon == ':');
		/*walk over buffer starting from begining*/
		cursor = index_buffer;
		/*the below code depends on these two fields beign zeroed*/
		_masks[E].len = 0;
		_masks[E].key = 0;
		for(jj = 0; jj < MAX_BITS; ++jj) {
			rc = fscanf(stream," %u ", &A);
			if(rc == EOF) {
				break;
			}
			/*make sure shift will fit in mask, and prevents walking off
			* edge of array*/
			assert(A < MAX_BITS);
			_masks[E].key |= 1<<A;
			_masks[E].len++;
			/*store and advance buffer*/
			*cursor++ = A;
			if(metadata[2*A+1]++ == 0) {
				metadata[2*A] = E;
			}
		}
		fclose(stream);
		/*check to see if pattern already exists*/
		/*allocate an appropriate size buffer for the MaskData
		* instance and copy valid contents of buff into*/
		_masks[E].bits = (unsigned char*)malloc(_masks[E].len* sizeof(unsigned char));
		for(kk = 0; kk < _masks[E].len; ++kk) {
			_masks[E].bits[kk] = index_buffer[kk];
		}
	}
	free(lineptr);
	/*we wont use index_buffer again*/
	free(index_buffer); index_buffer = NULL;
	for(ii = 0; ii < N; ++ii) {
		printf("Bit %u is flipped %u times\n", ii, metadata[ii*2+1]);
	}
	printf("\n");

	/*free each of the MaskData bits arrays*/
	for(ii = 0; ii < N; ++ii) {
		print_MaskData(_masks+ii);
		printf("\n");
		free(_masks[ii].bits);
	}
	free(_masks);
	free(metadata);
	return 0;
}
