#include "ImageUtil.h"

#include <cmath>


/**
 * @brief Converts a 8-bit grayscale image to a double grayscale image.
 * @param im 8-bit grayscale image.
 * @return Returns a matrix of type double representing the image.
 */
Matd BYTE2double(const Matb& im)
{
	//Get the sizes.
	int rows = im.size(), cols = im[0].size();
	//Create a new BYTE matrix for conversion.
	Matd im2(rows, Vecd(cols));
	
	//Convert from intensity value to double value.
	for(int i = 0; i < rows; ++i)
		for(int j = 0; j < cols; ++j)
			im2[i][j] = im[i][j]/255.0;

	return im2;
}

/**
 * @brief Converts a double grayscale image to a 8-bit grayscale image.
 * @param im double grayscale image.
 * @return Returns an 8-bit matrix of the image.
 */
Matb double2BYTE(const Matd& im)
{
	//Get sizes
	int rows = im.size(), cols = im[0].size();
	//Create a new vector for conversion.
	Matb im2(rows, Vecb(cols));

	//Convert from double to intensity value.
	for(int i = 0; i < rows; ++i)
		for(int j = 0; j < cols; ++j)
			im2[i][j] = im[i][j]*255;

	return im2;
}

/**
 * @brief Creates a 2D Gaussian kernel with the given sigma and size.
 * @param kernel_size The size of the kernel/matrix.
 * @param sigma The sigma value needed for the gaussian distribution.
 * @return Returns a matrix of type double.
 */
Matd gaussKernel(int kernel_size, double sigma)
{
	double sum = 0.0; //For normalization.
	double r, s = 2.0 * sigma * sigma;
	int neg_end = -kernel_size/2, pos_end = -neg_end;

	//Create kernel
	Matd kernel(kernel_size, Vecd(kernel_size));

	for(int i = neg_end; i <= pos_end; ++i)
	{
		for(int j = neg_end; j <= pos_end; ++j)
		{
			r = i*i+j*j; // x^2+y^2
			kernel[i+pos_end][j+pos_end] = exp(-r/s)/(M_PI * s);
			sum += kernel[i+pos_end][j+pos_end];
		}
	}

	//Normalize the kernel.
	for(int i = 0; i < kernel_size; ++i)
		for(int j = 0; j < kernel_size; ++j)
			kernel[i][j] /=sum;

	return kernel;
}

/**
 * @brief Performs a gaussian blur on the image using the given kernel.
 * @param kernel The gaussian kernel matrix.
 * @param image The grayscale image.
 * @return A matrix of the blurred image.
 */
Matd gaussian_blur(const Matd& kernel, const Matd& image)
{
	//Get the offset of the kernel from its center.
	int kernel_size = kernel.size(), kernel_offset = kernel_size/2, rows = image.size(), cols = image[0].size();
	Matd blurred(rows, Vecd(cols));
	//Pad the image with zeros.
	Matd padded(rows + kernel_offset*2, Vecd(cols + kernel_offset*2));

	//Copy the image to the padded vector.
	for(int i = 0; i < rows; ++i)
		for(int j = 0; j < cols; ++j)
			padded[i+kernel_offset][j+kernel_offset] = image[i][j];

	//Perform Gaussian Blur.
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < cols; ++j)
		{
			double sum = 0.0;
			//Convolution of the kernel and inner matrix of the image.
			for(int k = 0; k < kernel_size; ++k)
			{
				for(int z = 0; z < kernel_size; ++z)
				{
					sum += kernel[k][z]*padded[i+k][j+z];
				}
			}
			blurred[i][j] = sum;
		}
	}

	return blurred;
}

/**
 * @brief Performs a Sobel edge detector using the given GX and GY masks.
 * @param GX matrix of the horizontal Sobel mask.
 * @param GY matrix of the vertical Sobel mask.
 * @return Returns a matrix of the detected edges.
 */
