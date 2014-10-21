#include <iostream>


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "That was wrog, usage:" << argv[0] << "string" << std::endl;
	}
	else
	{
		std::cout <<"you are learning"<< std::endl;
	}
	return 0;
}