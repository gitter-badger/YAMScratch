#include <string>
#include <fstream>
#include <iostream>

#include "parser_bowling.h"
#include "bowling_frame.h"
#include "bowling_game.h"
#include "bowling_player.h"
#include "bowling_constants.h"

int ParserBowling::readFile(std::string input_filename, BowlingGame* game)
{

	in_str_.open(input_filename.c_str(), std::ifstream::in);
	if(in_str_.is_open())
	{
		std::string first_name;
		std::string last_name;
		char c;
		c = in_str_.peek();
		while(c != EOF)
		{
			
			//we are assuming that the input format is good
			//stronger error checking would go here
			in_str_ >> first_name >> last_name;
			//create a new player

			BowlingPlayer* player = new BowlingPlayer(first_name,last_name,TOTAL_SIZE);

			int first_throw, second_throw;
			int max_frames = NUM_FRAMES_PER_GAME;
			for(int frame_count = 0; frame_count < max_frames; frame_count++)
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
					second_throw = 0;
				}
				//assignment to frame
				player->setFrame(frame_count,first_throw,second_throw);
				//extend the frame count
				if(frame_count == NUM_FRAMES_PER_GAME -1)
				{
					if(player->isFrameStrike(frame_count))
					{
						std::cout << "last frame strike" << frame_count << std::endl;
						max_frames += 2;
					}
					else if(player->isFrameSpare(frame_count))
					{
						std::cout << "last frame spare" << frame_count << std::endl;
						max_frames += 1;
					}
				}

			}
			//check if there is another
			c = in_str_.peek();
		}

	}
	else
	{
		std::cerr << "Failed to open for reading file: " << input_filename
					<< std::endl;
	}
	return 0;
}
