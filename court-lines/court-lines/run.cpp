#include <iostream>
#include "CourtLineDetector.h"


int main()
{
	CourtLineDetector cld(550, 1392, "image.raw", 130, 120, 7, 3.5); //Create the object which setups everything.
	cld.run(); //Run the program.
	return 0;
}