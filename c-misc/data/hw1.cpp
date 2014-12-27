#include <iostream>
#include <istream>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <fstream>

using namespace std;



//create a linked list to store each point as it is created
//we will never insert into this list
//we will never walk backwards
//simple implementaation is fine
//afterwards we will walk along and delete every node

//should allow adoption of threaded model with less work
struct EllipsePoint {
	int x; //store the grid location
	int y;
	int index_x; //store the pixel location
	int index_y;
	EllipsePoint *next;
	EllipsePoint *prev;
	bool done; //flag to let the other thread know we are done
};

//create a new point
EllipsePoint *EllipsePoint_create(int x, int y, EllipsePoint *prev, EllipsePoint *next)
{
	EllipsePoint *point = new EllipsePoint;
	assert(point != NULL);
	point->x = x;
	point->y = y;
	point->next = next;
	point->done = false;
	return point;
}

void EllipsePoint_print(EllipsePoint *point)
{
	cout << "(x,y) = (" << point->x << ',' << point->y << ')' << endl;
}

void EllipsePoint_destroy(EllipsePoint *point)
{
	assert(point != NULL);
	delete point;
}

//build one quarter of the ellipse
EllipsePoint *calculate_ellipse(unsigned short A, unsigned short B, EllipsePoint *tail)
{ /* this will create a list of points on the ellipse starting 
	closest to x =0, y= max */
	if ((A > 0x7FFF) || (B > 0x7FFF))
	{
		cout << "too big" << endl;
		return NULL;
	}

	assert(tail != NULL);
	ofstream out("out3.txt");
	out << "width: " << A << " height: " << B << endl;

	long long A2 = A*A; //should be 64 bits on unix platform
	long long B2 = B*B;
	int x_offset = (A+1)%2;
	int y_offset = (B+1)%2;
	//
	out << x_offset << "," << y_offset << endl;
	//create first element of list
	short x = A -1;
	short y = y_offset;

	//manually create the first node
	tail->x = x;
	tail->y = y;
	tail->next = NULL;
	tail->prev = NULL;
	tail->done = true;

	EllipsePoint * head = tail;
	//precompute the squares
	long long x2n0 = x*x;
	long long y2n0 = y*y;
	out << "(x,y) = (" << head->x << ',' << head->y << ')' << endl;
	while(head->y < B-1)
	{		
		//recursively compute the next steps
		long long x2n1 = x2n0 - (x<<2) + 4;
		long long y2n1 = y2n0 + (y<<2) + 4;
		//compute the steps
		//careful for the overflow error using regular abs()
		long long x_step = llabs(B2*x2n1 + A2*y2n0 - A2*B2);
		long long y_step = llabs(B2*x2n0 + A2*y2n1 - A2*B2);
		long long both = llabs(B2*x2n1 + A2*y2n1 - A2*B2);
		long long smallest = x_step;
		if (y_step < smallest)
			smallest = y_step;
		if (both < smallest)
			smallest = both;
		//now that  we have the smallest
		if (smallest == x_step)
		{
			x -= 2; //update x
			x2n0 = x2n1; //update the precompute value
			head = EllipsePoint_create(x,y,NULL,head);
			head->next->prev = head;

		}
		else if (smallest == y_step)
		{
			y += 2;
			y2n0 = y2n1;
			head = EllipsePoint_create(x,y,NULL,head);
			head->next->prev = head;
		}
		else
		{
			x -= 2;
			y += 2;
			x2n0 = x2n1;
			y2n0 = y2n1;
			head = EllipsePoint_create(x,y,NULL,head);
			head->next->prev = head;
		}
		out << "(x,y) = (" << head->x << ',' << head->y << ')' << endl;
	}
	//burn down the x error
	while (x > 1)
	{
		x -= 2;
		head = EllipsePoint_create(x,y,NULL,head);
		head->next->prev = head;
		//testing
		out << "(x,y) = (" << head->x << ',' << head->y << ')' << endl;
	}
	head->done = true; //
	return head;
}

