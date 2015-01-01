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
		cout << input_filename << endl << output_filename << endl;
		
		//create a game
		BowlingGame *game = new BowlingGame();
		ParserBowling *parser= new ParserBowling();
		//parse the input
		int a = parser->readFile(input_filename,game);
		//sort the player names
		std::sort(game->begin(),game->end());
		BowlingPlayer* p;

		for(p = game->begin(); p != game->end(); p++)
			cout << p->getFirstName() << " " << p->getLastName() << endl;

		cout << "hi" <<  a<<endl;
	}
	/* code */
	return 0;
}