Matd sobel_edge(const Mati& GX, const Mati& GY, const Matd& image, int t1, int t2)
{
	//Get the offset of Sobel mask from its center.
	int sobel_size = GX.size(), sobel_offset = sobel_size/2, rows = image.size(), cols = image[0].size();
	//Create a new matrix.
	Matd edges(rows, Vecd(cols));

	//Avoid the edges for Sobel.
	for(int i = sobel_offset; i < rows - sobel_offset; ++i)
	{
		for(int j = sobel_offset; j < cols - sobel_offset; ++j)
		{
			double gX = 0.0;
			double gY = 0.0;
			double g = 0.0;
			//Calculate X and Y convolutions.
			for(int k = -sobel_offset; k <= sobel_offset; ++k)
			{
				for(int z = -sobel_offset; z <= sobel_offset; ++z)
				{
					gX += GX[k+sobel_offset][z+sobel_offset]*image[i+k][j+z];
					gY += GY[k+sobel_offset][z+sobel_offset]*image[i+k][j+z];
				}
			}
			g = sqrt(gX*gX + gY*gY);
			edges[i][j] = g;
		}
	}

	//Perform a hysteresis to obtain a binary image.
	Matd nedges(rows, Vecd(cols));

	for(int i = 0; i < rows; ++i)
	{
		double hyst = t1/255.0;
		for(int j = 0; j < cols; ++j)
		{
			if(edges[i][j] > hyst)
			{
				nedges[i][j] = 1.0;
				if(t1 == hyst)
					hyst = t2/255.0;
			}
			else
				nedges[i][j] = 0;
		}
	}

	return nedges;
}

/**
 * @brief Detects lines based on the mask.
 * @param mask 2D vector of the line mask.
 * @param image 2D vector of the binary image.
 * @return Returns a 2D image of the detected lines.
 */
Matd line_detect(const Mati& mask, const Matd& image)
{
	//Get the size of image and the mask offset
	int mask_offset = mask.size()/2, rows = image.size(), cols = image[0].size();
	//Create a matrix for the detected lines.
	Matd lines(rows, Vecd(cols));

	for(int i = mask_offset; i < rows - mask_offset; ++i)
	{
		for(int j = mask_offset; j < cols - mask_offset; ++j)
		{
			double sum = 0.0;
			//Calculate X and Y convolutions.
			for(int k = -mask_offset; k <= mask_offset; ++k)
			{
				for(int z = -mask_offset; z <= mask_offset; ++z)
				{
					sum += mask[k+mask_offset][z+mask_offset]*image[i+k][j+z];
				}
			}
			lines[i][j] = sum;
		}
	}

	return lines;
}

/**
 * @brief Rounds a number.
 * @param val The value to be rounded.
 * @return Returns the rounded value.
 */
static inline double round(double val)
{
	return floor(val+0.5);
}

/**
 * @brief Creates the accumulator for houghs.
 * @param image The binary image.
 * @return Returns the accumulator (r, theta).
 */
Mati hough_transform(const Matd& image)
{
	//Get the size of the image and it's center.
	int rows = image.size(), cols = image[0].size();
	double centerX = cols/2, centerY = rows/2;

	//Create the accumulator.
	double hough_h = (sqrt(2.0) * (rows>cols?rows:cols))/2.0;
	double accu_h = hough_h * 2.0; //Max rho.
	int accu_w = 180; //The angle.
	
	Mati accu(accu_h, Veci(accu_w));

	//Vote for lines and store them inside the accumulator.
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < cols; ++j)
		{
			if(image[i][j] != 0)
			{
				for(int theta = 0; theta < 180; ++theta)
				{
					double rads = theta * (180.0/M_PI);
					double r =  (j - centerX)*cos(rads) + (i - centerY)*sin(rads);
					accu[(int) round(r + hough_h)][theta]++;
				}
			}
		}
	}

	return accu;
}

/**
 * @brief Using Besenham's draw algorithm, it draws a line between two points.
 * @param x1 The x coordinate of the first point.
 * @param y1 The y coordinate of the first point.
 * @param x2 The x coordinate of the second point.
 * @param y2 The y coordinate of the second point.
 */
void besenham(int x1, int y1, int x2, int y2, Matd& image)
{
	int deltax = x2 - x1;
	int deltay = y2 - y1;
	double error = 0.0;
	double deltaerr = abs(deltay/deltax);
	int y = y1;
	for(int x = x1; x < x2; ++x)
	{
		image[y][x] = 1.0;
		error += deltaerr;
		if(error >=0.5)
		{
			y++;
			error -= 1.0;
		}
	}
}

