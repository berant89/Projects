//#include "CourtLineDetector.h"
//#include "LineUtil.h"
//
//#include <stdlib.h>
//#include <stdio.h>
//#include <iostream>
//
////Defines
//#define SHOW_INTERMEDIATES 0 //Flag to show the images at each operation.
//#define NAME_LINES 1 //Flag for the function name_lines()
//#define DRAW_AXES 1 //Flag for the function draw_axes()
//
//using namespace std;
//
//
///**
// * Holds the configuration for thresholding,
// * Canny and Houghs transform.
// * @var Config::thresh
// * Threshold for thresholding.
// * @var Config::maxval
// * Value for thresholding.
// * @var Config::cThreshold
// * Canny hysteresis threshold.
// * @var Config::ratio
// * Canny recommends uppper:lower ratio between 2:1 and 3:1.
// * @var Config::kernel_size
// * Canny kernel size.
// * @var Config::rho
// * Distance resolution of the accumulator in pixels.
// * @var Config::theta
// * Angle resolution of the accumulator in pixels.
// * @var Config::lThreshold
// * Minimum number of intersections to detect a line.
// * @var Config::minLL
// * Minimum length of the line.
// * @var Config::maxLG
// * Maximum allowed gap between points on the same line to link them.
// */
//typedef struct {
//	int thresh;
//	int maxval;
//	int cThreshold;
//	int ratio;
//	int kernel_size;
//	double rho;
//	double theta;
//	int lThreshold;
//	double minLL;
//	double maxLG;
//}Config;
//
////Global Variables
//Mat nSrc; //Will hold the original image.
//Mat nDst; //Will hold the thresholded image.
//Mat nDetected_edges; //The output of the Canny detector.
//Mat nHlines; //Lines found by the Hough Transform.
//
//Config nConfig;
//string nInput = "image.raw"; //Input filename.
////Dimensions of the RAW image.
//int nImageSizeX = 1392;
//int nImageSizeY = 550;
//
///**
// * Constructor that loads the image and prepares for processing.
// */
//CourtLineDetector::CourtLineDetector(void)
//{
//	this->load_raw();
//	this->cld_setup();
//}
//
//
//CourtLineDetector::~CourtLineDetector(void)
//{
//}
//
//int CourtLineDetector::run()
//{
//	this->thresh_canny_line();
//
//	#if NAME_LINES
//	this->name_lines();
//	#endif
//	
//	#if DRAW_AXES
//	this->draw_axes();
//	#endif
//	
//	this->write_raw();
//	return 0;
//}
//
///**
// * Creates matrices of the same type as nSrc and setups
// * the configuration matrix for thresholding, Canny and Houghs
// * line transform. Along with setting up the values for the above
// * operations.
// */
//void CourtLineDetector::cld_setup()
//{
//	Size src_size = nSrc.size();
//	int src_type = nSrc.type();
//	//Create the matrices.
//	nDst.create(src_size, src_type);
//	nDetected_edges.create(src_size, src_type);
//	nHlines.create(src_size, src_type);
//	nHlines = Scalar::all(0); //Fills the matrix with 0, creating a black background.
//
//	//Setup up config struct.
//	nConfig.thresh = 128; //Threshold for thresholding.
//	nConfig.maxval = 255; //Value for thresholding.
//	nConfig.cThreshold = 40; //Canny hysteresis threshold.
//	nConfig.ratio = 3; //Canny recommends uppper:lower ratio between 2:1 and 3:1.
//	nConfig.kernel_size = 3; //Canny kernel size.
//	nConfig.rho = 1; //Distance resolution of the accumulator in pixels.
//	nConfig.theta = CV_PI/180; //Angle resolution of the accumulator in pixels.
//	nConfig.lThreshold = 10; //Minimum number of intersections to detect a line.
//	nConfig.minLL = 10; //Minimum length of the line.
//	nConfig.maxLG = 5; //Maximum allowed gap between points on the same line to link them.
//}
//
///**
// * Draws a line from the given coordinates in the vector.
// * @param l An int vector of size 4 that contains two points.
// */
//void draw_lines(Vec4i l)
//{
//	//Seperate the points into (x1,y1) and (x2,y2).
//	int x1 = l[0], y1 = l[1];
//	int x2 = l[2], y2 = l[3];
//	int thickness = 1; //Thickness of the line to be drawn.
//	Scalar colour = Scalar(255, 255, 255); //White
//	line(nHlines, Point(x1, y1), Point(x2, y2), colour, thickness, CV_AA);
//}
//
///**
// * Performs, in their respective order, thresholding, Canny and 
// * Probabilistic Hough transform, based on the settings from
// * nConfig. If the SHOW_INTERMEDIATES flag is set, windows will
// * open showing the result of each operation; press any key to
// * continue.
// */
//void CourtLineDetector::thresh_canny_line()
//{
//	int thresh = nConfig.thresh;
//	int maxval = nConfig.maxval;
//	int cThreshold = nConfig.cThreshold;
//	int ratio = nConfig.ratio;
//	int kernel_size = nConfig.kernel_size;
//	double rho = nConfig.rho;
//	double theta = nConfig.theta;
//	int lThreshold = nConfig.lThreshold;
//	double minLL = nConfig.minLL;
//	double maxLG = nConfig.maxLG;
//
//
//	//Thresholding
//	dilate(nSrc, nDst, Mat()); //Dilate the image.
//	threshold(nDst, nDst, thresh, maxval, THRESH_BINARY);
//	#if SHOW_INTERMEDIATES
//	imshow("Threshold", nDst);
//	waitKey();
//	#endif
//
//	//Canny edge detector
//	Canny(nDst, nDetected_edges, cThreshold, cThreshold * ratio, kernel_size, true);
//	#if SHOW_INTERMEDIATES
//	imshow("Canny", nDetected_edges);
//	waitKey();
//	#endif
//
//	//Probabilistic Hough line transform
//	vector<Vec4i> lines; //Vector of lines of type int.
//	HoughLinesP(nDetected_edges, lines, rho, theta, lThreshold, minLL, maxLG);
//	std::for_each(lines.begin(), lines.end(), draw_lines);
//	#if SHOW_INTERMEDIATES
//	imshow("Lines", nHlines);
//	waitKey();
//	#endif
//}
//
///**
// * Names the lines in the nHlines matrix.
// */
//void CourtLineDetector::name_lines()
//{
//	//Name horizontal lines.
//	vector<string> horizLines;
//	horizLines.push_back("Baseline");
//	horizLines.push_back("Service Line");
//	Scalar colour = Scalar(255, 255, 255); //White
//	int skip = 0; //to ensure we're not on the same line
//	for(int i = 0; i < nHlines.rows && !horizLines.empty(); ++i)
//	{
//		double val = norm(nHlines.row(i)/255, NORM_L1);
//		switch(val > 227 && !skip)
//		{
//		case 1:
//			putText(nHlines, horizLines.back(), Point(nHlines.cols/4, i+13), FONT_HERSHEY_PLAIN, 1, colour);
//			skip = 1;
//			horizLines.pop_back(); //Pop the last word out.
//			break;
//		default:
//			if(val < 227 && skip)
//				skip = 0;
//			break;
//		}
//	}
//
//	//Name vertical lines.
//	vector<string> vertLines;
//	vertLines.push_back("Doubles Sideline");
//	vertLines.push_back("Singles Sideline");
//	vertLines.push_back("Center Service Line");
//	vertLines.push_back("Singles Sideline");
//	vertLines.push_back("Doubles Sideline");
//
//	//Hardcoded locations next to the lines.
//	vector<Point> locations;
//	locations.push_back(Point(10, nHlines.rows *0.25));
//	locations.push_back(Point(170, nHlines.rows *0.25));
//	locations.push_back(Point(nHlines.cols/4 + 100, nHlines.rows *0.25));
//	locations.push_back(Point(nHlines.cols* 0.75 + 50, nHlines.rows *0.25));
//	locations.push_back(Point(1250, nHlines.rows *0.25));
//	for(int i = 0; i < vertLines.size(); ++i)
//		putText(nHlines, vertLines[i], locations[i], FONT_HERSHEY_PLAIN, 1, colour);
//
//	#if SHOW_INTERMEDIATES
//	imshow("Named Lines", nHlines);
//	waitKey();
//	#endif
//}
//
///**
// * Draws X and Y axes on the image and intervals them.
// */
//void CourtLineDetector::draw_axes()
//{
//	//Mid points of the image.
//	int midx = nHlines.cols/2 - 50; //50 is subtracted to make the numbers and line more visible.
//	int midy = nHlines.rows/2;
//	Scalar colour = Scalar(255,255,255); //White.
//	int thickness = 1; //Line and Text thickness
//
//	//Creates x-axis.
//	line(nHlines, Point(0, midy), Point(nHlines.cols, midy), colour, thickness, CV_AA);
//	for(int i = 0; i < nHlines.cols; ++i)
//	{
//		//Divide the line every 172 pixels.
//		if(!(i%172))
//		{
//			//Draw a vertical line at twice the thickness.
//			line(nHlines, Point(i, midy-5), Point(i, midy+5), colour, 2*thickness, CV_AA);
//			//Place the number above the line.
//			std::ostringstream num;
//			num<<i;
//			putText(nHlines, num.str(), Point(i, midy-10), FONT_HERSHEY_PLAIN, thickness, colour);
//		}
//		else //Divide the line every 43 pixels at regular thickness.
//			if(!(i%43))
//				line(nHlines, Point(i, midy-5), Point(i, midy+5), colour, thickness, CV_AA);
//	}
//
//	//Creates y-axis.
//	line(nHlines, Point(midx, 0), Point(midx, nHlines.rows), colour, thickness, CV_AA);
//	for(int i = 0; i < nHlines.rows; ++i)
//	{
//		//Divide the line every 55 pixels.
//		if(!(i%55))
//		{
//			//Draw a horizontal line at twice the thickness.
//			line(nHlines, Point(midx-5, i), Point(midx+5, i), colour, 2*thickness, CV_AA);
//			//Place the number above the line.
//			std::ostringstream num;
//			num<<i;
//			putText(nHlines, num.str(), Point(midx+5, i), FONT_HERSHEY_PLAIN, thickness, colour);
//		}
//		else //Divide the line every 43 pixels at regular thickness.
//			if(!(i%11))
//				line(nHlines, Point(midx-5, i), Point(midx+5, i), colour, thickness, CV_AA);
//	}
//
//	#if SHOW_INTERMEDIATES
//	imshow("Axes", nHlines);
//	waitKey();
//	#endif
//}