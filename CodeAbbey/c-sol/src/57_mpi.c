#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <mpi.h>

/*
** should run with N processes, as it follows the master slave model, 
** with N-1 slaves performing work chunks in a queue
*/
#define MASTER 0
/*the size of a buffer that read before sending it to child process*/
#define CHUNKSIZE 40
#define GRACEFUL_TERMINATE (MPI_ERR_LASTCODE + 10) 
#define PROGRAM_ERROR (MPI_ERR_LASTCODE + 11)


int main(int argc, char *argv[])
{
	int nproc;
	int rank;
	int rc;
	int error_code;
	int error_str_len;
	MPI_Init(&argc, &argv);
	MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
	rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	/*TODO: handle the errors in a better way*/
	if(rc != MPI_SUCCESS) {
		char error_string[MPI_MAX_ERROR_STRING];
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
		/*master thread is in charge of reading the input file and then sending
		* chunks to the slaves to compute*/
		unsigned long N;
		errno = 0;
		rc = scanf(" %lu ", &N);
		if(rc != 1) {
			perror("failed to read input");
			exit(-1);
		}
		/*allocate an input buffer and output buffer*/
		double* input, * result;
		input = (double*)malloc(N * sizeof(double));
		if(input == NULL) {
			perror("failed to allocate input buffer");
			exit(-1);
		}
		/*the first and last values are unchanged, so we must account for this
		* child routines*/
		result = (double*)malloc(N * sizeof(double));
		if(result == NULL) {
			perror("failed to allocate result buffer");
			exit(-1);
		}
		unsigned ii, jj, kk;
		unsigned n_chunks, remain;
		n_chunks = N / CHUNKSIZE;
		remain = N % CHUNKSIZE;
		for(ii = 0; ii < n_chunks; ++ii) {
			scanf("");
			/*
			* sent buffers overlap by two entries because we are averaging
			* the closest three together, where the earlier chunk fills in
			* buff_size-2 and later chunk fills in buff_size-1
			*/
		}
		for(ii = 0; ii < remain; ++ii) {
			for(jj = 0; jj < CHUNKSIZE; ++jj) {
				rc = scanf(" %lf ", input+((ii * CHUNKSIZE) + jj));
				if(rc != 1) {
					fprintf(stderr, "Input read failed on line %u\n", (ii*CHUNKSIZE + jj+2) );
					/*alert all proceses in wait loop*/
					error_code = PROGRAM_ERROR;
					MPI_Bcast(&error_code, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
					MPI_Finalize();
					exit(-1);
				}
			}
		}
		/*send a sucess message to worker processes so they can start work*/
		rc = GRACEFUL_TERMINATE;
		MPI_Bcast(&rc, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

		MPI_Finalize();
	} else {
		/*these are all worker processes*/
		for(;;) {
			/*first check and see if there were any errors
			* this also handles shutdown*/
			MPI_Bcast(&rc, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
			if(rc != MPI_SUCCESS) {
				if(rc == GRACEFUL_TERMINATE) {
					/*stderr is unbiffered on unix*/
					fprintf(stderr, "Exiting process %d gracefully\n", rank);
				}
				MPI_Finalize();
				exit(rc);
			}
		}

		/*we should only reach here in exceptional conditions*/
		MPI_Finalize();
		return -2;
	}
	return 0;
}
