#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

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
VECTOR_INIT(unsigned)

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

/*========================================================*/
struct MetaData {
	unsigned bins;
	unsigned* offsets; /*has bins + 1 total elements*/
	unsigned* grid;
};
typedef unsigned* MetaData_iterator;

MetaData_iterator MetaData_row_start(struct MetaData* md, unsigned row) {
	/*sanity check*/
	if(row > md->bins) {
		return NULL;
	}
	return md->grid + md->offsets[row];
}

MetaData_iterator MetaData_row_end(struct MetaData* md, unsigned row) {
	if(row > md->bins) {
		return NULL;
	}
	return md->grid + md->offsets[row+1];
}

void MetaData_init(struct MetaData* md, unsigned bins, unsigned* bin_counts, 
					struct MaskData* mask_buff, unsigned len_mask_buff) {
	/*sanity checks*/
	NULL_CHECK(md, "NULL ptr: cannot initialize a null pointer");
	NULL_CHECK(bin_counts, "NULL ptr: must pass a valid bin array");
	NULL_CHECK(mask_buff, "NULL ptr: must pass a valid MaskData array");
	if(len_mask_buff == 0) return;
	if(bins == 0) return;
	unsigned ii, jj, kk, total, tmp_key;
	md->bins = bins;
	errno = 0;
	md->offsets = (unsigned*)malloc((bins+1) * sizeof(unsigned));
	NULL_CHECK(md->offsets, "failed to allocate metadata offset buffer");
	/*compute the total number of grid elements to allocate*/
	total = 0;
	for(ii = 0; ii < bins; ++ii) {
		md->offsets[ii] = total;
		total += bin_counts[ii];
	}
	/*and get last one*/
	md->offsets[ii] = total;

	errno = 0;
	md->grid = (unsigned*)malloc(total * sizeof(unsigned));
	NULL_CHECK(md->grid, "failed to allocate metadata grid table");
	/*create a temporary offset table to keep track of insertions*/
	unsigned* temp_offsets;
	errno = 0;
	temp_offsets = (unsigned*)calloc(md->bins, sizeof(unsigned));
	NULL_CHECK(temp_offsets, "failed to allocate temp buffer");
	for(ii = 0; ii < len_mask_buff; ++ii) {
		for(jj = 0; mask_buff[ii].len; ++jj) {
			tmp_key = mask_buff[ii].bits[jj];
			/*make sure bit number is within range of bits metadata
			* is keeping track of*/
			assert(tmp_key < md->bins);
			kk = temp_offsets[tmp_key] + md->offsets[tmp_key];
			assert(kk < md->offsets[tmp_key+1]);
			md->grid[kk] = tmp_key;
			temp_offsets[tmp_key]++;
		}
	}
	/*make sure metadata is internally consistant:
	* the number of elements set in the preceding step should match
	* exactly with the size of each bin in the grid*/
	for(ii = 0; ii < md->bins; ++ii) {
		assert(temp_offsets[ii] == (md->offsets[ii+1] - md->offsets[ii]));
	}
}

void print_MetaData(struct MetaData* md) {
	unsigned ii;
	if(md == NULL) {
		return;
	}

}
/*===============================================================*/

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

int stream_from_single_line(FILE* in_stream, FILE ** out_stream) {
	char* lineptr;
	size_t nbytes;
	ssize_t bytes_read;
	lineptr = NULL;
	nbytes = 0;
	errno = 0;
	bytes_read = getline(&lineptr, &nbytes, in_stream);
	if(bytes_read < 0) {
		/*this checks for when there is only EOF */
		return EOF;
	}
	*out_stream = fmemopen(lineptr, bytes_read, "r");
	free(lineptr);
}

