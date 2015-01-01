#ifndef BowlingFrame_
#define BowlingFrame_


class BowlingFrame
{
public:
	BowlingFrame(int throw_count) : num_throws_(throw_count) {};
	BowlingFrame(const BowlingFrame& frame) {copy(frame);};
	~BowlingFrame(){};
	

	//GETTERS
	int getNumThrows(void) const;
	int getFrameScore(void) const;
	//SETTERS
	void setThrowScores(){};

	
private: //Representation
	//PRIVATE MEMBER FUNCTIONS
	void copy(const BowlingFrame& frame);

	//REPRESENTATION
	int num_throws_;
	int throws_[3];
	int frame_score_;

};

#endif