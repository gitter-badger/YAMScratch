#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include "code_abbey_utility.h"

signed array_checksum(long const* array, size_t size, long* checksum) {
	*checksum = 0;
	size_t ii;
	for(ii = 0; ii < size; ++ii) {
		*checksum += array[ii];
		*checksum *= 113;
		*checksum %= 10000007;
	}
	return 0;
}

uint64_t modular_exponentiation(uint64_t A, uint64_t B, uint32_t M){
	uint64_t result, accum, mask;
	/*sanity checks*/
	if(B == 0) {
		return 1;
	}
	if(M == 0){
		return 0;
	}
	/*base case (A^1)%M*/
	accum = A % M;
	mask = 1;
	if((mask & B)) {
		result = accum;
	} else {
		result = 1;
	}
	uint32_t ii;
	for(ii = 1; ii < 64; ++ii) {
		accum *= accum;
		accum %= M;
		/*now accum is (A^ii)%M, see if it contributes*/
		if(((mask << ii) & B)) {
			result *= accum;
			result %= M;
		}
	}
	return result;
}

unsigned long linear_congruent_gen(unsigned long A, unsigned long C,
								unsigned long M, unsigned long Xcurr) {
	/*make the chance of overflow smaller*/
	A %= M;
	Xcurr %= M;
	C %= M;
	return ((A * Xcurr) % M + C ) % M;
}
/*Prefix trees for strings*/

struct prefix_node* prefix_node_init() {
	errno = 0;
	struct prefix_node* node;
	node = (struct prefix_node *)malloc(sizeof (*node));
	if(node == NULL) {
		return node;
	}
	/*set all of the children to NULL*/
	unsigned ii;
	for(ii =0; ii < NUM_CHILDREN; ++ii) {
		node->children[ii] = NULL;
	}
	node->value = 0;
	return node;
}
/*must be lower case word*/
int insert_word(struct prefix_node* root, char* buff, size_t len) {
	/*sanity checks*/
	if(len == 0) return 0;
	/*root node is direct value comparison*/
	unsigned long ii, key;
	struct prefix_node* cursor;
	cursor = root;
	for(ii = 0; ii < len; ++ii) {
		/*skip all non lower case ascii characters*/
		if(buff[ii] <= 'z') {
			if(buff[ii] >= 'a'){
				key = buff[ii] - 'a';
				assert(key < NUM_CHILDREN);
				if(cursor->children[key] == NULL) {
					cursor->children[key] = prefix_node_init();
					if(cursor->children[key] == NULL) {
						/*failure to allocate new node*/
						return -1;
					}
				}
				cursor = cursor->children[key];
			}
		}
	}
	/*now cursor points to leaf node*/
	cursor->value++;
	/*return how many times the word occurs*/
	return cursor->value;
}

void destroy_prefix_tree(struct prefix_node* root) {
	unsigned ii;
	/*check if each of children nodes are empty, if not destroy them*/
	for(ii = 0; ii < NUM_CHILDREN; ++ii) {
		if(root->children[ii] != NULL) {
			destroy_prefix_tree(root->children[ii]);
		}
	}
	free(root);
}

void print_tree_lexographic_order(struct prefix_node* root, char* print_buff, unsigned level, size_t max_depth) {
	/*sanity checks*/
	if(max_depth == 0) return;
	//printf("level %d\n", level);
	/*now traverse the tree*/
	unsigned ii, flag; 
	flag = 0; /*keep track if this level is all empty*/
	for(ii = 0; ii < NUM_CHILDREN; ++ii) {
		if(root->children[ii] != NULL) {
			flag = 1;
			/*only traverse deeper if we are not at max level*/
			if(level < max_depth) {
				print_buff[level] = 'a' + ii;
				print_tree_lexographic_order(root->children[ii], print_buff, level+1, max_depth);
			}
		}
	}
	//printf("flag is %d\n", flag);
	/*if every child was null, print if this word was seen more than once*/
	if(flag == 0 && root->value > 1) {
		print_buff[level] = '\0';
		printf("%s ", print_buff);	
	}
}
