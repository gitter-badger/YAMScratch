#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
/*convert the endianess of stl to host*/
#include <endian.h>
/*test file permissions*/
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define HEADER_LENGTH 80
#define BUFFER_MULTIPLE 10
#define CHAR_PER_EXTENSION 4
#define MAX_FILE_SIZE_KILOBYTES 1024

struct IntFace {
	/*solidworks may not fill in normal information */
	uint32_t n[3];  	/*Normal vector*/
	uint32_t v1[3]; 	/*vertex 1 */
	uint32_t v2[3]; 	/*vertex 2*/
	uint32_t v3[3];		/*vertex 3*/
	uint16_t ac;		/*Attribute counter*/ 
};

/*Alias type to interpret stl format*/
struct Face {
	float n[3];
	float v1[3];
	float v2[3];
	float v3[3];
	uint16_t ac;
};


int main(int argc, char** argv) {
	/*allocate a buffer to read the file
	this buffer must be a multiple of the face length structure so
	we can trivially alias*/
	if(argc != 2) {
		fprintf(stderr, "Improper number of arguments\n\n" );
		fprintf(stderr, "Usage: \n\tslicer <filename.stl>\n");
		exit(1);
	}
	size_t buffer_length = sizeof(struct IntFace)* BUFFER_MULTIPLE;
	uint8_t* buffer = (uint8_t*)malloc(buffer_length);
	float i = 0;
	printf("%s %d\n", argv[1], sizeof(i));
	int fn_len = strlen(argv[1]);
	/*Stat the target file to test permissions are valid before 
	attempting to open*/

	struct stat* stat_info = (struct stat*)malloc(sizeof(struct stat));
	int n;
	n = stat(argv[1], stat_info);
	/*test that file size is not too big*/
	if((stat_info->st_size / 1024) > MAX_FILE_SIZE_KILOBYTES) {
		fprintf(stderr, "The file is too large: %dkb out of %dkb\n",
			(stat_info->st_size/ 1024)+1 , MAX_FILE_SIZE_KILOBYTES );

		free(buffer);
		exit(1);
	}
	printf("filesize: %d\n", stat_info->st_size);

	char extension[4];
	for(int i = 0; i < CHAR_PER_EXTENSION; ++i) {
		extension[i] = argv[1][fn_len-(CHAR_PER_EXTENSION - i)];
	}
	printf("%s\n",extension );
	/*clean up extension one we are done with it*/

	free(buffer);
	return 0;
}
