#include <iostream>
#include <random>
#include <cmath>
using namespace std;


#ifndef RG
#define RG

double RDG(double mean, double var)
{
	random_device rd;
	mt19937 gen(rd());
	/************ Box-Muller method ************/
	/* use two random variable u0, u1
	 * that are uniformly distributed in the interval (0, 1) 
	 * z0 = sqrt(-2 * log(u0)) * cos(2 * M_PI * u1)
	 * z1 = sqrt(-2 * log(u0)) * sin(2 * M_PI * u1)
	 * z0 and z1 are independent random variable with a standard normal distribution
	 * z0 ~ N(0, 1)		z1 ~ N(0, 1)
	 * any normal distribution N(mean, var) = N(0, 1) * sqrt(var) + mean
	 */
	uniform_real_distribution<> dis(0.0, 1.0);
	double u0, u1;
	u0 = dis(gen);
	u1 = dis(gen);
	double z = sqrt(-2 * log(u0)) * cos(2 * M_PI * u1);	
	return z * sqrt(var) + mean;
}

double linear_model_RDG(int basis_num, double var_of_bias, vector<double> weight)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dis(-10.0, 10.0);
	
	double x = dis(gen);
	double y = 0;
	if (weight.size() != basis_num)
	{
		cerr << "weight vector size dose not fit basis number" << endl;
	}
	/**************************************************************
	|															  |
	| y = [w0, w1, w2, ..., wn] * [x^0, x, x^2, x^3, ..., x^n]^T  |
	|															  |	
	**************************************************************/
	else
	{
		for (int i = 0; i < basis_num; ++i)
		{
			y += weight[i] * pow(x, i);
		}
	}
	/****** y = W^T * Phi(x) + bias ******/
	/* where bias is from guassian N(0, var) */
	y += RDG(0, var_of_bias);
	
	return y;
}


#endif

