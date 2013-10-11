#include "CannyEdge.h"

#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <iostream>


CannyEdge::CannyEdge(int kerne_size, double sigma)
{
	nKernel_size = kerne_size;
	nSigma = sigma;
	nKernel = createKernel(nKernel_size, nSigma);
}


CannyEdge::~CannyEdge(void)
{
	delete[] nKernel;
}

double* CannyEdge::createKernel(int kernel_size, double sigma)
{
	double sum = 0; //For normalization.
	double r, s = 2 * sigma * sigma;
	int neg_end = -kernel_size/2, pos_end = -neg_end;

	//Create kernel
	double* kernel = new double[kernel_size*kernel_size];

	for(int i = neg_end; i <= pos_end; ++i)
	{
		for(int j = neg_end; j <= pos_end; ++j)
		{
			int position = (i+2)*kernel_size + (j+2);
			r = i*i+j*j; // x^2+y^2
			kernel[position] = exp(-r/s)/(M_PI * s);
			sum += kernel[position];
		}
	}

	//Normalize the kernel.
	for(int i = 0; i < kernel_size; ++i)
		for(int j = 0; j < kernel_size; ++j)
			kernel[i*kernel_size + j] /=sum;

	return kernel;
}

void CannyEdge::print_kernel()
{
	for(int i = 0; i < nKernel_size; ++i)
    {
    	for(int j = 0; j < nKernel_size; ++j)
        {
			std::cout<<nKernel[i*nKernel_size+j]<<"\t";
        }
        std::cout<<std::endl;
    }
}

double* CannyEdge::getKernel()
{
	return nKernel;
}