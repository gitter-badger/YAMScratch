#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/

int main(int argc, char* argv[]) {
	long a,b;

	if(argc > 3) {
		errno = E2BIG;
	} else if(argc < 3) {
		perror("Not enouugh arguments");
		exit(EXIT_FAILURE);
	}
	a = atoi(argv[1]);
	b = atoi(argv[2]);
	printf("%ld\n", (a+b) );
	return 0;
}
