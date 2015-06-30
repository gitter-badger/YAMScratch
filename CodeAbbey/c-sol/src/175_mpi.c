#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <mpi.h>

#include "parseCodeAbbeyInput.h"

VECTOR_INIT(char)
/*
** should run with N processes, as it follows the master slave model, 
** with N-1 slaves performing work chunks in a queue
*/
#define MASTER 0

int main(int argc, char *argv[])
{
	int nproc;
	int rank;
	int rc;
	MPI_Init(&argc, &argv);
	MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
	rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	/*TODO: handle the errors in a better way*/
	if(rc != MPI_SUCCESS) {
		char error_string[MPI_MAX_ERROR_STRING];
		int error_str_len;
		MPI_Error_string(rc, error_string, &error_str_len);
		fprintf(stderr, "Rank unknown: %s\n", error_string);
		/*errors this early we probably can do nothing about*/
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	rc = MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	if(rc != MPI_SUCCESS) {
		char error_string[MPI_MAX_ERROR_STRING];
		int error_str_len;
		MPI_Error_string(rc, error_string, &error_str_len);
		fprintf(stderr, "MPI error in rank %d: %s\n", rank, error_string);
		/*errors this early we probably can do nothing about*/
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	if(rank == MASTER) {
		/*master thread is in charge of reading the input file and then sending*/
		unsigned long N;

		unsigned ii, jj, kk;
		errno = 0;
		rc = scanf(" %lu ", &N);
		if(rc != 1) {
			perror("failed to read input");
			exit(-1);
		}

		Vector_t(char)* next, * prev;
		/*the base case of the L system*/
		
		/*generate the program recursively by switching back and forth*/
		MPI_Finalize();
	} else {
		/*these are all worker processes*/
		/*first check and see if there were any errors*/

		MPI_Finalize();
	}
	return 0;
}
