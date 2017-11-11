#include <iostream>
#include <random>
#include <cmath>
using namespace std;


double RDG(double mean, double var)
{
	random_device rd;
	mt19937 gen(rd());
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

void sequential_estimate(const double& real_mean, const double& real_var, double mean, double var, int n = 2)
{
	double data = RDG(real_mean, real_var);
	double new_mean = mean + (data - mean) / n;
	double new_var = (n - 2) * var / (n - 1) + pow((data - mean), 2) / n;
	cout << "===========================" << endl;
	cout << "The " << n << "th time: " << endl;
	cout << "Data point: " << data << endl;
	cout << "Mean: " << new_mean << endl;
	cout << "Var: " << new_var << endl;
	if (pow((new_mean - mean), 2) < 0.0001 && pow((new_var - var), 2) < 0.000001)
	{
		return;
	}
	else
	{
		sequential_estimate(real_mean, real_var, new_mean, new_var, n + 1);
	}
}

int main()
{
	vector<double> weight {12.0, 3.0, 2.0};	
	cout << "Generated by univariate guassian data generator: " << RDG(10.0, 5.0) << endl;
	cout << "Generated by polynomial basis linear model data generator: " << linear_model_RDG(3, 3.0, weight) << endl;
	
	double first_data = RDG(10.0, 5.0);
	cout << "===========================" << endl;
	cout << "The first time: " << endl;
	cout << "Data point: " << first_data << endl;
	cout << "Mean: 10" << endl;
	cout << "Var: 5" << endl;
	sequential_estimate(10.0, 5.0, first_data, 0, 2);
	return 0;
}