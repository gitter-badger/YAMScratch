#ifndef Bowling_Player_
#define Bowling_Player_

#include <string>
#include <vector>
#include "bowling_frame.h"

class BowlingPlayer
{
public:
	BowlingPlayer(std::string fname,std::string lname) : first_name_(fname), last_name_(lname) {};
	~BowlingPlayer();

	//OPERATORS
	bool operator< (const BowlingPlayer &rhs) const;

	//GETTERS
	std::string getFullName() const;
	std::string getFirstName() const;
	std::string getLastName() const;
	std::string getFirstNameLowerCaseOnly() const;
	std::string getLastNameLowerCaseOnly() const;
	int getFinalScore() const;

	//SETTERS
private:
	std::string first_name_;
	std::string last_name_;
	std::vector<BowlingFrame> frames_;


};
#endif