int main(int argc, char const *argv[])
{
	int rc;
	unsigned ii, jj, kk, A, TV, E, T, total_bits_set;
	unsigned long N;
	/*read first line with unknown length of data*/
	FILE * stream;
	rc = stream_from_single_line(stdin, &stream);
	if(rc == EOF) {
		perror("end of input reached before expected");
		exit(-1);
	}
	Vector_t(unsigned)* _target_set_bits;
	_target_set_bits = newVector(unsigned);
	NULL_CHECK(_target_set_bits, "failed to allocate new vector");

	N = 0;
	TV = 0;
	for(;;) {
		rc = fscanf(stream," %u ", &A);
		if(rc == EOF) {
			break;
		}
		/*A should be 1 or zero*/
		assert(A < 2);
		/*this stores K as little endian, position 0 is represented by the low bit*/
		vector_push_back(unsigned, _target_set_bits, A);
		++N;
	}
	fclose(stream);
	/*use an array for programming convience to represent which
	* bits of the target value are set*/
	unsigned* target_bits;
	target_bits = (unsigned*)malloc(N*sizeof(unsigned));
	NULL_CHECK(target_bits, "failed to allocate target bit buffer");
	for(ii = 0; ii < N; ++ii) {
		if( _target_set_bits->items[ii] == 1) {
			target_bits[ii] = 1;
		} else {
			target_bits[ii] = 0;
		}
	}

	/* Allocate a buffer to record which indices are set 
	* for use in reading in each mask below*/
	unsigned* index_buffer, *cursor, * bit_counts;
	index_buffer = (unsigned*)malloc(N* sizeof(unsigned));
	NULL_CHECK(index_buffer, "failed to allocate a buffer")
	errno = 0;
	bit_counts = (unsigned*)calloc(N, sizeof(unsigned));
	NULL_CHECK(bit_counts, "failed to allocate metadata buffer")
	/*
	* Store each masks, we will only read up to N masks from the file,
	* this is the defined input format 
	*/
	struct MaskData* mask_buffer;
	mask_buffer = (struct MaskData*)malloc(N * sizeof(struct MaskData));
	NULL_CHECK(mask_buffer, "failed to allocate buffer for all masks");
	/*keep track total bits set for building the metadata offset table*/
	for(ii = 0; ii < N; ++ii) {
		rc = stream_from_single_line(stdin, &stream);
		if(rc == EOF) {
			/*no more data from stream coming*/
			break;
		}
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
			mask_buffer[ii].key = E;
			/*walk over buffer starting from begining*/
			cursor = index_buffer;
			/*the below code depends on these two fields being zeroed*/
			mask_buffer[ii].len = 0;
			mask_buffer[ii].value = 0;
			for(;;) {
				rc = fscanf(stream," %u ", &A);
				if(rc == EOF) {
					break;
				}
				/*value will be pretty useless if there are more than 64 masks*/
				mask_buffer[ii].value |= 1<<A;
				mask_buffer[ii].len++;
				/*store and advance buffer*/
				*cursor++ = A;
				bit_counts[A]++;
			}
		}
		fclose(stream);
		/*check to see if pattern already exists*/
		/*allocate an appropriate size buffer for the MaskData
		* instance and copy valid contents of buff into*/
		mask_buffer[ii].bits = (unsigned*)malloc(mask_buffer[ii].len * sizeof(unsigned));
		NULL_CHECK(mask_buffer[ii].bits, "failed to allocate mask bit count");
		for(kk = 0; kk < mask_buffer[ii].len; ++kk) {
			mask_buffer[ii].bits[kk] = index_buffer[kk];
		}

	}
	/*we wont use index_buffer again*/
	free(index_buffer); index_buffer = NULL;
	/*======END OF READING INPUT==========*/
	
	/*allocate a buffer that marks masks as available*/
	char* possible;
	possible = (char*)malloc(N * sizeof(char));
	NULL_CHECK(possible, "failed to allocate buffer for flags");
	memset(possible, MASK_INACTIVE, N*sizeof(char));
	/*copy all masks into an array that will act as set with contant tim
	* lookup by mask.key == index*/
	for(ii = 0; ii < _masks->elms ; ++ii) {
		/*we have already asserted that each key is less than N*/
		mask_buffer[_masks->items[ii].key] = _masks->items[ii];
		possible[_masks->items[ii].key] = MASK_ACTIVE;
	}
	/*remove any masks that are the only one to set the bit
	* only masks that are possible to be removed */
	unsigned removed;
	do {
		removed = 0;
		for(ii = 0; ii < N; ++ii) {
			if(mask_total_bit_counts[ii] == 1) {
				/*find mask which sets this bit*/

				
				if(target_bits[ii] == 1 && possible[ii] == MASK_ACTIVE){
					possible[ii] = MASK_KEEP;
					++removed;
				} else {
					possible[ii] = MASK_INACTIVE;
					/*change the bit counts in metadata to reflect change*/
					mask_total_bit_counts
				}
			}
		}
		printf("removed %u\n", removed);
	} while(removed != 0);
	
	printf("\n");

	/*free each of the MaskData bits arrays*/
	for(ii = 0; ii < _masks->elms; ++ii) {
		print_MaskData(&_masks->items[ii]);
		printf("\n");
		MaskData_clear(_masks->items[ii]);
	}
	vector_destroy(struct_MaskData, _masks);
	free
	return 0;
}
