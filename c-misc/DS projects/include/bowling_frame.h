#ifndef BowlingFrame_
#define BowlingFrame_


class BowlingFrame
{
public:
	BowlingFrame();
	BowlingFrame(const BowlingFrame& frame) {copy(frame);};
	~BowlingFrame(){};
	
	bool operator< (const BowlingFrame& rhs);
	
	//GETTERS
	int getFrameScore() const;
	bool isStrike();
	bool isSpare();
	int getFirstThrow() {return first_throw_; };
	int getSecondThrow() {return second_throw_; };
	//SETTERS
	void setFirstThrow(const int value);
	void setSecondThrow(const int value);
	void setCumulativeScore(const int score);
	//these latch
	void setStrike(bool state);
	void setSpare(bool state);

	
private: //Representation
	//PRIVATE MEMBER FUNCTIONS
	void copy(const BowlingFrame& frame);

	//REPRESENTATION
	int first_throw_;
	int second_throw_;
	int frame_number_;
	int cumulative_score_;
	bool is_strike_;
	bool is_spare_;

};

#endif