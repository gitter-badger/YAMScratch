#ifndef Parser_Bowling_
#define Parser_Bowling_

#include <string>

#include "bowling_game.h"
#include "bowling_player.h"


class ParserBowling
{
public:
	ParserBowling(const std::string filename): filename_(filename){};
	~ParserBowling();

	void readFile(); //
	void readFile(BowlingGame *game);


private:
	std::string filename_;
	std::istream in_str_; //input file stream
	BowlingGame game_; // the game

};

#endif