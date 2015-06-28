#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	unsigned N;
	int rc;
	rc = scanf("%u ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	/*allocate buffer for result*/
	char* result = (char*)calloc(N, sizeof(char));
	if(result == NULL) {
		perror("failed to allocate output buffer");
		exit(-1);
	}
	ssize_t bytes_read;
	char* lineptr;
	lineptr = NULL;
	size_t buffer_size;
	buffer_size = 0;
	unsigned ii, jj, kk;
	/*check each line for palindromes*/
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		bytes_read = getline(&lineptr, &buffer_size, stdin);
		if(errno == EINVAL) {
			perror("failed to read input line");
			exit(-1);
		}
		/*front index and back index taking into account terminating */
		if(bytes_read < 2) {
			/*the result is an empty string, which is not a palindrome*/
			result[ii] = 'N';
		} else {
			/*filter so that only character a-zA-Z remain*/
			char* filtered;
			/*
			* the buffer is the size of bytes_read so that the newline can be overwritten
			* with a null for printing when debugging
			*/
			filtered = (char*)malloc(bytes_read * sizeof(char));
			kk = 0;
			for(jj = 0; jj < bytes_read -1; ++jj) {
				if(lineptr[jj] <= 'Z') {
					if(lineptr[jj] >= 'A') {
						/*ignore case by pushing to upper case*/
						filtered[kk] = lineptr[jj] + 32;
						++kk;
					}

				} else if(lineptr[jj] <='z') {
					if(lineptr[jj] >= 'a') {
						filtered[kk] = lineptr[jj];
						++kk;
					}
				}
			}
			/*null terminate the buffer*/
			filtered[kk] = '\0';
			unsigned front, back;
			front = 0;
			if(kk < 1) {
				/*this is case of empty string after filetering*/
				result[ii] = 'N';
			} else if (kk == 1) {
				/*single character string is a palindrome*/
				result[ii] = 'Y';
			} else {
				back = kk - 1;
				while(front <= back) {
					if(filtered[front] != filtered[back]) {
						result[ii] = 'N';
						break;
					} else {
						++front;
						--back;
					}
				}
				if(front > back) {
					/*in this case the entire loop occured*/
					result[ii] = 'Y';
				}
			}
			free(filtered);
		}
		/*the buffer is reused*/
	}
	free(lineptr);
	for(ii = 0; ii < N; ++ii) {
		printf("%c ", result[ii]);
	}
	printf("\n");
	return 0;
}
