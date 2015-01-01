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

#ifndef PINS_PER_FRAME
#define PINS_PER_FRAME 10
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
		int frame_count = (NUM_FRAMES_PER_GAME-1); //do the first 9 frames

		int first_throw, second_throw;
		while(frame_count)
		{
			in_str_ >> first_throw;
			if(first_throw < PINS_PER_FRAME)
			{
				in_str_ >> second_throw;
			}
			else if(first_throw > PINS_PER_FRAME)
			{
				//this should never happen
				std::cerr << "More than " << PINS_PER_FRAME << " knocked over: "
						<< first_throw << std::endl << "Input file format error!" << std::endl;
			}
			else
			{
				//we have a strike
			}
			frame_count--;

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
