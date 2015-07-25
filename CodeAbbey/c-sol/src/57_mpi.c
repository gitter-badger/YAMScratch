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
#define ERROR_TAG 37

void master_send_failure(MPI_Comm communicator, int master_proc, int nproc, int error_code) {
	unsigned ii;
	for(ii = 0; ii < nproc; ++ii) {
		if(ii != master_proc) {
			/*blocking send*/
			rc = MPI_Send(&error_code, 1, MPI_INT, ii, ERROR_TAG, communicator);
			if(rc != MPI_SUCCESS) {
				/*uh-oh, an error in the error handling code*/
				MPI_Abort(communicator, error_code);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int nproc;
	int rank;
	int rc;
	int error_code;
	int error_str_len;
	MPI_Init(&argc, &argv);
	MPI_Comm MPI_COMM_WORLD;
	MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
	rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	/*make error string available to all processes*/
	char error_string[MPI_MAX_ERROR_STRING];
	/*TODO: handle the errors in a better way*/
	if(rc != MPI_SUCCESS) {

		MPI_Error_string(rc, error_string, &error_str_len);
		fprintf(stderr, "Rank unknown: %s\n", error_string);
		/*errors this early we probably can do nothing about*/
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	rc = MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	if(rc != MPI_SUCCESS) {
		int error_str_len;
		MPI_Error_string(rc, error_string, &error_str_len);
		fprintf(stderr, "MPI error in rank %d: %s\n", rank, error_string);
		/*errors this early we probably can do nothing about*/
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	if(rank == MASTER) {
		unsigned ii, jj, kk;
		/*master thread is in charge of reading the input file and then sending
		* chunks to the slaves to compute*/
		unsigned long N;
		errno = 0;
		rc = scanf(" %lu ", &N);
		if(rc != 1) {
			master_send_failure(MPI_COMM_WORLD, MASTER, nproc, PROGRAM_ERROR);
			perror("failed to read input");
			exit(-1);
		}
		/*allocate an input buffer and output buffer*/
		double* input, * result;
		input = (double*)malloc(N * sizeof(double));
		if(input == NULL) {
			master_send_failure(MPI_COMM_WORLD, MASTER, nproc, PROGRAM_ERROR);
			perror("failed to allocate input buffer");
			exit(-1);
		}
		/*the first and last values are unchanged, so we must account for this
		* child routines*/
		result = (double*)malloc(N * sizeof(double));
		if(result == NULL) {
			free(input);
			/*send failure message to each worker process*/
			master_send_failure(MPI_COMM_WORLD, MASTER, nproc, PROGRAM_ERROR);
			perror("failed to allocate result buffer");
			exit(-1);
		}

		unsigned n_chunks, remain;
		n_chunks = N / CHUNKSIZE;
		remain = N % CHUNKSIZE;
		for(ii = 0; ii < n_chunks; ++ii) {
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
					master_send_failure(MPI_COMM_WORLD)
					/*loop through and tell all processes to terminate*/
					MPI_Finalize();
					exit(-1);
				}
			}
		}
		/*send a sucess message to worker processes so they can start work*/
		rc = GRACEFUL_TERMINATE;
		MPI_Bcast(&rc, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

		MPI_Finalize();
		free(result);
	} else {
		/*perform the computation of average neighboring points in a new buffer
		* to reduce complexity of managing a 3 element circular buffer, too much
		* overkill for this application*/
		double* smoothed;
		/*these are all worker processes*/
		for(;;) {
			/*first check and see if there were any errors
			* this also handles shutdown*/
			
			if(rc != MPI_SUCCESS) {
				if(rc == GRACEFUL_TERMINATE) {
					/*stderr is unbuffered on unix*/
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
