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
	int getFrameScore(void) const;
	//SETTERS
	void setFirstThrow(const int value);
	void setSecondThrow(const int value);
	void setCumulativeScore(const int score);

	
private: //Representation
	//PRIVATE MEMBER FUNCTIONS
	void copy(const BowlingFrame& frame);

	//REPRESENTATION
	int first_throw_;
	int second_throw_;
	int frame_number_;
	int cumulative_score_;

};

#endif