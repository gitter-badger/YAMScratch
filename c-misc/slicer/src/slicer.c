#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define HEADER_LENGTH 80
#define BUFFER_MULTIPLE 10

struct Face {
	/*solidworks may not fill in normal information */
	uint32_t n[3];  	/*Normal vector*/
	uint32_t v1[3]; 	/*vertex 1 */
	uint32_t v2[3]; 	/*vertex 2*/
	uint32_t v3[3];		/*vertex 3*/
	uint16_t ac;		/*Attribute counter*/ 
};

int main(int argc, char** argv) {
	/*allocate a buffer to read the file
	this buffer must be a multiple of the face length structure so
	we can trivially alias*/
	if(argc == 2) {
		size_t buffer_length = sizeof(struct Face)* BUFFER_MULTIPLE;
		uint8_t* buffer = (uint8_t*)malloc(buffer_length);
		printf("%s\n", argv[1]);
	}

	return 0;
}
