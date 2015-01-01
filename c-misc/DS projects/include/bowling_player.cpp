#include <algorithm>
#include <string>
#include <iostream>

#include "bowling_player.h"
#include "bowling_frame.h"

#ifndef NUM_FRAMES_PER_GAME
#define NUM_FRAMES_PER_GAME 10
#endif

#ifndef END_PADDING_FRAMES
#define END_PADDING_FRAMES 2
#endif

#define TOTAL_SIZE NUM_FRAMES_PER_GAME + END_PADDING_FRAMES

BowlingPlayer::BowlingPlayer(std::string fname, std::string lname)
{
	first_name_ = fname;
	last_name_ = lname;
	frames_ = new BowlingFrame[TOTAL_SIZE];
	num_frames_ = NUM_FRAMES_PER_GAME;
}

BowlingPlayer::~BowlingPlayer()
{
	delete [] frames_;
}

bool BowlingPlayer::operator< (const BowlingPlayer &rhs) const
{
	bool result;
	//this only works as long as strings are letters
	switch (this->getLastNameLowerCaseOnly().compare(rhs.getLastNameLowerCaseOnly()))
	{
		case -1:
			result = true;
			break;
		case 1:
			result = false;
			break;
		case 0:
			//evaluate the first names
			switch(this->getFirstNameLowerCaseOnly().compare(rhs.getFirstNameLowerCaseOnly()))
			{
				case -1:
					result = true;
					break;
				case 1:
					result = false;
					break;
				case 0:
					//if they are exactly the same favor this string 
					result = true;
					break;
			}
			break;
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

int BowlingPlayer::getFinalScore() const
{
	return 0;
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
	}
}

void BowlingPlayer::setFrame(const int index,const int first, const int second)
{
	this->setFrame(index,first);
	frames_[index].setSecondThrow(second);
}
