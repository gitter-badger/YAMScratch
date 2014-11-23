//#include <string.h>
#include <stdio.h>

void mstrcpy(char *buff, const char *source);

int main(int argc, char *argv[])
{	
	if (argc == 2)
	{
		FILE * file_ptr;
		char * name = argv[1];
		file_ptr = fopen(name,"w+");
		if (file_ptr != NULL)
		{
			fputs("fopen example",file_ptr);
			fclose(file_ptr);
		}
	}
	return 0;
}
