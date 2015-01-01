
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
