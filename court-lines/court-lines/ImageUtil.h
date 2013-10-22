#pragma once
#define _USE_MATH_DEFINES // for C++
#include "VecMatTypes.h"

Matd BYTE2double(const Matb&);
Matb double2BYTE(const Matd&);
Matd gaussKernel(int, double);
Matd gaussian_blur(const Matd&, const Matd&);
Matd sobel_edge(const Mati&, const Mati&, const Matd&, int, int);
Matd line_detect(const Mati&, const Matd&);
Mati hough_transform(const Matd&);
Matd hough_lines(int, int, int, const Mati&);
Matb threshold(int, const Matb&);
Matd threshold(int, const Matd&);