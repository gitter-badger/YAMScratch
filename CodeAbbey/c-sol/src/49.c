#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define BUFF_SIZE 10

int main(int argc, char const *argv[])
{
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	unsigned* result;
	errno = 0;
	result = (unsigned*)malloc(N * sizeof(unsigned));
	if(result == NULL) {
		perror("failed to allocate result buffer");
		exit(-1);
	}
	char* input, *buff;
	errno = 0;
	buff = (char*)malloc(BUFF_SIZE * sizeof(char));
	if(buff == NULL) {
		perror("failed to allocate read buffer");
		exit(-1);
	}

	unsigned ii, jj;
	unsigned player1_score, player2_score;
	char first_throw;
	for(ii = 0; ii < N; ++ii) {
		player1_score = 0;
		player2_score = 0;
		first_throw = 0;
		/*read all lines processing tokens until end of line*/
		for(;;) {
			errno = 0;
			input = fgets(buff, BUFF_SIZE, stdin);
			if(errno != 0) {
				perror("failed to read input");
				exit(-1);
			}
			if(input == NULL) {
				if(ferror(stdin) != 0) {
					fprintf(stderr, "Read failure, ferror = %d\n", ferror(stdin));
				}
				/*reached eof before any new characters could be read*/
				break;
			}
			unsigned bytes_read;
			bytes_read = strlen(input);
			/*autonoma accepting input walking over buffer*/
			for(jj = 0; jj < bytes_read; ++jj) {
				switch(input[jj]) {
					case 'P':
						if(first_throw == 0) {
							first_throw = input[jj];
						} else {
							switch(first_throw) {
								case 'S':
									++player1_score;
									break;
								case 'R':
									++player2_score;
									break;
							}
							/*reset the flag*/
							first_throw = 0;
						}
						break;
					case 'R':
						if(first_throw == 0) {
							first_throw = input[jj];
						} else {
							switch(first_throw) {
								case 'P':
									++player1_score;
									break;
								case 'S':
									++player2_score;
									break;
							}
							/*rest the flag*/
							first_throw = 0;
						}
						break;
					case 'S':
						if(first_throw == 0) {
							first_throw = input[jj];
						} else {
							switch(first_throw) {
								case 'P':
									++player2_score;
									break;
								case 'R':
									++player1_score;
									break;
							}
							first_throw = 0;
						}

						break;
					default:
						/*eat up whitespace*/
						if(first_throw != 0) {
							fprintf(stderr, "input read failed, need player moves as consecutive characters i.e SP\n");
							exit(-1);
						}
				}
			}
			/*check if we read the last chunk accounting for null character*/
			if(bytes_read < BUFF_SIZE -1) {
				break;
			}
			/*if the last character in the buffer is a newline, we quit as well*/
			if(input[bytes_read-1] == '\n') {
				break;
			}
		}
		if(player2_score > player1_score) {
			result[ii] = 2;
		} else {
			if(player1_score == 0) {
				fprintf(stderr, "Error, scores should no be 0 0\n");
				exit(-1);
			}
			result[ii] = 1;
		}
	}
	for(ii = 0; ii < N; ++ii) {
		printf("%d ",result[ii]);
	}
	printf("\n");
	free(buff);
	free(result);
	return 0;
}
