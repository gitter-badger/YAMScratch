#include <algorithm>

#include "bowling_player.h"

#ifndef NUM_FRAMES_PER_GAME
#define NUM_FRAMES_PER_GAME 10
#endif


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