#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <math.h>
#include <limits.h>
#include <assert.h>

#include "parseCodeAbbeyInput.h"

VECTOR_INIT(long)
VECTOR_INIT(char)

int main(int argc, char* argv[]) {
	unsigned N;
	int rc;
	rc = scanf("%u", &N);
	if(rc != 1 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}
	/*compute sum of arithmatic sequence*/
	unsigned ii;
	char operation;
	unsigned long A, tmp;
	operation = '\0';
	Vector(long)* num = newVector(long);
	Vector(char)* op_code = newVector(char);
	enum operations {PLUS, MINUS, MULTIPLY, DIVIDE, MODULO};

	ii = 0;


	while(operation != '%') {
		rc = scanf(" %c %lu", &operation, &A);
		if(rc != 2 || !rc) {
			errno = EINVAL;
			fprintf(stderr, "Failed on input line: %d  %d\n", ii+2, rc );
			perror("Enter a char and a interger each line.");
			exit(-1);
		}
		switch(operation) {
			case '+':
				vector_push_back(char, op_code, PLUS);
				vector_push_back(long, num, A);
				break;
			case '-':
				vector_push_back(char, op_code, MINUS);
				vector_push_back(long, num, A);
			case '*':
				vector_push_back(char, op_code, MULTIPLY);
				vector_push_back(long, num, A);
				break;
			case '/':
				vector_push_back(char, op_code, DIVIDE);
				vector_push_back(long, num, A);
				break;
			case '%':
				vector_push_back(char, op_code, MODULO);
				vector_push_back(long, num, A);
				break;
		}
		++ii; /*for error reporting only*/
	}
	/*now the modulo should be the last item on the vector*/
	assert(num->elms == op_code->elms);
	long mod = num->items[num->elms-1];
	tmp = N % mod;
	for(ii = 0; ii < num->elms-1; ++ii){
		switch(op_code->items[ii]) {
			case PLUS:
				tmp += (num->items[ii] % mod); 
				tmp %= mod;
				break;
			case MULTIPLY:
				tmp *= (num->items[ii] % mod);
				tmp %= mod;
				break;
		}
	}
	printf("%lu\n", tmp);

	vector_destroy(long, num);
	vector_destroy(char, op_code);
	return 0;
}
