#ifndef _IOSTREAM_
#include <iostream>
#endif

#include <stdlib.h>
#include <assert.h>

#include "ellipse.h"

using namespace std;

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
		std::cout << "too big" << std::endl;
		return NULL;
	}
	assert(tail != NULL);
	long long A2 = A*A; //should be 64 bits on unix platform
	long long B2 = B*B;
	int x_offset = (A+1)%2;
	int y_offset = (B+1)%2;
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
	}
	//burn down the x error
	while (x > 1)
	{
		x -= 2;
		head = EllipsePoint_create(x,y,NULL,head);
		head->next->prev = head;
	}
	head->done = true; // finish the list
	return head;
}
