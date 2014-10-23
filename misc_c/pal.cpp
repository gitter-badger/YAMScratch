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
		char pal[255];
		char rev[255];
		char *qur = argv[1]; //pointer to the string pointer
		char *foo = argv[1];
		char * bar = pal; //pointer to buffer
		char * right = (rev+254);
		//bounds checking of input string
		unsigned int size_input = 1; // use to track overflows of int
		//we remeber to subtract off 1 later in the size comparison
		while(*qur++ && size_input){size_input++;} //figure out how big the string is
		if (--size_input < sizeof(pal))
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
			//--------------------------------------------------------------//
			int max_count = 0;
			int max_index_start = 0;
			int max_index_end = 0;
			int i,j;
			
			for(i=0; i<size_input; i++)
			{
				char *right_ptr = right;
				char *left_ptr = pal;
				int temp_count = 0;
				int temp_index_start = 0;
				int temp_index_end = 0;

				for(j = i; j<size_input; j++)
				{
					printf("i: %u, j: %u,\n",i,j);
					printf("leftchar = %c , rightchar = %c\n",*(left_ptr+j), *right_ptr);

					printf("     The largest palindrome is size %u\n",max_count);
					printf("     The starting index is %x\n", max_index_start);
					printf("     The end index is :    %x\n\n", max_index_end);

					if (left_ptr[j] == *right_ptr++)
					{
						temp_index_start = temp_index_start ? temp_index_start : j;
						
						temp_count++;
						printf("__________same, temp count = %u, max_count = %u\n",temp_count,max_count);

						max_index_end = (max_count < temp_count) ? j : max_index_end;
						
						max_count = (max_count < temp_count) ? temp_count : max_count ;
						printf("The largest palindrome is size %u\n",max_count);
						printf("The end index is %x\n", max_index_end);
					}
					else
					{

						 //reset the count
						temp_count = 0; //zero the temp count
					}
				}
			}
			printf("The largest palindrome is size %u\n",max_count);
			printf("The starting index is %x\n", max_index_start);
			printf("The end index is :    %x\n", max_index_end);
		}
		else
		{
			std::cout << "too big input" << std::endl;
		}

	}
	return 0;
}