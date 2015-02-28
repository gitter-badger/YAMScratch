#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/*gives strlen*/
#include <string.h>
/*convert the endianess of stl to host*/
#include <endian.h>
/*testing file permissions*/
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/*command line argument parsing library*/
#include <argp.h>
/*lets us accumulate arguments in struct arguments*/
#include <argz.h>

#define HEADER_LENGTH 80
#define BUFFER_MULTIPLE 10
#define CHAR_PER_EXTENSION 4
#define MAX_ARGUMENTS 2
#define MIN_ARGUMENTS 1

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

/*Hold a variable length of arguments passed to the program
works kind of like std::vector*/
struct Arguments {
	char *argz;
	size_t argz_len;
};

struct Parameters {
	struct Arguments args;
	float h;
	uint8_t debug;
};

/*Forward Function Declarations*/
static int parse_opt(int key, char *arg, struct argp_state *state);

int main(int argc, char** argv) {
	/*This struct is where we define every parameter to the function*/
	struct argp_option options[] = 
	{
		{"height", 'h',"NUM", 0, "Set the layer height of each slice"},
		{"debug",'d',0 , OPTION_ARG_OPTIONAL, "Display debug output"},
		{"easter", 0, 0, OPTION_HIDDEN, "Easter egg"},
		{0}
	};
	/*This is where we add the arguments*/
	struct argp argp = {options, parse_opt,"FILENAME [OUTPUT FILENAME]"};
	/*Initialize and fill in our callback struct to use instead of globals*/
	struct Parameters params;
	params.h = 0;
	params.debug = 0;
	/*Check that all of the arguments parsed correctly*/
	error_t rc;
	rc = argp_parse(&argp, argc, argv, 0,0, &params);
	if(rc != 0) {
		fprintf(stderr, "Argp returned error code: %d\n",rc );
		exit(1);
	} else {
		const char *prev = NULL;
		char *word;
		while((word = argz_next(params.args.argz, params.args.argz_len, prev))) {
			printf("%s\n",word );
			prev = word;
		}	
	}


	/*allocate a buffer to read the file
	this buffer must be a multiple of the face length structure so
	we can trivially alias*/
	size_t buffer_length = sizeof(struct IntFace)* BUFFER_MULTIPLE;
	uint8_t* buffer = (uint8_t*)malloc(buffer_length);
	/*Stat the target file to test permissions are valid before 
	attempting to open*/

	struct stat* stat_info = (struct stat*)malloc(sizeof(struct stat));
	int n;
	n = stat(argv[1], stat_info);
	if(n) {
		fprintf(stderr, "Problems\n" );
	}

	free(params.args.argz);
	free(buffer);
	return 0;
}



const char *argp_program_bug_address = "yetanotherminion@gmail.com";
const char *argp_program_version = 
"YAM Slicer, version 0.1 -devel (x86_64-redhat-linux-gnu)\n\
Copyright (C) 2015 Isaiah Bell\n\
License MIT\n\
\n\
This is free software; you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.";

static int
parse_opt(int key, char *arg, struct argp_state *state) {
	struct Parameters *foo = state->input;
	struct Arguments *a = &(foo->args);
	float *layer_height = &(foo->h);
	switch(key) {
		case 'h': {
			*layer_height = atof(arg);
			break;
		} case 'd': {
			
			break;
		} case ARGP_KEY_INIT: {
			a->argz = 0;
			a->argz_len = 0;
			break;
		} case ARGP_KEY_ARG: {
			/*add to the struct arguments we made earlier*/
			argz_add(&a->argz, &a->argz_len, arg);
			break;
		} case ARGP_KEY_END: {
			size_t count = argz_count(a->argz,a->argz_len);
			if(count > MAX_ARGUMENTS) {
				argp_failure(state,1, 0, "Too many arguments");
			} else if(count < MIN_ARGUMENTS) {
				argp_failure(state, 1, 0, "Not enough arguments");
			}
			break;

		}
	}
	return 0;
}
