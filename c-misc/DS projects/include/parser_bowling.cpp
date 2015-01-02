#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

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
		int row_count = 1;
		while(!in_str_.eof())
		{
			in_str_ >> first_name;

			std::cout << std::endl << "row count " << row_count << std::endl << std::endl;
			row_count++;
			//we are assuming that the input format is good
			//stronger error checking would go here
			in_str_>> last_name;
			std::cout << first_name << ' ' << last_name << std::endl;
			//create a new player

			BowlingPlayer* player = new BowlingPlayer(first_name,last_name,TOTAL_SIZE);

			int first_throw, second_throw;
			int max_frames = NUM_FRAMES_PER_GAME;
			for(int frame_count = 0; frame_count < max_frames; frame_count++)
			{
				//std::cout << frame_count << " , " << max_frames << std::endl;
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

				std::cout << "throws: " << first_throw << " , " << second_throw
							<< " Strike: " << player->isFrameStrike(frame_count)
							<< " Spare: " << player->isFrameSpare(frame_count) << std::endl;
				//extend the frame count
				if(frame_count == NUM_FRAMES_PER_GAME -1)
				{
					if(player->isFrameStrike(frame_count))
					{
						std::cout << "last frame strike " << frame_count << std::endl;
						//manually get the next two pieces from stream
						in_str_ >> first_throw >> second_throw;
						std::cout << "throws: " << first_throw << " , " << second_throw << std::endl;
						player->setFrame(frame_count+1,first_throw,0);
						player->setFrame(frame_count+2,second_throw,0);
					}
					else if(player->isFrameSpare(frame_count))
					{
						std::cout << "last frame spare " << frame_count << std::endl;
						//manually get the next piece from stream
						in_str_ >> first_throw;
						std::cout << "throws: " << first_throw << " , " << 0 << std::endl;
						player->setFrame(frame_count+1,first_throw,0);
					}
				}

			}
			//check if there is another
			game->pushBackPlayer(*player);
			delete player;
			in_str_ >> std::ws;
			/*char c = in_str_.peek();
			int a = c;
			std::cout << a << std::endl;
			if(c == -1)
				break; */

		}

	}
	else
	{
		std::cerr << "Failed to open for reading file: " << input_filename
					<< std::endl;
	}
	std::cout << "leaving parser" << std::endl;
	return 0;
}
