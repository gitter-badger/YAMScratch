#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/


int main(int argc, char* argv[]) {
	char* lineptr;
	size_t nbytes;
	ssize_t bytes_read;
	lineptr = NULL; /*cause getline to allocate buffer for us*/
	nbytes = 0;
	bytes_read = getline(&lineptr, &nbytes, stdin);
	fprintf(stdout, "[%s]\n", lineptr );
	fflush(stdout);
	return 0;

}