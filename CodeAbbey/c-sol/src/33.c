#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/

#include "parseCodeAbbeyInput.h"
VECTOR_INIT(char)

int main(int argc, char* argv[]) {
	int rc;
	/*use a vector to hold the indeterminant length output*/
	Vector(char)* output = newVector(char);
	/*count the number of set bits to confirm parity*/
	char val;
	unsigned tmp, ii, kk;
	tmp = 0;
	/*the terminating dot charecter does not have the parity bit set*/
	while(tmp != '.') {
		rc = scanf(" %u ", &tmp);
		if(rc != 1 || rc == -1) {
			perror("failed to parse input");
			exit(-1);
		}
		kk = 0;
		for(ii = 0; ii < 8; ++ii){
			kk += ((tmp>>ii) & 1);
		}
		/*check if there is an even number of bits*/
		if(!(kk%2)){
			/*remove the parity bit if it exists*/
			val = (char)(tmp & 127);
			vector_push_back(char, output, val);
		}
	}
	for(ii = 0; ii < output->elms; ++ii){
		printf("%c", output->items[ii]);
	}
	printf("\n");
	return 0;
}
