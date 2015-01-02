#include <algorithm>
#include <string>
#include <iostream>

#include "bowling_player.h"
#include "bowling_frame.h"
#include "bowling_constants.h"

BowlingPlayer::BowlingPlayer()
{
	final_score_available_ = false;
	frames_ = new BowlingFrame[TOTAL_SIZE];
	num_frames_ = NUM_FRAMES_PER_GAME;
}

BowlingPlayer::BowlingPlayer(std::string fname, std::string lname, int frames)
{
	first_name_ = fname;
	last_name_ = lname;
	frames_ = new BowlingFrame[frames];
	num_frames_ = frames;
}

BowlingPlayer::~BowlingPlayer()
{
	delete [] frames_;
}

BowlingPlayer& BowlingPlayer::operator = (const BowlingPlayer& rhs)
{
	if(this != &rhs)
	{
		delete [] frames_;
		this->copy(rhs);
	}
	return *this;
}

void BowlingPlayer::copy(const BowlingPlayer& player)
{
	this->first_name_ = player.first_name_;
	this->last_name_ = player.last_name_;
	this->final_score_available_ = player.final_score_available_;
	this->num_frames_ = player.num_frames_;;
	//copy the data
	this->frames_ = new BowlingFrame[this->num_frames_];
	for(int i = 0; i< this->num_frames_; ++i)
		this->frames_[i] = player.frames_[i];
}

bool BowlingPlayer::operator < (const BowlingPlayer &rhs) const
{
	bool result;
	//this only works as long as strings are letters
	int compare = this->getLastNameLowerCaseOnly().compare(rhs.getLastNameLowerCaseOnly());
	if(compare < 0)
		result = true;
	else if(compare > 0)
		result = false;
	else
		{
		compare = (this->getFirstNameLowerCaseOnly().compare(rhs.getFirstNameLowerCaseOnly()));
		if(compare < 0)
			result = true;
		else
			result = false;
	}
	return result;
}

std::string BowlingPlayer::getFullName() const
{
	return first_name_ + std::string(" ") + last_name_;
}

std::string BowlingPlayer::getFirstName() const
{
	return first_name_;
}

std::string BowlingPlayer::getLastName() const
{
	return last_name_;
}

// provide only ASCII letters in lower case
std::string BowlingPlayer::getFirstNameLowerCaseOnly() const
{
	std::string s(first_name_);
	std::transform(s.begin(),s.end(),s.begin(),tolower);
	return s;
}

std::string BowlingPlayer::getLastNameLowerCaseOnly() const
{
	std::string s(last_name_);
	std::transform(s.begin(),s.end(),s.begin(),tolower);
	return s;
}

int BowlingPlayer::getFinalScore()
{
	if(!this->final_score_available_)
	{
		//int cumulative_score = 0;
		//update the cumulative scores
		int accumluator = 0;
		for(int i = 0; i < NUM_FRAMES_PER_GAME; i++)
		{
			accumluator += getAdjustedFrameScore(i);
		}
		this->final_score_ = accumluator;
		this->final_score_available_ = true; // flip the flag
	}
	//now that we have final scores report them
	return final_score_;
}

void BowlingPlayer::setFrame(const int index,const int first)
{
	if(index < 0)
	{
		std::cerr << "Invalid Indexing of BowlingFrame array" << std::endl;
		exit(1);
	}
	else if(index > TOTAL_SIZE)
	{
		std::cerr << "Invalid Indexing of BowlingFrame array" << std::endl;
		exit(1);
	}
	else
	{
		frames_[index].setFirstThrow(first);
		if(first == PINS_PER_FRAME)
			frames_[index].setStrike(true);
	}
	//reset the final score flag
	final_score_available_ = false;
}

void BowlingPlayer::setFrame(const int index,const int first, const int second)
{
	this->setFrame(index,first); // take advantage of all that error checking
	frames_[index].setSecondThrow(second);
	if((first + second ) == PINS_PER_FRAME && !frames_[index].isStrike())
		frames_[index].setSpare(true);

	final_score_available_ = false;
}

int BowlingPlayer::getAdjustedFrameScore(int i)
{
	int score= 0;
	if(frames_[i].isStrike())
	{
		score += PINS_PER_FRAME;
		if(frames_[i+1].isStrike())
		{
			score += PINS_PER_FRAME;
			if (frames_[i+2].isStrike())
			{
				score += PINS_PER_FRAME;
			}
			else
				score += frames_[i+2].getFirstThrow();
		}
		else if(frames_[i+1].isSpare())
		{
			score += PINS_PER_FRAME;
		}
		else
		{
			score += frames_[i+1].getFrameScore();
		}
	}
	else if(frames_[i].isSpare())
	{
		score += PINS_PER_FRAME;
		score += frames_[i+1].getFirstThrow();
	}
	else
		score += frames_[i].getFrameScore();
	return score;
}
