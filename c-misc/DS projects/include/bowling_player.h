#ifndef Bowling_Player_
#define Bowling_Player_

#include <string>
#include "bowling_frame.h"

class BowlingPlayer
{
public:
	BowlingPlayer(std::string fname,std::string lname);
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
	void setFrame(const int index,const int first); // try a little polymorphism
	void setFrame(const int index,const int first,const int second);

private:
	std::string first_name_;
	std::string last_name_;
	int num_frames_;
	BowlingFrame *frames_;


};
#endif