#include <iostream>
#include <stdlib.h>
#include <string>

#include "../include/bowling_game.h"
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
		cout << "hi" <<  a<<endl;
	}
	/* code */
	return 0;
}