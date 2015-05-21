#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <math.h>

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
	long A, tmp;
	tmp = N;
	operation = '\0';
	/*add one to N to account for the final mod operation*/
	ii = 0;
	while(operation != '%') {
		rc = scanf(" %c %lu", &operation, &A);
		printf("%c : %lu \n",operation, A );
		if(rc != 2 || !rc) {
			errno = EINVAL;
			fprintf(stderr, "Failed on input line: %d  %d\n", ii+2, rc );
			perror("Enter a char and a interger each line.");
			exit(-1);
		}
		switch(operation) {
			case '+':
				tmp += A;
				break;
			case '-':
				tmp -= A;
				break;
			case '*':
				tmp *= A;
				break;
			case '/':
				tmp /= A;
				break;
			case '%':
				tmp %= A;
				break;
		}
		++ii; /*for error reporting only*/

	}
	printf("%lu\n", tmp);
	return 0;
}
