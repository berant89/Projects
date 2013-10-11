#include <iostream>

#include "CourtLineDetector.h"
#include "CannyEdge.h"
#include "LineUtil.h"
#define TEST 1

using namespace std;

#if !TEST
int main()
{
	//CourtLineDetector cld; //Create the object which setups everything.
	//cld.run(); //Run the program.
	return 0;
}
#else

BYTE* blur(double* kernel, int kernel_size, BYTE* image, int rows, int cols)
{
	BYTE* blurred = new BYTE[rows*cols];
	int half = kernel_size/2;
	int kernel_center = half * (kernel_size + half);
	int end = cols - 4;

	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j <cols; ++j)
		{
			int position = i*cols+j;
			if(i < 4 && j < 4)
			{
				blurred[position] = image[position]*kernel[kernel_center];
			}
		}
	}

	return blurred;
}
int main()
{
	int rows = 1392;
	int cols = 550;
	/*int kernel_size = 5;
	double* kernel = new double[kernel_size*kernel_size];
	double sigma = 1.0;
	CannyEdge cantest(kernel_size, sigma);
	cantest.print_kernel();

	kernel = cantest.getKernel();*/

	int size = 1392*550;
	BYTE* buffer = new BYTE[size];

	vector< vector<BYTE> > image(rows, vector<BYTE>(cols));
	
	image = load_raw("image.raw", rows, cols);

	for(int i = 0; i < rows; ++i)
		for(int j = 0; j < cols; ++j)
			buffer[i*cols+j] = image[i][j];

	//BYTE* blurred = blur(kernel, kernel_size, buffer, rows, cols);
	write_raw("out.raw", buffer, size);
	//write_raw("out.raw", size, blurred);
	delete[] buffer;
	//delete[] kernel;
	return 0;
}
#endif