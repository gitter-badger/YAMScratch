#ifndef Parser_Bowling_
#define Parser_Bowling_

#include <string>
#include <fstream>

#include "bowling_game.h"
#include "bowling_player.h"


class ParserBowling
{
public:
	ParserBowling(){};
	~ParserBowling(){};

	//UTILITY
	int readFile(std::string input_filename, BowlingGame *game);

private:
	std::ifstream in_str_; //input file stream

};

#endif