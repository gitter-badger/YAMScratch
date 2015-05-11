#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <string.h> /*provides strsep*/


int main(int argc, char* argv[]) {
	char* lineptr;
	size_t nbytes;
	ssize_t bytes_read;
	int N;
	lineptr = NULL; /*cause getline to allocate buffer for us*/
	nbytes = 0;
	bytes_read = getline(&lineptr, &nbytes, stdin);
	if(bytes_read < 0) {
		perror("Error in getline");
		exit(EXIT_FAILURE);
	} else if(bytes_read == 0) {
		perror("zero bytes read");
		exit(-1);
	}

	fprintf(stdout, "[%s]\n", lineptr );
	fflush(stdout);
	printf("%d\n", atoi(lineptr) );


	return 0;

}