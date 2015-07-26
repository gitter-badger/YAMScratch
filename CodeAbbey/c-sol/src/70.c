#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "code_abbey_utility.h"

/*BUFF SIZE must be at least 2 to allow for null termination*/
#define BUFF_SIZE 10
#define WORD_LIST_SIZE 900000

int main(int argc, char const *argv[])
{
	unsigned long X0;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &X0);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	unsigned long Xcurr;
	Xcurr = X0;

	char* buffer;
	errno = 0;
	buffer = (char*)malloc(BUFF_SIZE * sizeof(char));
	if(buffer == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	}
	char* consonants, * vowels;
	consonants = "bcdfghjklmnprstvwxz";
	vowels = "aeiou";
	/*seed the linear generator*/
	unsigned long A, C, M;
	A = 445UL;
	C = 700001UL;
	M = 2097152UL;
	char max_buff[BUFF_SIZE];
	unsigned ii, jj, K, flag;
	int count, max_count;
	struct prefix_node* root;
	root = prefix_node_init();
	max_count = 0;
	/*fill with empty string*/
	max_buff[0] = '\0';
	for(ii = 0; ii < WORD_LIST_SIZE; ++ii) {
		flag = 1; /*start with consonant*/
		K = 6; /*generate  chars*/
		/*process chunks until we have produced K chars*/
		for(;K > 0;) {
			/*walk over buffer generating the characters*/
			for(jj = 0; (jj < BUFF_SIZE -1) && K > 0; ++jj, --K) {
				Xcurr = linear_congruent_gen(A, C, M, Xcurr);
				//printf("%lu\n", Xcurr);
				if(flag == 1) {
					buffer[jj] = consonants[Xcurr%19];
				} else {
					buffer[jj] = vowels[Xcurr%5];
				}
				flag = (flag) ? 0 : 1;
			}
			/*null terminate the buffer*/
			buffer[jj] = '\0';
			//printf("%s\n", buffer);
			count = insert_word(root, buffer, strlen(buffer));
			if(count == -1) {
				/*internal failure*/
				perror("internal failure");
				exit(-1);
			}
			if(count > max_count) {
				max_count = count;
				strncpy(max_buff, buffer, BUFF_SIZE);
				//printf("new max %d = %s\n",max_count, max_buff);
			}
		}
		/*print a space separator*/
		//printf(" ");
	}
	printf("%s\n", max_buff);
	destroy_prefix_tree(root);
	free(buffer);
	return 0;
}
