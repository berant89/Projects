#include "CourtLineDetector.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

//Defines
#define SHOW_INTERMEDIATES 0
typedef uchar BYTE;
/**
 * @struct Holds the configuration for thresholding,
 *	Canny and Houghs transform.
 */
typedef struct {
	int thresh;
	int maxval;
	int cThreshold;
	int ratio;
	int kernel_size;
	double rho;
	double theta;
	int lThreshold;
	double minLL;
	double maxLG;
}Config;

using namespace cv;

//Global Variables
Mat nSrc; //Will hold the original image.
Mat nDst; //Will hold the thresholded image.
Mat nDetected_edges; //The output of the Canny detector.
Mat nHlines; //Lines found by the Hough Transform.

Config nConfig;
string nInput = "image.raw"; //Input filename.
//Dimensions of the RAW image.
int nImageSizeX = 1392;
int nImageSizeY = 550;

/**
 * Constructor that loads the image and prepares for processing.
 */
CourtLineDetector::CourtLineDetector(void)
{
	this->load_raw();
	this->cld_setup();
}


CourtLineDetector::~CourtLineDetector(void)
{
}

int CourtLineDetector::run()
{
	this->thresh_canny_line();
	this->name_lines();
	this->write_raw();
	return 0;
}

/**
 * Creates matrices of the same type as nSrc and setups
 * the configuration matrix for thresholding, Canny and Houghs
 * line transform. Along with setting up the values for the above
 * operations.
 */
void CourtLineDetector::cld_setup()
{
	Size src_size = nSrc.size();
	int src_type = nSrc.type();
	//Create the matrices.
	nDst.create(src_size, src_type);
	nDetected_edges.create(src_size, src_type);
	nHlines.create(src_size, src_type);
	nHlines = Scalar::all(0); //Fills the matrix with 0, creating a black background.

	//Setup up config struct.
	nConfig.thresh = 128; //Threshold for thresholding.
	nConfig.maxval = 255; //Value for thresholding.
	nConfig.cThreshold = 40; //Canny hysteresis threshold.
	nConfig.ratio = 3; //Canny recommends uppper:lower ratio between 2:1 and 3:1.
	nConfig.kernel_size = 3; //Canny kernel size.
	nConfig.rho = 1; //Distance resolution of the accumulator in pixels.
	nConfig.theta = CV_PI/180; //Angle resolution of the accumulator in pixels.
	nConfig.lThreshold = 10; //Minimum number of intersections to detect a line.
	nConfig.minLL = 10; //Minimum length of the line.
	nConfig.maxLG = 5; //Maximum allowed gap between points on the same line to link them.
}

/**
 * Draws a line from the given coordinates in the vector.
 * @param l An int vector of size 4 that contains two points.
 */
void draw_lines(Vec4i l)
{
	//Seperate the points into (x1,y1) and (x2,y2).
	int x1 = l[0], y1 = l[1];
	int x2 = l[2], y2 = l[3];
	int thickness = 1; //Thickness of the line to be drawn.
	Scalar colour = Scalar(255, 255, 255); //White
	line(nHlines, Point(x1, y1), Point(x2, y2), colour, thickness, CV_AA);
}

/**
 * Performs, in their respective order, thresholding, Canny and 
 * Probabilistic Hough transform, based on the settings from
 * nConfig. If the SHOW_INTERMEDIATES flag is set, windows will
 * open showing the result of each operation; press any key to
 * continue.
 */
void CourtLineDetector::thresh_canny_line()
{
	int thresh = nConfig.thresh;
	int maxval = nConfig.maxval;
	int cThreshold = nConfig.cThreshold;
	int ratio = nConfig.ratio;
	int kernel_size = nConfig.kernel_size;
	double rho = nConfig.rho;
	double theta = nConfig.theta;
	int lThreshold = nConfig.lThreshold;
	double minLL = nConfig.minLL;
	double maxLG = nConfig.maxLG;


	//Thresholding
	dilate(nSrc, nDst, Mat()); //Dilate the image.
	threshold(nDst, nDst, thresh, maxval, THRESH_BINARY);
	#if SHOW_INTERMEDIATES
	imshow("Threshold", nDst);
	waitKey();
	#endif

	//Canny edge detector
	Canny(nDst, nDetected_edges, cThreshold, cThreshold * ratio, kernel_size, true);
	#if SHOW_INTERMEDIATES
	imshow("Canny", nDetected_edges);
	waitKey();
	#endif

	//Probabilistic Hough line transform
	vector<Vec4i> lines; //Vector of lines of type int.
	HoughLinesP(nDetected_edges, lines, rho, theta, lThreshold, minLL, maxLG);
	std::for_each(lines.begin(), lines.end(), draw_lines);
	#if SHOW_INTERMEDIATES
	imshow("Lines", nHlines);
	waitKey();
	#endif
}

