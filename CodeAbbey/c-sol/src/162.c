#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

enum FACES { LEFT=0, FRONT=1, RIGHT=2, BACK=3, UP=4, DOWN=5 };

struct ThreeCube {
	unsigned order;
	unsigned num_tracked;
	unsigned* tracked;
	unsigned grid[54];
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
         /7 8 9/
        /4 5 6/
       /1 2 3/
+-----+-----+-----+-----+
|7 8 9|7 8 9|7 8 9|7 8 9|
|4 5 6|4 5 6|4 5 6|4 5 6|
|1 2 3|1 2 3|1 2 3|1 2 3|
+-----+-----+-----+-----+
	   \7 8 9\
	    \4 5 6\
	     \1 2 3\
	      +-----+
*/

/*numer of cells per face*/
#define CUBE_ORDER 9

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
	cube->num_tracked = CUBE_ORDER;
	cube->tracked = (unsigned*)malloc(cube->num_tracked * sizeof(unsigned));
	
	unsigned ii, jj;
	for(ii = 0; ii < cube->num_tracked; ++ii) {
		cube->tracked[ii] = FRONT* CUBE_ORDER + ii;
		/*initialize it to a value that is reconizable*/
		cube->grid[FRONT* CUBE_ORDER +ii] = 100 + ii;
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
		printf("rotation = %c %u\n", C, rotation);

		switch(rotation) {
			case UP:
				for(jj = 0; jj < cube->num_tracked; ++jj) {
					face = cube->tracked[jj];
					cell = face % CUBE_ORDER;
					face /= CUBE_ORDER;
					switch(face) {

						case UP:
							new_face = face;
							if(cell%2 == 0) {
								new_cell = (cell%8) +2;
							}

						case DOWN:
							break;
						default: /*this will catch LEFT, RIGHT, FRONT, BACK*/
							new_face = (face + 3)%4;
							break;
					}
				}
				break;
			case DOWN:
				break;
			case LEFT:
				break;
			case RIGHT:
				break;
			case FRONT:
				break;
			case BACK:
				break;
		}
	}
	printf("\n");
	return 0;
}



