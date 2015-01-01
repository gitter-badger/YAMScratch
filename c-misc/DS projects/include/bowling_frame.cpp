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

bool BowlingFrame::isStrike()
{
	return is_strike_;
}

bool BowlingFrame::isSpare()
{
	return is_spare_;
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

void BowlingFrame::setStrike(bool state)
{
	if(state)
	{
		is_strike_ = state;
		is_spare_ = false;
	}
	else
		is_strike_ = false;
}

void BowlingFrame::setSpare(bool state)
{
	if(state)
	{
		is_spare_ = state;
		is_strike_ = false;
	}
	else
		is_spare_ = false;
}


