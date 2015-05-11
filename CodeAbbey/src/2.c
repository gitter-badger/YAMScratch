#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/

#define BUFFER_SIZE 3
#define BUFFER_END BUFFER_SIZE-1

int main(int argc, char* argv[]) {
	int nread;
	char* buffer;
	buffer = (char*)malloc(BUFFER_SIZE);
	fprintf(stdout, "N: ");
	fflush(stdout); 
	nread = read(STDIN_FILENO, buffer, BUFFER_END);
	buffer[BUFFER_END] = '\0'; /*forcibly null terminate*/
	printf("Input: %s ffoo\n", buffer);
	fflush(stdout);
	printf("bytes: %d\n", nread);
	fflush(stdout);

	/*force a read*/
	do {
		nread = read(STDIN_FILENO, buffer, BUFFER_END);
		buffer[BUFFER_END] = '\0'; /*forcibly null terminate*/
		printf("Foo %s\n", buffer);
		fflush(stdout);
		printf("bar %d\n", nread);
		fflush(stdout);
	} while(nread);

	return 0;

}