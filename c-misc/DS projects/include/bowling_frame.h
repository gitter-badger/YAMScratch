#ifndef BowlingFrame_
#define BowlingFrame_


class BowlingFrame
{
public:
	BowlingFrame(int throw_count) : num_throws_(throw_count) {};
	~BowlingFrame();
	BowlingFrame(const BowlingFrame &obj);

	//GETTERS
	int getNumThrows(void) const;
	int getFrameScore(void) const;
	//SETTERS
	void setThrowScores();

	
private: //Representation
	int num_throws_;
	int throws_[];
	int frame_score_;

};

#endif