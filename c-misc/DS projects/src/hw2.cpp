#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>

#include "../include/bowling_game.h"
#include "../include/bowling_player.h"
#include "../include/parser_bowling.h"

using namespace std;

int main(int argc, char const *argv[])
{
	if(argc == 3)
	{
		string input_filename(argv[1]);
		string output_filename(argv[2]);
		//cout << input_filename << endl << output_filename << endl;
		
		//create a game
		BowlingGame *game = new BowlingGame();
		ParserBowling *parser= new ParserBowling();
		//parse the input
		parser->readFile(input_filename,game);
		//sort the player names
		//cout << "size " << game->size() << endl; 

		std::sort(game->begin(),game->end());
		/*
		BowlingPlayer* p;
		for(p = game->begin(); p != game->end(); p++)
			cout << p->getFullName() << endl;
		*/
		game->outputScoreBoard(output_filename);

		delete parser;
		delete game;
	}
	/* code */
	return 0;
}