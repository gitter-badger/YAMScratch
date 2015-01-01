#ifndef Bowling_Player_
#define Bowling_Player_

#include <string>
#include "bowling_frame.h"

class BowlingPlayer
{
public:
	BowlingPlayer();
	BowlingPlayer(std::string fname,std::string lname,int frames);
	BowlingPlayer(const BowlingPlayer& p) {copy(p);};
	~BowlingPlayer();

	//OPERATORS
	bool operator < (const BowlingPlayer &rhs) const;
	BowlingPlayer& operator = (const BowlingPlayer& player);

	//GETTERS
	std::string getFullName() const;
	std::string getFirstName() const;
	std::string getLastName() const;
	std::string getFirstNameLowerCaseOnly() const;
	std::string getLastNameLowerCaseOnly() const;
	int getFinalScore();

	bool isFrameStrike(int index) {return frames_[index].isStrike(); };
	bool isFrameSpare(int index) {return frames_[index].isSpare(); };

	//SETTERS
	void setFrame(const int index,const int first); // try a little polymorphism
	void setFrame(const int index,const int first,const int second);


	//

private:
	void copy(const BowlingPlayer& rhs);
	std::string first_name_;
	std::string last_name_;
	bool final_score_available_;
	int num_frames_;
	BowlingFrame *frames_;



};
#endif