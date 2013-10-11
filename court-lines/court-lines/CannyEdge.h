#pragma once

class CannyEdge
{
public:
	CannyEdge(int, double);
	virtual ~CannyEdge(void);
	void print_kernel();
	double* getKernel();
private:
	double* createKernel(int, double);
	//Variables
	double* nKernel;
	int nKernel_size;
	double nSigma;
};

