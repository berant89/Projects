#include <sstream>

#include "CourtLineDetector.h"
#include "ImageUtil.h"
#include "RAWUtil.h"

using namespace std;

CourtLineDetector::CourtLineDetector(int rows, int cols, char* filename, int t1, int t2, int kernel_size = 3, double sigma = 1.0)
{
	//Load the image
	fImage = load_raw(filename, rows, cols);
	//Convert it to an image of type double.
	fD_image = BYTE2double(fImage);

	//hysteresis threshold values for Sobel.
	fT1 = t1;
	fT2 = t2;

	//Create the Gaussian kernel.
	fGkernel = gaussKernel(kernel_size, sigma);
	
	//Create the Sobel masks
	int sobel_size = 3;
	Mati GX(sobel_size, Veci(sobel_size));
	Mati GY(sobel_size, Veci(sobel_size));

	//Sobel Horizontal Mask
	GX[0][0] = 1; GX[0][1] = 0; GX[0][2] = -1;
	GX[1][0] = 2; GX[1][1] = 0; GX[1][2] = -2;
	GX[2][0] = 1; GX[2][1] = 0; GX[2][2] = -1;
	
	//Sobel Vertical Mask
	GY[0][0] = 1;  GY[0][1] = 2;  GY[0][2] = 1;
	GY[1][0] = 0;  GY[1][1] = 0;  GY[1][2] = 0;
	GY[2][0] = -1; GY[2][1] = -2; GY[2][2] = -1;

	fGX = GX;
	fGY = GY;

	//Create the line masks
	int line_size  = 3;
	Mati LM(line_size, Veci(line_size));
	//Horizontal line detect with a pixel width of one.
	LM[0][0] = -1; LM[0][1] = -1; LM[0][2] = -1;
	LM[1][0] = 2;  LM[1][1] = 2;  LM[1][2] = 2;
	LM[2][0] = -1; LM[2][1] = -1; LM[2][2] = -1;
	
	//Vertical line detect with a pixel width of one.
	/*LM[0][0] = -1; LM[0][1] = 2; LM[0][2] = -1;
	LM[1][0] = -1; LM[1][1] = 2; LM[1][2] = -1;
	LM[2][0] = -1; LM[2][1] = 2; LM[2][2] = -1;*/

	fLm = LM;
}

CourtLineDetector::~CourtLineDetector()
{
}

int CourtLineDetector::run()
{
	fBlurred = gaussian_blur(fGkernel, fD_image);
	fEdges = sobel_edge(fGX, fGY, fBlurred, fT1, fT2);

	/*
	hough_lines is creating points that are larger than the dimensions of 
	the image. Might be due to accumulator or implementation of hough_lines.
	 */
	/*fAccu = hough_transform(fEdges);
	fLines = hough_lines(fImage.size(), fImage[0].size(), 3, fAccu);*/

	fLines = line_detect(fLm, fEdges);

	//Create filenames with their dimensions.
	stringstream filename;
	filename<<"out "<< fImage[0].size()<<"x"<<fImage.size()<<".raw";
	write_raw(filename.str().c_str(), fImage);
	
	filename.str(string());
	filename<<"blurred "<< fImage[0].size()<<"x"<<fImage.size()<<".raw";
	write_raw(filename.str().c_str(), double2BYTE(fBlurred));

	filename.str(string());
	filename<<"edges "<< fImage[0].size()<<"x"<<fImage.size()<<".raw";
	write_raw(filename.str().c_str(), double2BYTE(fEdges));

	filename.str(string());
	filename<<"lines "<< fImage[0].size()<<"x"<<fImage.size()<<".raw";
	write_raw(filename.str().c_str(), double2BYTE(fLines));

	filename.str(string());
	write_coord("coords.txt", double2BYTE(fLines));

	return 0;
}