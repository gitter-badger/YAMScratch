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
	unsigned* bits; /*array indicating which indices are set, is malloced and freed*/
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

#define BIT_ARRAY_REPR_TYPE char
#define MASK_ACTIVE 1
#define MASK_INACTIVE 0
#define MASK_KEEP 10

int remove_one(unsigned N, unsigned* bit_counts, unsigned* target_bit_counts,
				unsigned lower_bound, unsigned* possible, struct MaskData* masks) {
	unsigned ii, jj, kk;
	/*check if this is a solution*/
	unsigned n_possible;
	n_possible = 0;
	unsigned possible_take_away[N];
	for(ii = lower_bound; ii < N; ++ii) {
		if(possible[ii] == MASK_ACTIVE) {
			for(jj = 0; jj < masks[ii].len; ++jj)
				kk = masks[ii].bits[jj];
				if(bit_counts[kk] == 1 && target_bit_counts[kk] == 1) {
					/*we cannot take away any more*/
					possible_take_away[n_possible] = ii;
					++n_possible;
			}
		}
	}
	return 0;
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
			/*for the masks to make sense in the context of our problem
			* where touching an egg at index I flips the bits given by XOR
			* the mask with the current state vector, we cannot touch an egg
			* outside of the the range*/
			assert(E < N);
			/*TODO: make strong check that the new mask does not overwrite 
			* any of the previous masks, aka there should no be two rules 
			* for touching the same egg */
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
		tmp_mask.bits = (unsigned*)malloc(tmp_mask.len * sizeof(unsigned));
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
	struct MaskData* mask_buffer;
	mask_buffer = (struct MaskData*)malloc(N * sizeof(struct MaskData));
	NULL_CHECK(mask_buffer, "failed to allocate buffer for all masks");
	/*allocate a buffer that marks masks as available*/
	char* possible;
	possible = (char*)malloc(N * sizeof(char));
	NULL_CHECK(possible, "failed to allocate buffer for flags");
	memset(possible, MASK_INACTIVE, N*sizeof(char));
	/*copy all masks into an array that will act as set with contant tim
	* lookup by mask.key == index*/
	for(ii = 0; ii < _masks->elms ; ++ii) {
		mask_buffer[_masks->items[ii].key] = _masks->items[ii];
		possible[_masks->items[ii].key] = MASK_ACTIVE;
	}

	for(ii = 0; ii < N; ++ii) {
		printf("Bit %u is flipped %u times\n", ii, metadata[ii*2+1]);
		if(metadata[ii*2+1] == 1) {
			if( ){
				printf("must keep %u\n", metadata[ii*2]);
				possible[ii] = MASK_KEEP;
			} else
				printf("must remove %u\n", metadata[ii*2]);

		}
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
