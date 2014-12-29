#include <iostream>
#include <istream>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <fstream>

#include "ellipse.h"

using namespace std;

//create a linked list to store each point as it is created
//we will never insert into this list
//we will never walk backwards
//simple implementaation is fine
//afterwards we will walk along and delete every node

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		//for testing
		EllipsePoint * tail = new EllipsePoint;
		EllipsePoint * head = calculate_ellipse(30,30,tail);
		EllipsePoint * node = head;
		while(node != NULL)
		{
			EllipsePoint_print(node);
			node = node->next;
		}


		return 0;
	}
	else if (argc == 4)
	{
		int width = atoi(argv[1]); //careful of overflow
		string input_file(argv[2]);
		string output_file(argv[3]);

		ifstream in_str(input_file.c_str());

		ofstream out_str(output_file.c_str());

		string buffer;

		while(!in_str.eof())
		{
			buffer = in_str.getline();
			//cout << buffer << endl;
			out_str << buffer << "          " << buffer.length() << endl;
			buffer = ""; //clear the string

		}
		
		return 0;
	}

	
}
