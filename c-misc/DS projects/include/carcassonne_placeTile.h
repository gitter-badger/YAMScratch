#ifndef _PLACE_TILE_
#define _PLACE_TILE_

#include <vector>

#include "carcassonne_tile.h"
#include "carcassonne_board.h"

void 
f_placeTile(std::vector<struct Board> & solutions, 
			std::vector<struct Tile> candidates,
			const std::vector<struct Tile> & source,
			const int source_index,
			const int insertion_x,
			const int insertion_y,
			const enum Rotation angle);

#endif
