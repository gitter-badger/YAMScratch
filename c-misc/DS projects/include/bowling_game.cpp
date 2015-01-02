
#include "bowling_game.h"

void BowlingGame::create()
{
	num_players_ = players_alloc_ = 0;
	players_ = NULL;
}

void BowlingGame::create(int size, const BowlingPlayer& player)
{
	players_ = new BowlingPlayer[size];
	num_players_ = players_alloc_ = size;
	for (BowlingPlayer* ptr = players_; ptr != players_ + num_players_; ++ptr)
		*ptr = player;
}

void BowlingGame::pushBackPlayer(const BowlingPlayer& player)
{
	//see if we need to allocate more storage
	if(players_alloc_ == num_players_)
	{
		players_alloc_ *= 2; 
		if(players_alloc_ < 1)
			players_alloc_ = 1;
		BowlingPlayer* new_players = new BowlingPlayer[players_alloc_];
		for(int i = 0; i<num_players_; ++i)
		{
			new_players[i] = players_[i];
		}
		delete [] players_;
		players_ = new_players;
	}
	//add on the new player
	players_[num_players_] = player;
	num_players_++;
}
