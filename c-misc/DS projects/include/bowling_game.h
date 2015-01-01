#ifndef Bowling_Game_
#define Bowling_Game_

#include "bowling_player.h"

//container class for BowlingPlayers
class BowlingGame
{
public:
	BowlingGame() {this->create();};
	BowlingGame(int size, const BowlingPlayer& p = BowlingPlayer()) {this->create(size,p);}
	~BowlingGame() {delete [] players_;};

	//GETTERS
	void clear() {delete [] players_; create();};
	int size() {return num_players_;};
	//SETTERS
	void pushBackPlayer(const BowlingPlayer& player);


	
	typedef BowlingPlayer* iterator;
	typedef const BowlingPlayer* const_iterator;
	//Iterators
	iterator begin() {return players_; };
	const_iterator begin() const {return players_; };
	iterator end() {return players_ + num_players_; };
	const_iterator end() const {return players_ + num_players_; };


private:
	void create();
	void create(int size, const BowlingPlayer& player);
	int players_alloc_;
	int num_players_;
	BowlingPlayer *players_;
	
};
#endif