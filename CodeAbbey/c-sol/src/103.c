#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#include "yam_vector.h"
#include "code_abbey_utility.h"
#include "yam_sorting.h"

struct MaskData {
	unsigned long key;	/*unique value*/
	unsigned long value; /*value we use can use to compare objects*/
	size_t len; /*number of bits set in mask*/
	unsigned* bits; /*array indicating which indices are set, is malloced and freed*/
};

unsigned less_than(struct MaskData** A, struct MaskData** B) {
	/*use the key as the */
	if((*A)->value < (*B)->value) {
		return 1;
	} else if((*A)->value > (*B)->value) {
		return 0;
	} else {
		/*if the keys are the same, check the length*/
		if((*A)->len < (*B)->len) {
			return 1;
		} else if((*A)->len > (*B)->len) {
			return 0;
		} else {
			/*check the values for the array*/
			long checksumA, checksumB;
			unsigned ii;
			checksumA = 0; checksumB = 0;
			for(ii = 0; ii < (*A)->len; ++ ii) {
				checksumA += (*A)->bits[ii];
				checksumA *= 113;
				checksumA %= 10000007;
				checksumB += (*B)->bits[ii];
				checksumB *= 113;
				checksumB %= 10000007;
			}
			/*make sure A<B and B< A both are true*/
			return (checksumA < checksumB) ? 1 : 0;
		}
	}
}

/*make single token name for use in TYPE macros*/
typedef struct MaskData struct_MaskData;
typedef struct MaskData* MaskData_ptr;

VECTOR_INIT(struct_MaskData)
VECTOR_INIT(unsigned)
QUICKSORT_INIT(MaskData_ptr)

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

#define MASK_ACTIVE 'A'
#define MASK_INACTIVE 'X'
#define MASK_KEEP 'K'

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
		for(jj = 0; jj < mask_buff[ii].len; ++jj) {
			/*ignore empty masks*/
			if(mask_buff[ii].len > 0) {
				tmp_key = mask_buff[ii].bits[jj];
				/*make sure bit number is within range of bits metadata
				* is keeping track of*/
				assert(tmp_key < md->bins);
				kk = temp_offsets[tmp_key] + md->offsets[tmp_key];
				assert(kk < md->offsets[tmp_key+1]);
				md->grid[kk] = ii;
				temp_offsets[tmp_key]++;
			}
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
	MetaData_iterator iter, end;
	if(md == NULL) return;
	printf("MetaData %p\n", (void*)md);
	for(ii = 0; ii < md->bins; ++ii) {
		printf("%u: ", ii);
		end = MetaData_row_end(md, ii);
		iter = MetaData_row_start(md, ii);
		if((end - iter) == 0) {
			printf("None");
		}
		for(; iter != end; ++iter) {
			printf("%u ", *iter);
		}
		printf("\n");
	}
}
/*===============================================================*/

int remove_one(unsigned N, unsigned* bit_counts, unsigned* target_bit_counts,
				unsigned lower_bound, unsigned* possible, struct MaskData* masks) {
	unsigned ii, jj, kk, flag;
	/*check if we have a solutions*/
	flag = 1;
	for(ii = 0; ii < N; ++ii) {
		if(target_bit_counts[ii] != (bit_counts[ii] % 2) ){
			flag = 0;
			break;
		}
	}
	if(flag == 1) {
		printf("found solution\n%s\n", possible);
		/*sucess*/
		return 1;
	}

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
	if(n_possible == 0) {
		/*we cannot go further*/
		return 0;
	}
	/*now recurse*/
	unsigned tmp_index;
	for(ii = 0; ii < n_possible; ++ii) {
		/*remove the bits from count*/
		for(jj = 0; jj < masks[possible_take_away[ii]].len; ++jj) {

		}
		possible[possible_take_away[ii]] = MASK_INACTIVE;

	}
	return 0;
}

