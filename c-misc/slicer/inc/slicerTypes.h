#ifndef SLICER_TYPES_
#define SLICER_TYPES_

/*Alias type to interpret stl format*/
struct Face {
	float n[3];
	float v1[3];
	float v2[3];
	float v3[3];
	uint16_t ac;
};

#endif