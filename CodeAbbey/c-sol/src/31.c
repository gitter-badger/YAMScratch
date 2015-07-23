#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

#include "yam_vector.h"
VECTOR_INIT(char)

#define NULL_CHECK(ptr, msg) 	\
	if(ptr == NULL) {			\
		perror(msg);			\
		exit(-1);				\
	}


int main(int argc, char const *argv[])
{
	/*rotate string by slice*/
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	Vector_t(char)* in_buff, * out_buff ;
	in_buff = newVector(char);
	NULL_CHECK(in_buff, "failed to allocate input buffer vector");
	out_buff = newVector(char);
	NULL_CHECK(out_buff, "failed to allocate output buffer vector");
	/*reserve reasonable amount of space*/
	vector_resize(char, in_buff, 128);
	vector_resize(char, out_buff, 128);

	long K;
	int c;
	unsigned ii, jj;
	for(ii = 0; ii < N; ++ii) {
		vector_erase(char, in_buff);
		vector_erase(char, out_buff);
		errno = 0;
		rc = scanf(" %ld ", &K);
		if(rc != 1 || errno != 0) {
			perror("input read failed");
			vector_destroy(char, in_buff);
			vector_destroy(char, out_buff);
			exit(-1);
		}
		for(;;) {
			errno = 0;
			c = getc(stdin);
			/*strip newline from from */
			if(c == '\n') {
				break;
			} else if(c == EOF){
				break;
			}
			vector_push_back(char, in_buff, (char)c);
		}
		/*we will separate the string into two sections and
		* paste it together. It is always possible to buy more space
		* but one can never purchase more time. First copy the right
		* slice into the output buffer, then the left slice*/
		if(K < 0) {
			K += in_buff->elms;
		}
		for(jj= K; jj < in_buff->elms; ++jj) {
			vector_push_back(char, out_buff, in_buff->items[jj]);
		}
		for (jj = 0; jj < K; ++jj) {
			vector_push_back(char, out_buff, in_buff->items[jj]);
		}
		vector_push_back(char, out_buff, '\0');
		printf("%s ", out_buff->items);
	}
	printf("\n");
	vector_destroy(char, in_buff);
	vector_destroy(char, out_buff);
	return 0;
}
