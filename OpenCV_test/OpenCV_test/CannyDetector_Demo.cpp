/**
 * @file CannyDetector_Demo.cpp
 * @brief Sample code showing how to detect edges using the Canny Detector
 * @author OpenCV team
 */

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


using namespace cv;

//Global Variables
Mat src, dst, detected_edges, hLines;

void load_raw()
{
	/// Load an image
	char* buffer = new char[1392*550];
	FILE* fp = fopen("image.raw", "rb");
	if(fp)
	{
		fread(buffer, 1392*550, 1, fp);
		fclose(fp);
	}
	else
	{
		delete buffer;
		fclose(fp);
		exit(1);
	}

	/*
	 * The first three and last two columns are just solid white and blacks
	 * which are unnecessary.
	 */
	src = Mat(550, 1392, CV_8UC1, buffer).clone().colRange(4, 1389).clone();
	
	delete buffer;

	if(!src.data)
		exit(1);
}

void write_raw()
{
	//Create buffer
	char* buffer = new char[hLines.rows * hLines.cols];
	
	//Load image data
	MatIterator_<char> it = hLines.begin<char>(), itEnd = hLines.end<char>();
	for(int i = 0; it !=itEnd; ++it, ++i)
		buffer[i] = *it;
	//Write out the image data
	std::ostringstream filename;
	filename<<"court_lines "<<hLines.cols<<"x"<<hLines.rows<<".raw";
	FILE *fp = fopen(filename.str().c_str(), "wb");

	//Check if it opened
	if(fp)
	{
		fwrite(buffer, hLines.rows * hLines.cols, 1, fp);
		fclose(fp);
	}
	else
	{
		std::cerr<<"File failed to open!\n";
		delete[] buffer;
		exit(1);
	}

	//Delete the buffer;
	delete [] buffer;
}

struct myclass
{
	bool operator() (Vec4i a, Vec4i b)
	{
		return a[1] < b[1];//(norm(a, 2) < norm(b,2));
	}
}mySort;

struct myclass2
{
	bool operator() (double a, double b)
	{
		return a > b;
	}
}mySort2;

/**
 * @function main
 */