int output_ellipse_moire(short width, short height, string pattern, string filename)
{
	//set the border character here
	const char border = '*';
	//use a non printing control character to make the outline
	const char first_pass = 1;

	//we pass in the tail so that we can run this function and the generator in separate threads
	int x_offset = (width+1)%2;
	int y_offset = (height+1)%2;
	char output_buffer[height][width+1]; //add room for null terminator
	for(int i = 0; i < height; i++)
	{
		for(int j =0; j < width; j++)
		{
			output_buffer[i][j] = ' ';
		}
		output_buffer[i][width] = '\x0';
	}
	//reverse the elements
	EllipsePoint * tail = new EllipsePoint;
	EllipsePoint * node = tail;
	//create new thread here
	EllipsePoint * head = calculate_ellipse(height, width,tail);
	//we reverse the order of the x and y so that the tail has
	//the minimum x coordinate in the y
	//in effect we are reflecting about the y =x axis to increment
	//by x while the algorithim is incrementing y
	while (node != NULL)
	{
		int x = node->y; //swap the x and y
		int y = node->x;
		signed int x_quadrants[4] = {(x - x_offset + width)>>1,\
									(-x - x_offset + width)>>1,\
									(x - x_offset + width)>>1,\
									(-x - x_offset + width)>>1};
		signed int y_quadrants[4] = {(-y - y_offset + height)>>1,\
									(-y - y_offset + height)>>1,\
									(y - y_offset + height)>>1,\
									(y - y_offset + height)>>1};

		for(char i = 0; i < 4; i++)
		{
			int x_index = x_quadrants[i];
			int y_index = y_quadrants[i];
			output_buffer[y_index][x_index] = first_pass; //use a nonprinting control character
			if (x_index >= width>>1)
			{
				//null terminate the string, because x coord is strictly increasing
				//a null will be overwritten if a point has a greater x_coord 
				output_buffer[y_index][x_index+1] = '\x0';
			}
		}
		node = node->prev; // move to next point
	}
	delete node;
	//get the length of the pattern
	size_t ring_length = pattern.length();
	// fill in the pattern
	int crossing_count;
	int pattern_index = 0;
	for(int i = 0; i < height; i++)
	{
		crossing_count = 0;
		bool crossing_flag = false;
		//the rows
		for(int j = 0; j < width; j++)
		{
			if (output_buffer[i][j] == first_pass)
			{
				if(!crossing_flag) crossing_flag = true;
				output_buffer[i][j] = border; //remap the border
			}
			else if (output_buffer[i][j] == '\x0')
			{
				break;
			}
			else if(crossing_flag)
			{
				crossing_flag = false;
				crossing_count++;
			}

			if (crossing_count%2 && !crossing_flag)
			{
				output_buffer[i][j] = pattern[pattern_index];
				pattern_index = (pattern_index + 1)%ring_length;
			}
		}
	}
	//print out the output
	for(int j = 0;j<height;j++)
	{
		cout << output_buffer[j] << endl;
	}
	return 0;
}


void print_commands(string wrong_command)
{
	cerr << '<' << wrong_command << "> Is not a valid command" << endl << endl;
	cerr << "Commands:" << endl;
	cerr << "  square \t\t" << endl;
	cerr << "  right_triangle\tplaceholder" << endl;
	cerr << "  isoceles_triangle\tplaceholder" << endl;
}

void print_usage(void)
{

}

int main(int argc, char *argv[])
{
	if (argc == 5)
	{
		//assign the
		string pattern = string(argv[1]);
		string string_height = string(argv[2]);
		int height = atoi(string_height.c_str());
		string command = string(argv[3]);
		//cout << pattern << height << command  << out_filename << endl;
		ofstream out_str(argv[4]); //open the file for output
		//select from the commands available
		if (command == string("bat"))
		{
			cout << "Im batman" << endl;
		}
		else if (command == string("foo"))
		{
			cout << "not a fool" << endl;
		}
		else if (command == string("circle"))
		{
			int code = output_ellipse_moire(height,height,pattern,argv[4]);
			return code;
		}
		else if (command == string("ellipse"))
		{
			int code = output_ellipse_moire(80,height,argv[4]);
			return code;
		}
		else //default
		{
			print_commands(command);
			exit(1);
		}
		return 0;

	}
	else if (argc == 1)
	{
		//will return NULL if a or b is larger than 0x7FFF
		unsigned short a = 0x7FFF;
		unsigned short b = 0x7FFF;

		unsigned short c = 30;
		EllipsePoint * tail = new EllipsePoint;
		EllipsePoint * head = calculate_ellipse(a,b,tail);
		if (head == NULL)
		{
			return 1;
		}
		EllipsePoint_print(tail);
		EllipsePoint_print(tail->prev);
		EllipsePoint_print(head);
		//clean up the ellipse

		return 0;
	}
}
