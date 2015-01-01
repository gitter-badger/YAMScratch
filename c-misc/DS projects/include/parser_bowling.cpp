#include <string>
#include <fstream>
#include <iostream>

#include "parser_bowling.h"
#include "bowling_frame.h"
#include "bowling_game.h"

#ifndef NUM_FRAMES_PER_GAME
#define NUM_FRAMES_PER_GAME 10
#endif

#ifndef NUM_THROWS_PER_FRAME
#define NUM_THROWS_PER_FRAME 2
#endif



int ParserBowling::readFile(std::string input_filename, BowlingGame* game)
{
	in_str_.open(input_filename.c_str(), std::ifstream::in);
	if(in_str_.is_open())
	{
		std::string first_name;
		std::string last_name;
		//get the first and last name
		in_str_ >> first_name >> last_name;
		int throws_remaining = (NUM_FRAMES_PER_GAME-1) * NUM_THROWS_PER_FRAME; //do the first 9 frames

		
		bool second_throw = false;

		int pins_hit;
		while(throws_remaining)
		{
			//assume that the input is well formatted
			//key assumotion is there are only pins_hit >= 0
			throws_remaining--;
		}
		//do the last frame


	}
	else
	{
		std::cerr << "Failed to open for reading file: " << input_filename
					<< std::endl;
	}
	return 0;
}


