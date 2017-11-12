#include <iostream>
#include <vector>
#include <cmath>
#include "Random_data_generator.hpp"
#include "mx.hpp"
using namespace std;

class data
{
	public:
	double feature;
	double target;
	data(){}
	data(double&& x, double&& y)
	{
		feature = x;
		target = y;
	}
};
	
ostream& operator<<(ostream& out, const data& d)
{
	out << "(" << d.feature << ", " << d.target << ")";
	return out;
}

void generate_dataset(vector<data>& D, double x_mean, double x_var, double y_mean, double y_var, int size)
{
	for (int i = 0; i < size; ++i)
	{
		data temp(RDG(x_mean, x_var), RDG(y_mean, y_var));
		D.emplace_back(move(temp));
	}
}

void build_matrix(Matrix<double>& phi, Matrix<double>& target, vector<data> d1, vector<data> d2, int basis_num)
{	
	vector<vector<double>> temp_phi;
	vector<vector<double>> temp_target;
	for (int i = 0; i < d1.size(); ++i)
	{
		vector<double> temp;
		temp.push_back(d1[i].target);
		temp_target.push_back(temp);
		temp.clear();
		for (int basis = 0; basis < basis_num; ++basis)
		{
			double x = d1[i].feature;
			temp.push_back(pow(x, basis));
		}
		temp_phi.push_back(temp);
	}
	
	for (int i = 0; i < d2.size(); ++i)
	{
		vector<double> temp;
		temp.push_back(d2[i].target);
		temp_target.push_back(temp);
		temp.clear();
		for (int basis = 0; basis < basis_num; ++basis)
		{
			double x = d2[i].feature;
			temp.push_back(pow(x, basis));
		}
		temp_phi.push_back(temp);
	}

	phi = temp_phi;
	target = temp_target;
}

int main()
{
	vector<data> dataset1;
	vector<data> dataset2;
	generate_dataset(dataset1, 5.0, 5.0, 30.0, 10.0, 5);
	generate_dataset(dataset2, 50.0, 15.0, 20.0, 20.0, 5);
	cout << "dataset1: ";
	for (auto& i : dataset1)
	{
		cout << i << " ";
	}
	cout << endl;
	cout << "dataset2: ";
	for (auto& i : dataset2)
	{
		cout << i << " ";
	}
	cout << endl;
	
	Matrix<double> phi;
	Matrix<double> target;
	build_matrix(phi, target, dataset1, dataset2, 3);
	
	cout << "************ PHI ************" << endl;
	phi.printout(cout);
	cout << "************ TARGET ************" << endl;
	target.printout(cout);
	return 0;
}
