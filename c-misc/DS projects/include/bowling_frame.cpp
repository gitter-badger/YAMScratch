#include <fstream>
#include <iostream>

#include "bowling_frame.h"


BowlingFrame::BowlingFrame()
{

}

bool BowlingFrame::operator< (const BowlingFrame& rhs)
{
	if(rhs.frame_number_ > frame_number_)
		return true;
	else
		//favor this when the two are equal
		return false;
}

//============GETTERS==============
int BowlingFrame::getFrameScore() const
{
	return first_throw_ + second_throw_;
}


//============SETTERS==============

void BowlingFrame::copy(const BowlingFrame& frame)
{
	first_throw_ = frame.first_throw_;
	second_throw_ = frame.second_throw_;
	frame_number_ = frame.frame_number_;
}

void BowlingFrame::setFirstThrow(const int value)
{
	first_throw_ = value;
}

void BowlingFrame::setSecondThrow(const int value)
{
	second_throw_ = value;
}
