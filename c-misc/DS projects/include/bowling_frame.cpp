#include <fstream>
#include <iostream>

#include "bowling_frame.h"

BowlingFrame::BowlingFrame(char throw_count)
{
	throws_ = new int[throw_count];
}

BowlingFrame::~BowlingFrame()
{
	delete [] throws_; //clean up our array
}

BowlingFrame::BowlingFrame(const BowlingFrame &obj)
//============GETTERS==============
int BowlingFrame::getNumThrows()
{
	return num_throws_;
}

int BowlingFrame::getFrameScore()
{
	return frame_score_;
}


//============SETTERS==============

