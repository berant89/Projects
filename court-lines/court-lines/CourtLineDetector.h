#pragma once
#include "VecMatTypes.h"

class CourtLineDetector
{
public:
	CourtLineDetector(int, int, char*, int, int, int, double);
	~CourtLineDetector(void);
	int run();
private:
	int fT1;
	int fT2;
	Matb fImage;
	Matd fD_image;
	Matd fGkernel;
	Mati fGX;
	Mati fGY;
	Mati fLm;
	Matd fBlurred;
	Matd fEdges;
	Mati fAccu;
	Matd fLines;
};

