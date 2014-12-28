#ifndef ELLIPSE_
#define ELLIPSE_

//node representation
struct EllipsePoint {
	int x; //store the grid location
	int y;
	int index_x; //store the pixel location
	int index_y;
	EllipsePoint *next;
	EllipsePoint *prev;
	bool done; //flag to let the other thread know we are done
};

//typedef struct EllipsePoint EllipsePoint;
//constructor
EllipsePoint *EllipsePoint_create(int x, int y, EllipsePoint *prev, EllipsePoint *next);
//destructor
void EllipsePoint_destroy(EllipsePoint *point);

void EllipsePoint_print(EllipsePoint *point);

EllipsePoint *calculate_ellipse(unsigned short A, unsigned short B, EllipsePoint *tail);

#endif