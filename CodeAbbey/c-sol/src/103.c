#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#include "yam_vector.h"

struct MaskData {
	unsigned long key;	/*unique value*/
	unsigned long value; /*value we use can use to compare objects*/
	size_t len; /*number of bits set in mask*/
	unsigned char* bits; /*array indicating which indices are set, is malloced and freed*/
};

#define MaskData_init(mask) \
	mask.key = 0;			\
	mask.value = 0;			\
	mask.len = 0;			\
	mask.bits = NULL;

#define MaskData_clear(mask) 	\
	if(mask.bits != NULL) {		\
		free(mask.bits);		\
	}

/*make single token name for use in TYPE macros*/
typedef struct MaskData struct_MaskData;

VECTOR_INIT(struct_MaskData)

/*becuse we are using a long as a bitmask*/
#define MAX_BITS 64

#define NULL_CHECK(ptr, msg) 	\
	if(ptr == NULL) {			\
		perror(msg);			\
		exit(-1);				\
	}


void print_MaskData(struct MaskData* m) {
	printf("Key: %lu value: %lu\n", m->key, m->value);
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
	unsigned ii, jj, kk, A, K, E, T;
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
	K = 0;
	for(;;) {
		rc = fscanf(stream," %u ", &A);
		if(rc == EOF) {
			break;
		}
		/*A should be 1 or zero*/
		assert(A < 2);
		/*this stores K as little endian, position 0 is represented by the low bit*/
		K |= A<<N;
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
	unsigned* index_buffer, *cursor;
	index_buffer = (unsigned*)malloc(N* sizeof(unsigned));
	NULL_CHECK(index_buffer, "failed to allocate a buffer")
	char* metadata;
	errno = 0;
	metadata = (char*)calloc(2*N, sizeof(char));
	NULL_CHECK(metadata, "failed to allocate metadata buffer")
	/*
	* Store each masks, we will only read up to N masks from the file,
	* this is the defined input format 
	*/
	Vector_t(struct_MaskData)* _masks;
	_masks = newVector(struct_MaskData);
	/*read in the N masks*/
	struct MaskData tmp_mask;
	MaskData_init(tmp_mask);
	for(ii = 0; ii < N; ++ii) {
		/*clears*/
		errno = 0;
		bytes_read = getline(&lineptr, &nbytes, stdin);
		if(bytes_read < 0) {
			/*this checks for when there is only EOF */
			break;
		} else if(bytes_read == 1) {
			/*check for empty line which should not happen*/
			if(*lineptr == '\n') {
				continue;
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
		/*check if input line has correct format*/
		if(colon == ':') {
			tmp_mask.key = E;
			/*walk over buffer starting from begining*/
			cursor = index_buffer;
			/*the below code depends on these two fields beign zeroed*/
			tmp_mask.len = 0;
			tmp_mask.value = 0;
			for(jj = 0; jj < MAX_BITS; ++jj) {
				rc = fscanf(stream," %u ", &A);
				if(rc == EOF) {
					break;
				}
				/*make sure shift will fit in mask, and prevents walking off
				* edge of array*/
				assert(A < MAX_BITS);
				tmp_mask.value |= 1<<A;
				tmp_mask.len++;
				/*store and advance buffer*/
				*cursor++ = A;
				if(metadata[2*A+1]++ == 0) {
					metadata[2*A] = E;
				}
			}
		}
		fclose(stream);
		/*check to see if pattern already exists*/
		/*allocate an appropriate size buffer for the MaskData
		* instance and copy valid contents of buff into*/
		tmp_mask.bits = (unsigned char*)malloc(tmp_mask.len * sizeof(unsigned char));
		for(kk = 0; kk < tmp_mask.len; ++kk) {
			tmp_mask.bits[kk] = index_buffer[kk];
		}
		vector_push_back(struct_MaskData, _masks, tmp_mask);
	}
	/*clean up the tmp_mask so we do not accidently free data belonging to
	* object inside the vector*/
	tmp_mask.bits = NULL;
	free(lineptr);
	/*we wont use index_buffer again*/
	free(index_buffer); index_buffer = NULL;
	/*======END OF READING INPUT==========*/
	/*======BEGIN SOLVING SYSTEM==========*/

	for(ii = 0; ii < N; ++ii) {
		printf("Bit %u is flipped %u times\n", ii, metadata[ii*2+1]);
	}
	printf("\n");

	/*free each of the MaskData bits arrays*/
	for(ii = 0; ii < _masks->elms; ++ii) {
		print_MaskData(&_masks->items[ii]);
		printf("\n");
		MaskData_clear(_masks->items[ii]);
	}
	vector_destroy(struct_MaskData, _masks);
	free(metadata);
	return 0;
}
