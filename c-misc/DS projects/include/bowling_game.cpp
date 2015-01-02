#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "bowling_game.h"
#include "bowling_constants.h"

void BowlingGame::create()
{
	num_players_ = players_alloc_ = 0;
	players_ = NULL;
}

void BowlingGame::create(int size, const BowlingPlayer& player)
{
	players_ = new BowlingPlayer[size];
	num_players_ = players_alloc_ = size;
	for (BowlingPlayer* ptr = players_; ptr != players_ + num_players_; ++ptr)
		*ptr = player;
}

void BowlingGame::pushBackPlayer(const BowlingPlayer& player)
{
	//see if we need to allocate more storage
	if(players_alloc_ == num_players_)
	{
		players_alloc_ *= 2; 
		if(players_alloc_ < 1)
			players_alloc_ = 1;
		BowlingPlayer* new_players = new BowlingPlayer[players_alloc_];
		for(int i = 0; i<num_players_; ++i)
		{
			new_players[i] = players_[i];
		}
		delete [] players_;
		players_ = new_players;
	}
	//add on the new player
	players_[num_players_] = player;
	num_players_++;
}

void BowlingGame::updateMaxPlayerNameLength()
{
	BowlingPlayer* p;
	for(p = this->begin(); p != this->end(); ++p)
	{
		int temp = p->getFullName().length();
		if ( temp > max_name_size_)
			max_name_size_ = temp;
	}
}

void BowlingGame::outputScoreBoard()
{
	//sort the players
	std::sort(this->begin(),this->end());
	//find the max name size
	this->updateMaxPlayerNameLength();
	//prepare the output buffers
	std::stringstream name_line, score_line,vsp;

	for(BowlingPlayer* player = this->begin(); player != this->end(); player++)
	{
		name_line.str(std::string());
		name_line.clear();

		name_line << std::setw(LEFT_NAME_PAD) << std::setfill(FILL) << std::left << VERTICAL_SEPERATOR;
		name_line << std::setw(max_name_size_) << std::setfill(FILL) << std::left << player->getFullName();
		name_line << std::setw(RIGHT_NAME_PAD) << std::setfill(FILL) << std::right << VERTICAL_SEPERATOR;
		
		std::string first_score;
		std::string second_score;
		for(int i = 0; i < NUM_FRAMES_PER_GAME-1; i++)
		{
			//get the values
			if(player->isFrameStrike(i))
			{
				first_score = FILL;
				second_score = STRIKE_CHAR;
			}
			else if(player->isFrameSpare(i))
			{
				second_score = SPARE_CHAR;
				first_score =  static_cast<std::ostringstream*>(&(std::ostringstream() << player->getFrameFirstThrow(i)) )->str();
			}
			else
			{
				first_score =  static_cast<std::ostringstream*>(&(std::ostringstream() << player->getFrameFirstThrow(i)) )->str();
				second_score =  static_cast<std::ostringstream*>(&(std::ostringstream() << player->getFrameSecondThrow(i)) )->str();
			}
			//fill in the values calculated
			name_line << std::setw(THROW_VALUE_WIDTH) << std::setfill(FILL) << std::right << first_score;
			name_line << std::setw(THROW_VALUE_WIDTH) << std::setfill(FILL) << std::right << second_score;
			name_line << std::setw(RIGHT_NAME_PAD) << std::setfill(FILL) << std::right << VERTICAL_SEPERATOR;
		}
		//fill in the last frame
		std::string third_score;
		if(player->isFrameStrike(NUM_FRAMES_PER_GAME-1))
		{
			first_score = STRIKE_CHAR;
			if(player->isFrameStrike(NUM_FRAMES_PER_GAME))
			{
				second_score = STRIKE_CHAR;
				if(player->isFrameStrike(NUM_FRAMES_PER_GAME + 1))
					third_score = STRIKE_CHAR;
				else
					third_score = static_cast<std::ostringstream*>(&(std::ostringstream() << player->getFrameFirstThrow(NUM_FRAMES_PER_GAME + 1)) )->str();
			}
			else if(player->isFrameSpare(NUM_FRAMES_PER_GAME))
			{
				second_score = static_cast<std::ostringstream*>(&(std::ostringstream() << player->getFrameFirstThrow(NUM_FRAMES_PER_GAME)) )->str();
				third_score = SPARE_CHAR;
			}
			else
			{
				second_score =  static_cast<std::ostringstream*>(&(std::ostringstream() << player->getFrameFirstThrow(NUM_FRAMES_PER_GAME )) )->str();
				third_score =  static_cast<std::ostringstream*>(&(std::ostringstream() << player->getFrameSecondThrow(NUM_FRAMES_PER_GAME )) )->str();
			}	
		}
		else if(player->isFrameSpare(NUM_FRAMES_PER_GAME-1))
		{
			second_score = SPARE_CHAR;
			first_score =  static_cast<std::ostringstream*>(&(std::ostringstream() << player->getFrameFirstThrow(NUM_FRAMES_PER_GAME-1)) )->str();
			if(player->isFrameStrike(NUM_FRAMES_PER_GAME))
			{
				third_score = STRIKE_CHAR;
			}
			else
				third_score =  static_cast<std::ostringstream*>(&(std::ostringstream() << player->getFrameFirstThrow(NUM_FRAMES_PER_GAME )) )->str();
		}
		else
		{
			first_score =  static_cast<std::ostringstream*>(&(std::ostringstream() << player->getFrameFirstThrow(NUM_FRAMES_PER_GAME-1)) )->str();
			second_score =  static_cast<std::ostringstream*>(&(std::ostringstream() << player->getFrameSecondThrow(NUM_FRAMES_PER_GAME-1)) )->str();
			third_score = FILL;
		}
		name_line << std::setw(THROW_VALUE_WIDTH) << std::setfill(FILL) << std::right << first_score;
		name_line << std::setw(THROW_VALUE_WIDTH) << std::setfill(FILL) << std::right << second_score;
		name_line << std::setw(THROW_VALUE_WIDTH) << std::setfill(FILL) << std::right << third_score;

		name_line << std::setw(RIGHT_NAME_PAD) << std::setfill(FILL) << std::right << VERTICAL_SEPERATOR;

		std::cout << name_line.str() << std::endl << player->isFrameSpare(NUM_FRAMES_PER_GAME-1) << std::endl;
	}


}
