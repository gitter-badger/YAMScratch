#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <string.h> /*provides strsep*/

#include "wrapped.h"

int main(int argc, char* argv[]) {
	char * lineptr, * token ;
	size_t nbytes;
	ssize_t bytes_read;
	int N, ii, jj, token_length;
	long running_sum;
	char delimeter;
	lineptr = NULL; /*cause getline to allocate buffer for us*/
	nbytes = 0;

	bytes_read = wrap_getline(&lineptr, &nbytes, stdin);

	/*assume past here that the first line is a valid integer*/
	N = atoi(lineptr);
	if(!N){
		errno = EINVAL;
		perror("Please enter an integer > 0");
		exit(EINVAL);
	}
	free(lineptr);
	/*get the list of numbers all on one line */
	lineptr = NULL;
	nbytes = 0;
	bytes_read = wrap_getline(&lineptr, &nbytes, stdin);
	/*
	* iterate over line buffer and change all delimeter characters to nulls
	* yielding pointers the start of each subsegment
	* we search for a valid digit, if so, we increment token length
	* it is assumed that the input is of format:
	*     <int>DELIMETER_CHAR<int>DELINETER_CHAR<int>....\n
	* therefore we move along the line buffer and find each bucket holding an int
	* we stop once we hit a newline
	*/
	jj = 0;
	token_length = 0;
	delimeter = ' ';
	running_sum = 0;
	for(ii = 0; ii < bytes_read; ii++){
		if( lineptr[ii] == delimeter) {
			token_length = 0;
			lineptr[ii] = '\0'; /*make a token string by null terminating*/
			running_sum += atoi(token);
			if(++jj > N) {
				break;
			}
		} else if( lineptr[ii] == '\n') {
			token_length = 0;
			lineptr[ii] = '\0';
			running_sum += atoi(token);
			/*check that all tokens have been read in*/
			if(++jj > N) {
				errno = EPROTO;
				perror("Reached end of line before all tokens were parsed");
				exit(-1);
			}
		} else {
			token_length++;
		}
		/*started new token*/
		if(token_length == 1) {
			token = lineptr + ii;
		}
	}
	/*at the end, make sure that we found the appropriate number of tokens*/
	if(jj != N) {
		errno = EINVAL;
		perror("Number of tokens did not match input");
		exit(-1);
	}
	token = NULL;
	free(lineptr); lineptr = NULL;
	printf("%ld\n", running_sum);
	return 0;
}
