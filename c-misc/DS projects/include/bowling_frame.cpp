#include <fstream>
#include <iostream>

#include "bowling_frame.h"


//============GETTERS==============
int BowlingFrame::getNumThrows() const
{
	return num_throws_;
}

int BowlingFrame::getFrameScore() const
{
	return frame_score_;
}


//============SETTERS==============

