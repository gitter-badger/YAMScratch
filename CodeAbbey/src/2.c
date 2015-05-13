#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <string.h> /*provides strsep*/


int main(int argc, char* argv[]) {
	char * lineptr, * token ;
	size_t nbytes;
	ssize_t bytes_read;
	int N, ii, jj, token_length;
	char delimeter;
	lineptr = NULL; /*cause getline to allocate buffer for us*/
	nbytes = 0;
	bytes_read = getline(&lineptr, &nbytes, stdin);
	if(bytes_read < 0) {
		/*this checks for when there is only EOF */
		errno = EINVAL;
		perror("Must input a number");
		exit(EINVAL);
	} else if(bytes_read == 1) {
		/*check for newline ignoring specific EOL*/
		if(*lineptr == '\n') {
			errno = EINVAL;
			perror("Please input a number");
			exit(EINVAL);
		}
	}
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
	bytes_read = getline(&lineptr, &nbytes, stdin);
	if(bytes_read < 0) {
		/*this checks for when there is only EOF */
		errno = EINVAL;
		perror("Must input a number");
		exit(EINVAL);
	} else if(bytes_read == 1) {
		/*check for newline ignoring specific EOL*/
		if(*lineptr == '\n') {
			errno = EINVAL;
			perror("Please input a number");
			exit(EINVAL);
		}
	}
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
	for(ii = 0; ii < bytes_read; ii++){

		token = lineptr + ii;
		if( lineptr[ii] == delimeter) {
			token_length = 0;
			lineptr[ii] = '\0'; /*make a token string by null terminating*/
			printf("Token: %s\n",token); /*emit the token*/
			if(++jj > N) {
				break;
			}

		} else if( lineptr[ii] == '\n') {
			token_length = 0;
			lineptr[ii] = '\0';
			/*check that all tokens have been read in*/
			if(jj++ < N) {
				errno = EPROTO;
				perror("Reached end of line before all tokens were parsed");
				exit(-1);
			}

			printf("Token: %s\n", token);

		}
	}
	printf("%s\n", lineptr);
	return 0;
}
