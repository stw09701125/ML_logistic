#include <iostream>
#include <vector>
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

void generate_dataset(Matrix<data>& D, double x_mean, double x_var, double y_mean, double y_var, int size)
{
	vector<data> dataset;
	for (int i = 0; i < size; ++i)
	{
		data temp(RDG(x_mean, x_var), RDG(y_mean, y_var));
		dataset.emplace_back(move(temp));
	}
	D = move(dataset);
}

int main()
{
	Matrix<data> dataset1;
	generate_dataset(dataset1, 10.0, 5.0, 30.0, 10.0, 5);
	dataset1.printout(cout);
	return 0;
}
