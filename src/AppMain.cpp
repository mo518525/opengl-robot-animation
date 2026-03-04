#include <iostream>
#include "ApplicationWindow.h"


int main(void)
{
	try
	{
		ApplicationWindow mg;
		mg.run();
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() << "\nDruecke eine Taste zum beenden." << std::endl;
		std::getchar();
	}


	return 1;
}