/**
 * Names the lines in the nHlines matrix.
 */
void CourtLineDetector::name_lines()
{
	//Name horizontal lines.
	vector<string> horizLines;
	horizLines.push_back("Baseline");
	horizLines.push_back("Service Line");
	Scalar colour = Scalar(255, 255, 255);
	int skip = 0;
	for(int i = 0; i < nHlines.rows && !horizLines.empty(); ++i)
	{
		double val = norm(nHlines.row(i)/255, NORM_L1);
		switch(val > 227 && !skip)
		{
		case 1:
			putText(nHlines, horizLines.back(), Point(nHlines.cols/2, i), FONT_HERSHEY_PLAIN, 3, colour);
			skip = 1;
			horizLines.pop_back();
			break;
		default:
			if(val < 227 && skip)
				skip = 0;
			break;
		}
	}

	//Name vertical lines.
	vector<string> vertLines;
	vertLines.push_back("Doubles Sideline");
	vertLines.push_back("Singles Sideline");
	vertLines.push_back("Center Service Line");
	vertLines.push_back("Singles Sideline");
	vertLines.push_back("Doubles Sideline");

	vector<Point> locations;
	locations.push_back(Point(10, nHlines.rows *0.25));
	locations.push_back(Point(170, nHlines.rows *0.25));
	locations.push_back(Point(nHlines.cols/2 - 60, nHlines.rows *0.25));
	locations.push_back(Point(nHlines.cols* 0.75 + 50, nHlines.rows *0.25));
	locations.push_back(Point(1250, nHlines.rows *0.25));
	for(int i = 0; i < vertLines.size(); ++i)
		putText(nHlines, vertLines[i], locations[i], FONT_HERSHEY_PLAIN, 1, colour);
}

/**
 * Loads a 8-bit RAW image from a given filename.
 */
void CourtLineDetector::load_raw()
{
	int size = nImageSizeX * nImageSizeY;
	//Load the image
	BYTE* buffer = new BYTE[size];
	FILE* fp = fopen(nInput.c_str(), "rb");
	if(fp)
	{
		fread(buffer, size, 1, fp);
		fclose(fp);
	}
	else
	{
		delete[] buffer; //Delete the buffer and it's content.
		fclose(fp);
		std::cerr<<"Failed to open file: "<<nInput<<std::endl;
		exit(1);
	}
	
	/*
	 * The first three and last two columns are just solid white and blacks
	 * which are unnecessary.
	 */
	nSrc = Mat(nImageSizeY, nImageSizeX, CV_8UC1, buffer).clone().colRange(4, 1389).clone();
	
	delete[] buffer;

	if(!nSrc.data)
	{
		std::cerr<<"Could not create the image matrix nSrc.\n";
		exit(1);
	}

}

/**
 * Creates and writes an 8-bit RAW image with a given filename.
 */
void CourtLineDetector::write_raw()
{
	int size = nHlines.rows * nHlines.cols;
	//Create buffer.
	BYTE* buffer = new BYTE[size];

	//Load image data.
	MatIterator_<BYTE> it = nHlines.begin<BYTE>(), itEnd = nHlines.end<BYTE>();
	for(int i = 0; it != itEnd; ++it, ++i)
		buffer[i] = *it;
	
	//Create filename.
	std::ostringstream filename;
	filename<<"court_lines "<<nHlines.cols<<"x"<<nHlines.rows<<".raw";
	FILE *fp = fopen(filename.str().c_str(), "wb");

	//Check if it opened.
	if(fp)
	{
		fwrite(buffer, size, 1, fp);
		fclose(fp);
		delete[] buffer;
	}
	else
	{
		std::cerr<<filename.str()<<" failed to open to be written to.\n";
		delete[] buffer;
		exit(1);
	}
}