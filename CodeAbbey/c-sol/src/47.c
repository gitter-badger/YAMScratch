#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	/*encrypt a message with casear shift of K*/
	unsigned long N, K;
	int rc;
	errno = 0;
	rc = scanf(" %lu %lu ", &N, &K);
	if(rc != 2) {
		perror("failed to read input");
		exit(-1);
	}
	/*
	* Each line is terminate with a '.' char, we
	* read only N lines from input file, throw error
	* if there are not N lines.
	* The message was encoded with a shift of K, so we
	* decode with a shift of (26 - K) 
	*/
	K = 26 - K;
	unsigned ii;
	int c;
	int dot_flag;
	for(ii = 0; ii < N; ++ii) {
		/*process the input as a stream one char at a time*/
		dot_flag = 1;
		for(;dot_flag;) {
			/*strip out newlines for this application only*/
			errno = 0;
			c = getc(stdin);
			if(c == EOF) {
				if(feof(stdin)) {
					break;
				} else if(ferror(stdin)) {
					/*error with reading input, exit so we can try again*/
					perror("error reading character from input");
					exit(-1);
				} else {
					/*should never reach*/
					perror("should never reach");
					exit(-1);
				}
			}
			switch(c) {
				case '\r':
					/*absorb this character for newlines on windows machines*/
					break;
				case '\n':
					/*place a space to separate*/
					c = ' ';
					rc = putc(c, stdout);
					break; 
				case ' ':
					rc = putc(c, stdout);
					break;
				case '.':
					dot_flag = 0; /*trigger termination of loop*/
					rc = putc(c, stdout);
					break;
				default:
					/*change only the capital letters*/
					if(c <= 'Z') {
						if(c >= 'A') {
							c -= 'A';
							c += K;
							c %= 26;
							c += 'A';
						}
					}
					rc = putc(c, stdout);
					break;
			}
			/*check the return code from put char*/
			if(rc == EOF) {
				perror("error in writing to output stream");
				exit(-1);
			}
		}
	}
	printf("\n");
	return 0;
}
