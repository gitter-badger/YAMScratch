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
		unsigned ii, jj;
		errno = 0;
		rc = scanf(" %lu ", &N);
		if(rc != 1) {
			perror("failed to read input");
			exit(-1);
		}

		Vector_t(char)* next, * prev, * swap_tmp;
		next = newVector(char);
		prev = newVector(char);
		/*the base case of the L system*/
		vector_push_back(char, next, 'A');
		/*add a null terminator so we can do a hacky thing and 
		* treat the underlying representation as a string and print
		* directly
		*/
		vector_push_back(char,next, '\0');
		fprintf(stdout, "#0 %s\n",next->items);
		/*generate the program recursively by switching back and forth*/
		for(ii = 1; ii < 10; ++ii) {
			/*swap the pointers for old and new*/
			swap_tmp = next;
			next = prev;
			prev = swap_tmp;
			/*clear room in next*/
			vector_clear(char, next);
			unsigned num_items;
			num_items = prev->elms;
			/*create the next portion of the L-system*/
			for(jj = 0; jj < num_items; ++jj) {
				switch(prev->items[jj]) {
					case 'A':
						/*all 'A's become 'AB's*/
						vector_push_back(char, next, 'A');
						vector_push_back(char, next, 'B');
						break;
					case 'B':
						/*replace all 'A's with 'B's*/
						vector_push_back(char, next, 'A');
						break;
					default:
						/*just copy any other character over, this will move the null as well*/
						vector_push_back(char, next, prev->items[jj]);
						break;
				} 
			}
			printf("#%u %s\n",ii, next->items);
		}
		/*send a sucess message to worker processes so they can start work*/
		rc = MPI_SUCCESS;
		MPI_Bcast(&rc, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

		vector_destroy(char, next);
		vector_destroy(char, prev);
		MPI_Finalize();
	} else {
		/*these are all worker processes*/
		/*first check and see if there were any errors*/
		MPI_Bcast(&rc, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
		if(rc != MPI_SUCCESS) {
			MPI_Finalize();
			exit(rc);
		}
		fprintf(stdout,"Hello from processor %d\n", rank);
		fflush(stdout);

		MPI_Finalize();
	}
	return 0;
}
