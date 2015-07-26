#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

/*prefix string structure for lowercase letters*/
#define NUM_CHILDREN 26

struct prefix_node;

struct prefix_node {
	unsigned value;
	struct prefix_node* children[NUM_CHILDREN];
};

struct prefix_node* prefix_node_init() {
	errno = 0;
	struct prefix_node* node;
	node = malloc(sizeof (*node));
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

int main(int argc, char const *argv[])
{
	int rc;
	/*oversize buffer for 3 character words*/
	char in_buff[10];
	char most_buffer[10];
	/*allocate prefix string*/
	struct prefix_node* root;
	root = prefix_node_init();
	unsigned count;
	for(;;) {
		errno = 0;
		rc = scanf(" %9s ", in_buff);
		if(errno != 0) {
			perror("input read failure in scanf");
			destroy_prefix_tree(root);
			exit(-1);
		}
		if(rc == EOF) {
			break;
		}
		if(strcmp("end", in_buff) == 0) {
			break;
		}
		count = insert_word(root, in_buff, strlen(in_buff));
		if(count == -1) {
			/*internal failure*/
			perror("internal failure");
			exit(-1);
		}

	}
	print_tree_lexographic_order(root, most_buffer, 0, 3);
	printf("\n");
	destroy_prefix_tree(root);
	return 0;
}
