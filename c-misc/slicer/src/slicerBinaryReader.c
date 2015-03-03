#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <endian.h>
/*gives bzero*/
#include <strings.h>

#include "slicerBinaryReader.h"
#include "slicerConstants.h"

int bat(){return 6;}

int parseBinary(char* fn) {
	/*allocate a buffer to read the file
	this buffer must be a multiple of the face length structure so
	we can trivially alias*/
	const size_t buffer_length = (size_t)(BYTES_PER_FACET * BUFFER_MULTIPLE);
	uint8_t* buffer = (uint8_t*)malloc(buffer_length);
	FILE *file_ptr;
	file_ptr = fopen(fn, "rb");
	if(file_ptr == NULL) {
		fprintf(stderr, "File failed to open\n" );
		free(buffer);
	}
	int rc = fread(buffer, 1, HEADER_LENGTH, file_ptr);
	if (rc != HEADER_LENGTH)
	{
		fprintf(stderr, "STL header not found\n");
		free(buffer);
		exit(1);
	} else {
		/*turn the buffer into a string by null terminating*/
		for(int i = 0; i < HEADER_LENGTH; ++i) {
			if(buffer[i] == 32) {
				buffer[i] = '_';
			}
		}
		buffer[HEADER_LENGTH] = 0;
		printf("Header\n%s\n", buffer);
	}

	/*now get the number of triangles*/
	bzero(buffer, HEADER_LENGTH);
	rc = fread(buffer, 1, 4, file_ptr);
	if(rc != 4) {
		fprintf(stderr, "Read failed\n");
		free(buffer);
		exit(1);
	} else {
		uint32_t* four_byte;
		four_byte = (uint32_t*) buffer;
		uint32_t triangle_num = le32toh((*four_byte));
		printf("Number of triangles is: %d\n",triangle_num );
		bzero(buffer, 4);
	}
	/*Now start reading the rest of the stl into the buffer*/
	rc = fread(buffer, 1, buffer_length, file_ptr);
	/*printf("bytes read: %d\n", rc );*/
	while(rc >= buffer_length) {
		
		if(1) {
			printf("Debug\n");
		}
		bzero(buffer, buffer_length);
		rc = fread(buffer, 1, buffer_length, file_ptr);
	}

	free(buffer);
	return 0;
}
