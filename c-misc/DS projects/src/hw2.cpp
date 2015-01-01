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
	}
	/* code */
	return 0;
}