int main(int argc, char const *argv[])
{
	int rc;
	unsigned ii, jj, kk, A, TV, E;
	unsigned long N;
	char *lineptr;
	size_t nbytes;
	ssize_t bytes_read;
	lineptr = NULL;
	nbytes = 0;
	bytes_read = 0;
	FILE * stream;
	/*read first line with unknown length of data*/
	errno = 0;
	bytes_read = getline(&lineptr, &nbytes, stdin);
	if(bytes_read < 0) {
		/*this checks for when there is only EOF */
		perror("reached end of input stream before expected");
		exit(-1);
	}
	stream = fmemopen(lineptr, bytes_read, "r");

	Vector(unsigned)* _target_set_bits;
	_target_set_bits = newVector(unsigned);
	NULL_CHECK(_target_set_bits, "failed to allocate new vector");

	N = 0;
	TV = 0;
	for(;;) {
		rc = fscanf(stream," %u ", &A);
		if(rc != 1) {
			if(feof(stream)) break;
			if(ferror(stream)) {
				perror("error reading input stream");
				exit(-1);
			}
			break;
		}
		/*A should be 1 or zero*/
		assert(A < 2);
		/*this stores K as little endian, position 0 is represented by the low bit*/
		vector_push_back(unsigned, _target_set_bits, A);
		++N;
	}
	fclose(stream);
	free(lineptr); lineptr = NULL;
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
	/*initialize each mask with a length of zero*/
	mask_buffer = (struct MaskData*)calloc(N,  sizeof(struct MaskData));
	NULL_CHECK(mask_buffer, "failed to allocate buffer for all masks");
	/*keep track total bits set for building the metadata offset table*/

	for(ii = 0; ii < N; ++ii) {
		lineptr = NULL;
		nbytes = 0;
		errno = 0;
		bytes_read = getline(&lineptr, &nbytes, stdin);
		if(bytes_read < 0) {
			/*this checks for when there is only EOF */
			perror("reached end of input stream before expected");
			exit(-1);
		} if(bytes_read == 1) {
			if(*lineptr == '\n'){
				continue;
			} else {
				perror("unexpected input data");
				exit(-1);
			}
		}
		stream = fmemopen(lineptr, bytes_read, "r");

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
	struct MetaData meta;
	MetaData_init(&meta, N, bit_counts, mask_buffer, N);
	/*
	print_MetaData(&meta);
	*/
	/*allocate a buffer that marks masks as available*/
	char* possible;
	possible = (char*)malloc((N+1) * sizeof(char));
	NULL_CHECK(possible, "failed to allocate buffer for flags");
	memset(possible, MASK_ACTIVE, N*sizeof(char));
	/*null terminate so we can print it directly for debugging*/
	possible[N] = '\0';

	/*remove duplicates by creating a shadow pointer buffer and sorting
	* those pointers*/
	unsigned (*less)(struct MaskData**, struct MaskData**);
	less = &less_than;
	struct MaskData** shadow;
	shadow = (struct MaskData**)malloc(N * sizeof(struct MaskData*));
	NULL_CHECK(shadow, "failed to allocate shadow buffer");
	for(ii = 0; ii < N; ++ii) {
		shadow[ii] = mask_buffer+ii;
	}

	yam_quicksort(MaskData_ptr,shadow, 0, N-1, less);
	for(ii = 1; ii < N; ii++) {
		if((less_than(&shadow[ii-1], &shadow[ii]) == 0) && \
					(less_than(&shadow[ii], &shadow[ii-1]) == 0)) {
			possible[shadow[ii]->key] = MASK_INACTIVE;
			/*remove the bit counts*/
			for(jj = 0; jj < shadow[ii]->len; ++jj) {
				--bit_counts[shadow[ii]->bits[jj]];
			}
		}
	}
	printf("After eliminating duplicates\n%s\n\n", possible);		
	/*remove any masks that are the only one to set the bit
	* only masks that are possible to be removed */
	unsigned removed;
	MetaData_iterator md_iter, end;
	do {
		removed = 0;
		for(ii = 0; ii < N; ++ii) {
			if(bit_counts[ii] == 1) {
				/*find mask which sets this bit:
				* so first get the bin of all masks that set this
				* and find the one that is marked active*/
				md_iter = MetaData_row_start(&meta, ii);
				end = MetaData_row_end(&meta, ii);
				for(; md_iter != end; ++md_iter) {
					if(possible[*md_iter] != MASK_INACTIVE) {
						break;
					}
				}
				if(possible[*md_iter] == MASK_KEEP) {
					continue;
				}
				
				if(target_bits[ii] == 1){
					possible[ii] = MASK_KEEP;
					++removed;
				} else {
					possible[ii] = MASK_INACTIVE;
					/*change the bit counts in metadata to reflect change*/
					for(jj = 0; jj < mask_buffer[*md_iter].len; ++jj) {
						--bit_counts[mask_buffer[*md_iter].bits[jj]];
					}
					++removed;
				}
			}
		}
	} while(removed != 0);
	printf("After first pass filtering:\n\t%s", possible);

	printf("\n");
	return 0;
}