int main( int, char** argv )
{
  load_raw();

  //Create matrices of the same type as src
  dst.create(src.size(), src.type());
  detected_edges.create(src.size(), src.type());
  hLines.create(src.size(), src.type());

  //Apply thresholding
  dilate(src, src, Mat());
  threshold(src, src, 128, 255, THRESH_BINARY);
  /*imshow("Threshold image", src);
  waitKey();*/

  //Apply Canny edge detector
  //dilate(src, src, Mat());
  Canny(src, detected_edges, 40, 120, 3, true);
  imshow("Canny Edges", detected_edges);
  //Wait until user presses a key to continue
  waitKey();

  //Setup dst and hLines
  dst = detected_edges.clone();
  cvtColor(dst, dst, CV_GRAY2BGR); //Convert dst to a colour matrix
  hLines = Scalar::all(0); //Fills the matrix with 0 creating a black background
  
  //dilate(detected_edges, detected_edges, Mat());
  
  //Detect lines using Houghs transform
  vector<Vec4i> lines;
  HoughLinesP(detected_edges, lines, 1, CV_PI/180, 10, 10, 5 );
  std::sort(lines.begin(), lines.end(), mySort);
  for( size_t i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    line( dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);
	line( hLines, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 1, CV_AA);

	//if(i+1 < lines.size())
	//{
	//int diffy = abs(l[3] - lines[i+1][1]);
	//int diffx = abs(l[2] - lines[i+1][0]);
	//int thickness = 1;
	//if(diffx < 25)
	//switch(diffy < 5)
	//{
	//case 0:
	//	line( hLines, Point(l[2], l[3]), Point(lines[i+1][0], lines[i+1][1]), Scalar(255,255,255), thickness, CV_AA);
	//	break;
	///*case 1:
	//	line( hLines, Point(l[2], l[3]), Point(lines[i+1][0], lines[i+1][1]), Scalar(255,255,255), thickness, CV_AA);
	//	break;
	//case 2:
	//	line( hLines, Point(l[2], l[3]), Point(lines[i+1][0], lines[i+1][1]), Scalar(255,255,255), thickness, CV_AA);
	//	break;
	//case 3:
	//	line( hLines, Point(l[2], l[3]), Point(lines[i+1][0], lines[i+1][1]), Scalar(255,255,255), thickness, CV_AA);
	//	break;*/
	//default:
	//	break;
	//}
	//}
	/*Vec2f xl, yl, mag, angle;
	xl[0] = l[0];
	xl[1] = l[2];
	yl[0] = l[1];
	yl[1] = l[3];
	cartToPolar(xl, yl, mag, angle);
	std::cout<<"X = "<<xl<<" Y = "<<yl<<" Angle = "<<atan2(angle[1], angle[0]) * 180/CV_PI<<" Magnitude = "<<sqrt(pow(mag[0],2)+pow(mag[1],2))<<std::endl;
	imshow("Just lines", hLines);
	waitKey();*/
  }

  //Not necessary as it makes no noticeable changes to the image.
  //morphologyEx(hLines, hLines, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(3,3)));
  //dilate(hLines, hLines, Mat());

 imshow("detected lines", dst);
 //putText(hLines, "TESTING", Point(1392/2, 550/2), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
 imshow("Just lines", hLines);
 waitKey();
 //Scan for vertical lines and mark them.

 //Scan for horizontal lines and mark them.
 vector<string> horizLines;
 horizLines.push_back("Baseline");
 horizLines.push_back("Service Line");

 vector<double> vals;
 int skip = 0;
 for(int i = 0; i < hLines.rows && !horizLines.empty(); ++i)
 {
	 double val = norm(hLines.row(i)/255, NORM_L1);
	 vals.push_back(val);
	 if(val > 227 && !skip)
	 {
		 putText(hLines, horizLines.back(), Point(hLines.cols/4, i+13), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
		 imshow("Row Lines", hLines);
		 waitKey();
		 skip = 1;
		 horizLines.pop_back();
	 }
	 else
		 if(val < 227 && skip)
			 skip = 0;
 }
 std::sort(vals.begin(), vals.end(), mySort2);

 vector<string> vertLines;
 vertLines.push_back("Doubles Sideline");
 vertLines.push_back("Singles Sideline");
 vertLines.push_back("Center Service Line");
 vertLines.push_back("Singles Sideline");
 vertLines.push_back("Doubles Sideline");

 vector<Point> locations;
 locations.push_back(Point(10, hLines.rows *0.25));
 locations.push_back(Point(170, hLines.rows *0.25));
 locations.push_back(Point(hLines.cols/4+100, hLines.rows *0.25));
 locations.push_back(Point(hLines.cols* 0.75 + 50, hLines.rows *0.25));
 locations.push_back(Point(1250, hLines.rows *0.25));

 for(int i = 0; i < vertLines.size(); ++i)
	putText(hLines, vertLines[i], locations[i], FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
 //vector<double> valv;
 //vector<Point> locV;
 //int skip = 0;
 //for(int i = 0; i < hLines.cols && !vertLines.empty(); ++i)
 //{
	// double val = norm(hLines.col(i)/255, NORM_L1);
	// valv.push_back(val);
	// if(val > 29 && !skip)
	// {
	//	 locV.push_back(Point(i, hLines.rows/4));
	//	 //std::cout<<i<<std::endl;
	//	 /*putText(hLines, vertLines.back(), Point(i, hLines.rows/4), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	//	 imshow("Col Lines", hLines);
	//	 waitKey();*/
	//	 skip = 1;
	//	 //vertLines.pop_back();
	// }
	// else
	//	 if(val < 29 && skip)
	//		 skip = 0;
 //}

 //for(int i = 0; i < locV.size() && !vertLines.empty(); ++i)
 //{
	// putText(hLines, vertLines.back(), locV[i], FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	// vertLines.pop_back();
	// imshow("Col Lines", hLines);
	// waitKey();
 //}
 //std::sort(valv.begin(), valv.end(), mySort2);
 
 //Scan vertically for a straight white line and mark it.
 //int block = 277;
 //for(int i = 0; i < hLines.cols/block; ++i)
 //{
	// MatIterator_<char> it = hLines.colRange(i, block*(i + 1)).begin<char>(), itend = hLines.col(i).end<char>();
	// int n = 0, k = hLines.rows;
	// Mat copyL = hLines.colRange(i, block*(i + 1)).clone();
	// for(;it != itend; ++it)
	// {
	//	 imshow("Column", copyL.rowRange(n++,k));
	//	 waitKey();
	// }
	// imshow("Column", hLines.col(i));
	// waitKey();
 //}

 //int skip = 0;
 ////Scan vertically for a straight white Line and mark it.
 //vector<string> vertLines;
 //vertLines.push_back("Doubles Sideline");
 //vertLines.push_back("Singles Sideline");
 //vertLines.push_back("Center Service Line");
 //vertLines.push_back("Singles Sideline");
 //for(int i = 0; i < hLines.cols; ++i)
 //{
	// double val = norm(hLines.col(i)/255, NORM_L1);
	// //std::cout<<"Value = "<<val<<" at Column "<<i<<std::endl;
	// if(val > 30 && !skip)
	// {
	//	 //std::cout<<i<<std::endl;
	//	 putText(hLines, vertLines.back(), Point(i, hLines.rows/4), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	//	 imshow("Col Lines", hLines);
	//	 waitKey();
	//	 skip = 1;
	//	 vertLines.pop_back();
	// }
	// else
	//	 if(val < 29 && skip)
	//		 skip = 0;
 //}

 ////Scan horizontally for a straight white line and mark it
 //vector<string> horizLines;
 //horizLines.push_back("Baseline");
 //horizLines.push_back("Service Line");
 //for(int i = 0; i < hLines.rows; ++i)
 //{
	// double val = norm(hLines.row(i)/255, NORM_L1);
	// if(val > 500 && !skip)
	// {
	//	 putText(hLines, horizLines.back(), Point(hLines.cols/2, i), FONT_HERSHEY_PLAIN, 3, Scalar(255,255,255));
	//	 imshow("Row Lines", hLines);
	//	 waitKey();
	//	 skip = 1;
	//	 horizLines.pop_back();
	// }
	// else
	//	 if(val < 500 && skip)
	//		 skip = 0;
 //}

 //Pixel counting

 line( hLines, Point(0, hLines.rows/2), Point(hLines.cols, hLines.rows/2), Scalar(255,255,255), 1, CV_AA);
 int midy = hLines.rows/2;
 for(int i = 0; i < hLines.cols; ++i)
 {
		 if(!(i%172))
		 {
			 line( hLines, Point(i, midy-5), Point(i, midy+5), Scalar(255,255,255), 2, CV_AA);
			 std::ostringstream num;
			 num<<i;
			 putText(hLines, num.str(), Point(i, midy-10), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
		 }
		 else
		 {
			 if(!(i%43))
			 {
				line( hLines, Point(i, midy-5), Point(i, midy+5), Scalar(255,255,255), 1, CV_AA);
			 }

		 }
 }
 line( hLines, Point(hLines.cols/2-50, 0), Point(hLines.cols/2-50, hLines.rows), Scalar(255,255,255), 1, CV_AA);
 int midx = hLines.cols/2 - 50;
 for(int i = 0; i < hLines.cols; ++i)
 {
	 if(!(i%55))
	 {
		 line( hLines, Point(midx-5, i), Point(midx+5, i), Scalar(255,255,255), 2, CV_AA);
		 std::ostringstream num;
		 num<<i;
		 putText(hLines, num.str(), Point(midx+5, i), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	 }
	 else
	 {
		 if(!(i%11))
		 {
			 line( hLines, Point(midx-5, i), Point(midx+5, i), Scalar(255,255,255), 1, CV_AA);
		 }
	 }
 }
  write_raw();
  imshow("Complete", hLines);
  waitKey();
  return 0;
}
