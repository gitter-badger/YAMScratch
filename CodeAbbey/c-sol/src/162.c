#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define NULL_CHECK(ptr, msg) 	\
	if(ptr == NULL) {			\
		perror(msg);			\
		exit(-1);				\
	}

enum FACES { LEFT=0, FRONT=1, RIGHT=2, BACK=3, UP=4, DOWN=5 };

/*numer of cells per face*/
#define CUBE_ORDER 9

struct ThreeCube {
	unsigned order;
	unsigned num_tracked;
	unsigned* tracked;
	unsigned grid[CUBE_ORDER*6];
};
/*
Grid is flat array with grid index =
	FACE * 9 + cell_index

      +---+
     / U /
+---+---+---+---+
| L | F | R | B |
+---+---+---+---+
     \ D \
      +---+

          +-----+
         /0 1 2/
        /7 8 3/
       /6 5 4/
+-----+-----+-----+-----+
|2 3 4|2 3 4|2 3 4|2 3 4|
|1 8 5|1 8 5|1 8 5|1 8 5|
|0 7 6|0 7 6|0 7 6|0 7 6|
+-----+-----+-----+-----+
	   \4 5 6\
	    \3 8 7\
	     \2 1 0\
	      +-----+
*/
int main(int argc, char const *argv[])
{
	/*simulate a rubiks cube*/
	unsigned long N;
	int rc;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	/*initialize the cube*/
	struct ThreeCube* cube;
	cube = (struct ThreeCube*)calloc(1, sizeof(struct ThreeCube));
	NULL_CHECK(cube, "failed to allocate cube");
	/*hold all writes until end*/
	unsigned* grid_buffer;

	grid_buffer = (unsigned*)malloc(CUBE_ORDER*6 *sizeof(unsigned));
	NULL_CHECK(grid_buffer, "failed to allocate grid_buffer");
	
	cube->num_tracked = CUBE_ORDER;
	//cube->num_tracked = CUBE_ORDER*6;

	cube->tracked = (unsigned*)malloc(cube->num_tracked * sizeof(unsigned));
	

	unsigned ii, jj;
	for(ii = 0; ii < cube->num_tracked; ++ii) {
		cube->tracked[ii] = FRONT* CUBE_ORDER + ii;
		/*initialize it to a value that is reconizable*/
		cube->grid[FRONT* CUBE_ORDER +ii] = 100 + ii;

		//cube->tracked[ii] = ii;
		//cube->grid[ii] = ii*2;
	}
	/*initialize the write buffer*/
	errno = 0;
	memcpy(grid_buffer, cube->grid, CUBE_ORDER*6*sizeof(unsigned));
	if(errno != 0) {
		perror("failed to copy grid buffer");
		exit(-1);
	}

	char C;
	unsigned face, cell, new_face, new_cell, rotation;
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf(" %c ", &C);
		if(rc != 1) {
			perror("failed to read input");
			exit(-1);
		}
		switch(C) {
			case 'U':
				rotation = UP;
				break;
			case 'D':
				rotation = DOWN;
				break;
			case 'L':
				rotation = LEFT;
				break;
			case 'R':
				rotation = RIGHT;
				break;
			case 'F':
				rotation = FRONT;
				break;
			case 'B':
				rotation = BACK;
				break;
			default:
				fprintf(stderr, "recieved %c expected [U/D/L/R/F/B]\n", C);
				exit(-1);
		}
		switch(rotation) {
			case UP:
				for(jj = 0; jj < cube->num_tracked; ++jj) {
					face = cube->tracked[jj];
					cell = face % CUBE_ORDER;
					face /= CUBE_ORDER;
					/*set the default*/
					new_face = face;
					new_cell = cell;
					switch(face) {
						case UP:
							if(cell != 8) {
								new_cell = (cell+2) % 8;
							}
							break;
						case DOWN:
							/*not affected*/
							break;
						default: /*this will catch LEFT, RIGHT, FRONT, BACK*/
							if(cell > 1 && cell < 5) {
								/*rotate faces of cells 2 3 4*/
								new_face = (face + 3)%4;
							}
							break;
					}
					grid_buffer[new_face*CUBE_ORDER + new_cell] = cube->grid[face*CUBE_ORDER + cell];
					cube->tracked[jj] = new_face*CUBE_ORDER+new_cell;
				}
				break;
			case DOWN:
				for(jj = 0; jj < cube->num_tracked; ++jj) {
					face = cube->tracked[jj];
					cell = face % CUBE_ORDER;
					face /= CUBE_ORDER;
					/*set the default*/
					new_face = face;
					new_cell = cell;
					switch(face) {
						case DOWN:
							if(cell != 8) {
								new_cell = (cell+2) % 8;
							}
							break;
						case UP:
							/*not affected*/
							break;
						default: /*this will catch LEFT, RIGHT, FRONT, BACK*/
							if(cell == 0 || cell == 7 || cell == 6) {
								/*rotate faces of cells 0 7 6*/
								new_face = (face+1) % 4;
							}
							break;
					}
					grid_buffer[new_face*CUBE_ORDER + new_cell] = cube->grid[face*CUBE_ORDER + cell];
					cube->tracked[jj] = new_face*CUBE_ORDER+new_cell;
				}
				break;
			case LEFT:
				for(jj = 0; jj < cube->num_tracked; ++jj) {
					face = cube->tracked[jj];
					cell = face % CUBE_ORDER;
					face /= CUBE_ORDER;
					/*set the default*/
					new_face = face;
					new_cell = cell;
								switch(face) {
						case LEFT:
							if(cell != 8) {
								new_cell = (cell+2) % 8;
							}
							break;
						case RIGHT:
							/*not affected*/
							break;
						case FRONT:
							if(cell < 3) {
								new_face = DOWN;
								new_cell = (cell+2) % 8;
							}
							break;
						case DOWN:
							if(cell > 1 && cell < 5) {
								new_face = BACK;
								new_cell = (cell+2) % 8;
							}
							break;
						case BACK:
							if(cell > 3 && cell < 7) {
								new_face = UP;
								new_cell = (cell+2) % 8;
							}
							break;
						case UP:
							if(cell == 0 || cell == 6 || cell == 7) {
								new_face = FRONT;
								new_cell = (cell+2) % 8;
							}
							break;
					}
					grid_buffer[new_face*CUBE_ORDER + new_cell] = cube->grid[face*CUBE_ORDER + cell];
					cube->tracked[jj] = new_face*CUBE_ORDER+new_cell;
				}
				break;
			case RIGHT:
				for(jj = 0; jj < cube->num_tracked; ++jj) {
					face = cube->tracked[jj];
					cell = face % CUBE_ORDER;
					face /= CUBE_ORDER;
					/*set the default*/
					new_face = face;
					new_cell = cell;
					switch(face) {
						case RIGHT:
							if(cell != 8) {
								new_cell = (cell+2) % 8;
							}
							break;
						case LEFT:
							/*not affected*/
							break;
						case BACK:
							if(cell < 3) {
								new_face = DOWN;
								new_cell = (cell+6) % 8;
							}
							break;
						case DOWN:
							if(cell == 0 || cell == 7 || cell == 6) {
								new_face = FRONT;
								new_cell = (cell+6) % 8;
							}
							break;
						case FRONT:
							if(cell > 3 && cell < 7) {
								new_face = UP;
								new_cell = (cell+6) % 8;
							}
							break;
						case UP:
							if(cell > 1 && cell < 5) {
								new_face = BACK;
								new_cell = (cell+6) % 8;
							}
							break;
					}
					grid_buffer[(new_face*CUBE_ORDER) + new_cell] = cube->grid[(face*CUBE_ORDER) + cell];
					cube->tracked[jj] = (new_face*CUBE_ORDER)+new_cell;
				}
				break;
				break;
			case FRONT:
				for(jj = 0; jj < cube->num_tracked; ++jj) {
					face = cube->tracked[jj];
					cell = face % CUBE_ORDER;
					face /= CUBE_ORDER;
					/*set the default*/
					new_face = face;
					new_cell = cell;
					switch(face) {
						case FRONT:
							if(cell != 8) {
								new_cell = (cell+2) % 8;
							}
							break;
						case BACK:
							/*not affected*/
							break;
						case RIGHT:
							if(cell < 3) {
								new_face = DOWN;
								new_cell = (cell+4) % 8;
							}
							break;
						case DOWN:
							if(cell > 3 && cell < 7) {
								new_face = LEFT;
								/*add zero to cell*/
							}
							break;
						case LEFT:
							if(cell > 3 && cell < 7) {
								new_face = UP;
								/*add zero to cell*/
							}
							break;
						case UP:
							if(cell > 3  && cell < 7) {
								new_face = RIGHT;
								new_cell = (cell+4) % 8;
							}
							break;
					}
					grid_buffer[(new_face*CUBE_ORDER) + new_cell] = cube->grid[(face*CUBE_ORDER) + cell];
					cube->tracked[jj] = (new_face*CUBE_ORDER)+new_cell;
				}
				break;
			case BACK:
				for(jj = 0; jj < cube->num_tracked; ++jj) {
					face = cube->tracked[jj];
					cell = face % CUBE_ORDER;
					face /= CUBE_ORDER;
					/*set the default*/
					new_face = face;
					new_cell = cell;
					switch(face) {
						case BACK:
							if(cell != 8) {
								new_cell = (cell+2) % 8;
							}
							break;
						case FRONT:
							/*not affected*/
							break;
						case LEFT:
							if(cell < 3) {
								new_face = DOWN;
								/*add zero to cell*/
							}
							break;
						case DOWN:
							if(cell < 3) {
								new_face = RIGHT;
								new_cell = (cell+4) % 8;
							}
							break;
						case RIGHT:
							if(cell > 3 && cell < 7) {
								new_face = UP;
								new_cell = (cell+4) % 8;
							}
							break;
						case UP:
							if(cell < 3) {
								new_face = LEFT;
								/*add zero to cell*/
							}
							break;
					}
					grid_buffer[(new_face*CUBE_ORDER) + new_cell] = cube->grid[(face*CUBE_ORDER) + cell];
					cube->tracked[jj] = (new_face*CUBE_ORDER)+new_cell;
				}
				break;
		}
		/*update the cube according to the write buffer*/
		memcpy(cube->grid, grid_buffer, CUBE_ORDER*6*sizeof(unsigned));
	}
	/*convert the numbering back from inner representation to given repr
		+-----+    +-----+
		|2 3 4|    |7 8 9|
		|1 8 5| => |4 5 6|
		|0 7 6|    |1 2 3|
		+-----+    +-----+
	*/
	unsigned mapping[9] = {0 ,7 ,6, 1, 8, 5, 2 ,3, 4};
	for(ii = 0; ii < cube->num_tracked; ++ii) {
		face = cube->tracked[mapping[ii]];
		face /= CUBE_ORDER;
		switch(face) {
			case 0:
				C = 'L';
				break;
			case 1:
				C = 'F';
				break;
			case 2:
				C = 'R';
				break;
			case 3:
				C = 'B';
				break;
			case 4:
				C = 'U';
				break;
			case 5:
				C = 'D';
				break;
		}
		printf("%c ", C);
	}
	printf("\n");
	return 0;
}