/**
 * @brief Using the accumulator it calculates (x1,y1) and (x2,y2) and draws them.
 * @param rows The X dimension of the original image.
 * @param cols The Y dimension of the original image.
 * @param threshold The number of intersections to be considered a line.
 * @param accu The accumulator from the hough_transform.
 * @return An image of the lines drawn.
 */
Matd hough_lines(int rows, int cols, int threshold, const Mati& accu)
{
	int accu_h = accu.size(), accu_w = accu[0].size();
	double rows_half = rows/2, cols_half = cols/2;
	Matd nlines(rows, Vecd(cols)); //Image to be drawn on.
	Points lines; //Holds the pair of points.

	for(int r = 0; r < accu_h; ++r)
	{
		for(int theta = 0; theta < accu_w; ++theta)
		{
			if((int) accu[r][theta] >= threshold)
			{
				//Is this point a local maxima? Check the 9x9 neighbourhood.
				int max = accu[r][theta];
				for(int y = -4; y <= 4; ++y)
				{
					for(int x = -4; x <= 4; ++x)
					{
						double yr = y+r;
						int xt = x+theta;
						//Are yr and xt withtin the bounds?
						if((yr >= 0 && yr < accu_h) && (xt >= 0 && xt < accu_w))
						{
							if((int) accu[yr][xt] > max)
							{
								max = accu[yr][xt];
								//Break out of the neighbourhood check.
								y = x = 5;
							}
						}
					}
				}

				if(max > (int) accu[r][theta])
					continue;

				int x1, y1, x2, y2;
				x1 = y1 = x2 = y2 = 0;
				double rads = theta * (180.0/M_PI);
				double r_ac = r-accu_h/2;

				//Calculate the points.
				if(theta >= 45 && theta <= 135)
				{
					//y = (r - x*cos(theta)) / sin(theta)
					x1 = 0;
					y1 = (r_ac - (x1 - cols_half) * cos(rads)) / sin(rads) + rows_half;
					x2 = cols - 1;
					y2 = (r_ac - (x2 - cols_half) * cos(rads)) / sin(rads) + rows_half;
				}
				else
				{
					//x = (r - y sin(theta)) / cos(theta)
					y1 = 0;
					x1 = (r_ac - (y1 - rows_half) * sin(rads)) / cos(rads) + cols_half;
					y2 = rows - 1;
					x2 = (r_ac - (y2 - rows_half) * sin(rads)) / cos(rads) + cols_half;
				}

				//Push them as pairs.
				lines.push_back(Coords(Coord(x1,y1), Coord(x2,y2)));
				//Draw the line between the two points in the image.
				besenham(x1, y1, x2, y2, nlines);
			}
		}
	}

	return nlines;
}

/**
 * @brief Performs a threshold on the given image.
 * @param thresh The threshold value.
 * @param image 2D vector of the image.
 * @return Returns a 2D thresholded image.
 */
Matb threshold(int thresh, const Matb& image)
{
	int rows = image.size(), cols = image[0].size();
	//Create the threshhold vector.
	Matb image_thresh(rows, Vecb(cols));

	for(int i = 0, ilen = image.size(); i < ilen; ++i)
	{
		for(int j = 0, jlen = image[0].size(); j < jlen; ++j)
		{
			if(image[i][j] >= thresh)
				image_thresh[i][j] = 255;
			else
				image_thresh[i][j] = 0;
		}
	}

	return image_thresh;
}

/**
 * @brief Performs a threshold on the given image.
 * @param thresh The threshold value.
 * @param image 2D vector of the image.
 * @return Returns a 2D thresholded image.
 */
Matd threshold(int thresh, const Matd& image)
{
	//Convert the intensity value to a double value.
	double d_thresh = thresh/255.0;
	int rows = image.size(), cols = image[0].size();
	//Create the threshhold vector.
	Matd image_thresh(rows, Vecd(cols));

	for(int i = 0, ilen = image.size(); i < ilen; ++i)
	{
		for(int j = 0, jlen = image[0].size(); j < jlen; ++j)
		{
			if(image[i][j] >= d_thresh)
				image_thresh[i][j] = 1.0;
			else
				image_thresh[i][j] = 0.0;
		}
	}

	return image_thresh;
}