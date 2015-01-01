#ifndef Bowling_Player_
#define Bowling_Player_

#include <string>

class BowlingPlayer
{
public:
	BowlingPlayer(std::string name) : name_(name) {};
	~BowlingPlayer();

private:
	std::string name_;

};
#endif