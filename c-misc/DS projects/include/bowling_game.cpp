#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "bowling_game.h"
#include "bowling_constants.h"
#include "bowling_player.h"

void BowlingGame::create()
{
	num_players_ = players_alloc_ = 0;
	players_ = NULL;
	max_name_size_ = 0;
}

void BowlingGame::create(int size, const BowlingPlayer& player)
{
	max_name_size_ = 0;
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

void BowlingGame::outputScoreBoard(std::string filename)
{
	//sort the players
	std::sort(this->begin(),this->end());
	//find the max name size
	this->updateMaxPlayerNameLength();
	//prepare the output buffers
	std::stringstream name_line, score_line,vsp;

	int score_board_length = LEFT_NAME_PAD + max_name_size_ + RIGHT_NAME_PAD \
							+ NUM_FRAMES_PER_GAME * RIGHT_NAME_PAD \
							+ NUM_FRAMES_PER_GAME * NUM_THROWS_PER_FRAME * THROW_VALUE_WIDTH \
							+ THROW_VALUE_WIDTH;
	std::string border(score_board_length, HORIZONTAL_SEPERATOR);

	//open the output stream
	std::ofstream out_str_(filename.c_str());

	for(BowlingPlayer* player = this->begin(); player != this->end(); player++)
	{
		//clear the buffers
		name_line.str(std::string());
		name_line.clear();
		score_line.str(std::string());
		score_line.clear();

		name_line << std::setw(LEFT_NAME_PAD) << std::setfill(FILL) << std::left << VERTICAL_SEPERATOR;
		name_line << std::setw(max_name_size_) << std::setfill(FILL) << std::left << player->getFullName();
		name_line << std::setw(RIGHT_NAME_PAD) << std::setfill(FILL) << std::right << VERTICAL_SEPERATOR;
		score_line << std::setw(LEFT_NAME_PAD+max_name_size_) << std::setfill(FILL) << std::left << VERTICAL_SEPERATOR;
		score_line << std::setw(RIGHT_NAME_PAD) << std::setfill(FILL) << std::right << VERTICAL_SEPERATOR;

		std::string first_score;
		std::string second_score;
		int cumulative_score = 0;
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
				first_score =  this->filterScoreForZeros(player->getFrameFirstThrow(i));

			}
			else
			{
				first_score =  this->filterScoreForZeros(player->getFrameFirstThrow(i));
				second_score =  this->filterScoreForZeros(player->getFrameSecondThrow(i));

			}
			cumulative_score += player->getAdjustedFrameScore(i);
			//fill in the values calculated
			name_line << std::setw(THROW_VALUE_WIDTH) << std::setfill(FILL) << std::right << first_score;
			name_line << std::setw(THROW_VALUE_WIDTH) << std::setfill(FILL) << std::right << second_score;
			name_line << std::setw(RIGHT_NAME_PAD) << std::setfill(FILL) << std::right << VERTICAL_SEPERATOR;
			score_line << std::setw(2*THROW_VALUE_WIDTH) << std::setfill(FILL) << std::right << cumulative_score;
			score_line << std::setw(RIGHT_NAME_PAD) << std::setfill(FILL) << std::right << VERTICAL_SEPERATOR;
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
				
					third_score = this->filterScoreForZeros(player->getFrameFirstThrow(NUM_FRAMES_PER_GAME + 1));
			}
			else if(player->isFrameSpare(NUM_FRAMES_PER_GAME))
			{
				
				second_score = this->filterScoreForZeros(player->getFrameFirstThrow(NUM_FRAMES_PER_GAME));
				third_score = SPARE_CHAR;
			}
			else
			{
				second_score = this->filterScoreForZeros(player->getFrameFirstThrow(NUM_FRAMES_PER_GAME));
				third_score = this->filterScoreForZeros(player->getFrameSecondThrow(NUM_FRAMES_PER_GAME));
			}	
		}
		else if(player->isFrameSpare(NUM_FRAMES_PER_GAME-1))
		{
			second_score = SPARE_CHAR;
			first_score = this->filterScoreForZeros(player->getFrameFirstThrow(NUM_FRAMES_PER_GAME -1));
			
			if(player->isFrameStrike(NUM_FRAMES_PER_GAME))
			{
				third_score = STRIKE_CHAR;
			}
			else
				third_score = this->filterScoreForZeros(player->getFrameFirstThrow(NUM_FRAMES_PER_GAME));
		}
		else
		{
			first_score =  this->filterScoreForZeros(player->getFrameFirstThrow(NUM_FRAMES_PER_GAME-1));
			second_score =  this->filterScoreForZeros(player->getFrameSecondThrow(NUM_FRAMES_PER_GAME-1));
			third_score = FILL;
		}
		cumulative_score += player->getAdjustedFrameScore(NUM_FRAMES_PER_GAME - 1);
		name_line << std::setw(THROW_VALUE_WIDTH) << std::setfill(FILL) << std::right << first_score;
		name_line << std::setw(THROW_VALUE_WIDTH) << std::setfill(FILL) << std::right << second_score;
		name_line << std::setw(THROW_VALUE_WIDTH) << std::setfill(FILL) << std::right << third_score;
		score_line << std::setw(3*THROW_VALUE_WIDTH) << std::setfill(FILL) << std::right << cumulative_score;
		//close the last one
		name_line << std::setw(RIGHT_NAME_PAD) << std::setfill(FILL) << std::right << VERTICAL_SEPERATOR;
		score_line << std::setw(RIGHT_NAME_PAD) << std::setfill(FILL) << std::right << VERTICAL_SEPERATOR;
		//stream the results out
		out_str_ << border << std::endl << name_line.str() << std::endl << score_line.str() << std::endl;
		//set the final score
		player->updateFinalScore();
	}
	out_str_ << border << std::endl << std::endl;
	//sort the players by score
	/*
	for(BowlingPlayer* player = this->begin(); player != this->end(); player++)
	{
		std::cout << player->getFullName() << " " << player->getFinalScore() << std::endl;
	}
*/
	std::sort(this->begin(),this->end(),BowlingPlayer::sortByScore);
	//print out the player names
	for(BowlingPlayer* player = this->begin(); player != this->end(); player++)
	{
		out_str_ << std::setw(max_name_size_+1) << std::setfill(FILL) << std::left << player->getFullName();
		out_str_ << std::setw(THROW_VALUE_WIDTH*NUM_THROWS_PER_FRAME) << std::setfill(FILL) << std::right << player->getFinalScore();
		out_str_ << std::endl;
	}
	//trailing whitespace
	out_str_ << std::endl;
}

std::string BowlingGame::filterScoreForZeros(int val)
{
	std::string str;
	if(val)
	{
		str = static_cast<std::ostringstream*>(&(std::ostringstream() << val))->str();
	}
	else
		str = SCRATCH_CHAR;
	return str;
}
