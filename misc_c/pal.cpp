#include <iostream>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "usage: " << argv[0] << " candidate_palindrome_string" << std::endl;
	}
	else
	{
		char pal[10];
		char rev[255];
		char *foo,*foo2 = argv[1]; //pointer to the string pointer
		char *bar = pal; //pointer to buffer
		char *right = (rev+254);
		//bounds checking of input string
		unsigned int size_input = 1; // use to track overflows of int
		//we remeber to subtract off 1 later in the size comparison
		while(*foo++ && size_input){size_input++;} //figure out how big the string is
		if (size_input-1 < sizeof(pal))
			{
			//strcpy
			*bar = *foo;
			while(*foo++){*++bar = *foo;}
			//end strcpy
			printf("bar is: %s\n\r", pal );

			foo =  argv[1];
			//reverse strcpy
			*right = 0x00;
			*--right = *foo;
			while(*++foo){*--right = *foo;}
			//end reverse strcpy
			printf("right is : %s\n\r",right);
		}
		else
		{
			std::cout << "too big input" << std::endl;
		}
	}
	return 0;
}