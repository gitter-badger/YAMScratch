#include <iostream>
#include <stdlib.h>
#include <assert.h>

using namespace std;



//create a linked list to store each point as it is created
//we will never insert into this list
//we will never walk backwards
//simple implementaation is fine
//afterwards we will walk along and delete every node
struct EllipsePoint {
	int x;
	int y;
	struct EllipsePoint *next;
};

//create a new point
struct EllipsePoint *EllipsePoint_create(int x, int y,struct EllipsePoint *next)
{
	struct EllipsePoint *point = new EllipsePoint;
	assert(point !=NULL);
	point->x = x;
	point->y = y;
	point->next = next;
	return point;
}

void EllipsePoint_print(EllipsePoint *point)
{
	cout << "(x,y) = (" << point->x << ',' << point->y << ')' << endl;
}

void EllipsePoint_destroy(struct EllipsePoint *point)
{
	assert(point != NULL);
	delete point;
}

//build one quarter of the ellipse
struct EllipsePoint *calculate_ellipse(short A, short B)
{ /* this will create a list of points on the ellipse starting 
	closest to x =0, y= max */
<<<<<<< HEAD
=======
	assert(A <= 0xFFFF); // testing to prevent overflows
	assert(B <= 0xFFFF);
>>>>>>> 27835bb1912b1d89968f985994853606ae62534f
	long A2 = A*A; //should be 64 bits on unix platform
	long B2 = B*B;
	char x_offset = (A+1)%2;
	char y_offset = (B+1)%2;
	//create first element of list
	short x = A -1;
	short y = y_offset;
	EllipsePoint * head = EllipsePoint_create(x,y,NULL);
	//precompute the squares
	long x2n0 = x*x;
	long y2n0 = y*y;

	while(head->y < B-1)
	{
		//recursively compute the next steps
		long x2n1 = x2n0 - 4*x + 4;
		long y2n1 = y2n0 + 4*y + 4;
		//compute the steps
		long x_step = abs(B2*x2n1 + A2*y2n0 - A2*B2);
		long y_step = abs(B2*x2n0 + A2*y2n1 - A2*B2);
		long both = abs(B2*x2n1 + A2*y2n1 - A2*B2);
		long smallest = x_step;
		if (y_step < smallest)
			smallest = y_step;
		if (both < smallest)
			smallest = both;
		//now that  we have the smallest
		if (smallest == x_step)
		{
			x -= 2; //update x
			x2n0 = x2n1; //update the precompute value
			head = EllipsePoint_create(x,y,head);
		}
		else if (smallest == y_step)
		{
			y += 2;
			y2n0 = y2n1;
			head = EllipsePoint_create(x,y,head);
		}
		else
		{
			x -= 2;
			y += 2;
			x2n0 = x2n1;
			y2n0 = y2n1;
			head = EllipsePoint_create(x,y,head);
		}
	
	}
	//burn down the x error
	while (x > 1)
	{
		x -= 2;
		head = EllipsePoint_create(x,y,head);
	}
	return head;
}

int main(int argc, char *argv[])
{
	struct EllipsePoint * head = calculate_ellipse(10,10);
	EllipsePoint * node = head;
	while (node != NULL)
	{
		EllipsePoint_print(node);
		node = node->next;
	}
<<<<<<< HEAD
	cout << "Head node" << endl;
	EllipsePoint_print(head);
=======
>>>>>>> 27835bb1912b1d89968f985994853606ae62534f
	return 0;
}
