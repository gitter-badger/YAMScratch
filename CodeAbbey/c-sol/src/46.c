#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/

#define MOVE_X(x_var, cell) \
	(x_var) |= 1U<<(cell)
#define MOVE_Y(y_var, cell) \
	(y_var) |= 1U<<(cell)

#define MASK_DIAG_2 0x111
#define MASK_DIAG_1 0x54
#define MASK_ROW 0x7
#define MASK_COL 0x49

int move_wins(unsigned move_var) {
	/*check rows first*/
	unsigned ii;
	for(ii = 0; ii < 3; ++ii){
		if((move_var & MASK_ROW<<(ii*3)) == (unsigned)(MASK_ROW<<(ii*3))) return 1;
	}
	/*check each column*/
	for(ii = 0; ii < 3; ++ii) {
		if((move_var & MASK_COL<<ii) == (unsigned)(MASK_COL<<ii)) return 1;
	}
	if((move_var & MASK_DIAG_1) == MASK_DIAG_1) return 1;
	if((move_var & MASK_DIAG_2) == MASK_DIAG_2) return 1;
	return 0;
}

int main(int argc, char* argv[]) {
	/*compute who won at tic tac toe given sequence of 
	* moves starting with X*/
	int rc;
	unsigned N;
	errno = 0;
	rc = scanf(" %u ", &N);
	if(rc != 1 || errno != 0) {
		perror("input read failed on first line");
		exit(-1);
	}
	unsigned ii, jj, M, O_moves, X_moves, draw_flag;

	for(jj = 0; jj < N; ++jj) {
		O_moves= 0;
		X_moves = 0;
		draw_flag = 1;
		for(ii = 0; ii < 9; ++ii) {
			errno = 0;
			rc = scanf(" %u ", &M);
			/*account for zero indexing*/
			--M;
			if(rc != 1 || errno != 0) {
				perror("input read failed");
				exit(-1);
			}
			if((ii % 2)) {
				MOVE_X(O_moves, M);
				if(move_wins(O_moves)) {
					++ii;
					printf("%d ", ii);
					draw_flag = 0;
					break;
				}
			} else {
				MOVE_Y(X_moves, M);
				if(move_wins(X_moves)) {
					++ii;
					printf("%d ", ii);
					draw_flag = 0;
					break;
				}
			}
		}
		/*consume the remaining values*/
		for(; ii < 9; ++ii) {
			rc = scanf(" %*u ");
			if(rc != 0 || errno != 0) {
				perror("input read failure after game determined");
				exit(-1);
			}
		}
		if((draw_flag)){
			printf("0 ");
		}
	}
	printf("\n");
	return 0;
}
