#include <iostream>
#include <vector>
#include <cmath>
#include "Random_data_generator.hpp"
#include "mx.hpp"
using namespace std;

class data
{
	public:
	double feature1;
	double feature2;
	data(){}
	data(double&& x, double&& y)
	{
		feature1 = x;
		feature2 = y;
	}
};
	
ostream& operator<<(ostream& out, const data& d)
{
	out << "(" << d.feature1 << ", " << d.feature2 << ")";
	return out;
}

double logistic(double&& a)
{
	return 1 / (1 + exp(-a));
}

Matrix<double>& logisticM(Matrix<double>&& a)
{
	for (int r = 0; r < a.get_row(); ++r)
	{
		double y = logistic(a.get_value(r, 0));
		a.change_value(r, 0, y);	
	}
	return a;
}

Matrix<double> diagonal_R(Matrix<double>& w, Matrix<double>& phi)
{
	Matrix<double> temp = phi * w.transpose(); // n x 1 = n x 3 * 3 x 1
	//cout << "temp:\n";
	//temp.printout(cout);
	Matrix<double> R(temp.get_row(), temp.get_row());
	for (int r = 0; r < temp.get_row(); ++r)
	{
		double y = logistic(temp.get_value(r, 0));
		y = y * (1 - y);
		//cout << "logistic: " << y << endl;
		R.change_value(r, r, y);
	}
	//R.printout(cout);
	return R; 
}

Matrix<double> gradient(Matrix<double>& w, Matrix<double>& phi, Matrix<double>& t)
{
	
	return phi.transpose() * (logisticM(phi * w.transpose()) - t);	
}

Matrix<double> hessian(Matrix<double>& w, Matrix<double>& phi)
{
	return phi.transpose() * diagonal_R(w, phi) * phi;
}

Matrix<double> result(Matrix<double>& w, Matrix<double>& phi)
{
	Matrix<double> temp = phi * w.transpose();
	for (int r = 0; r < temp.get_row(); ++r)
	{
		double y = logistic(temp.get_value(r, 0));
		temp.change_value(r, 0, y);
	}
	return temp;
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
		temp.push_back(0);
		temp_target.push_back(temp);
		temp.clear();
		double x = d1[i].feature1;
		double y = d1[i].feature2;
		temp.push_back(1);
		for (int basis = 1; basis < basis_num; ++basis)
		{
			temp.push_back(pow(x, basis));
			temp.push_back(pow(y, basis));
		}
		temp_phi.push_back(temp);
	}
	
	for (int i = 0; i < d2.size(); ++i)
	{
		vector<double> temp;
		temp.push_back(1);
		temp_target.push_back(temp);
		temp.clear();
		double x = d2[i].feature1;
		double y = d2[i].feature2;
		temp.push_back(1);
		for (int basis = 1; basis < basis_num; ++basis)
		{
			temp.push_back(pow(x, basis));
			temp.push_back(pow(y, basis));
		}
		temp_phi.push_back(temp);
	}

	phi = move(temp_phi);
	target = move(temp_target);
}

void learn_w(Matrix<double>& w, Matrix<double>& phi, Matrix<double>& t)
{
	int i = 0;
	
	// w = [w0, w1, w2, ..., wn]   //
	// t = [t0, t1, t2, ..., tn]^T //

	/**************************************
	|									  |
	|		 1 x0 x0^2 ... y0 y0^2 ...    |
	| 		 1 x1 x1^2 ... y1 y1^2 ...    |
	|   	 1 x2 x2^2 ... y2 y2^2 ...    |
	| phi =	 .							  |
	|		 .							  |
	|		 .							  |
	|		 1 xn xn^2 ... yn yn^2 ...    |
	|									  |
	**************************************/

	/**************** learning (training) process ********************
	|  													             |		
	|  update our linear model --> update "W"                        |
	|  Wn+1 = Wn - gradient(Wn) or                                   |
	|  Wn+1 = Wn - (hessian(Wn))^-1 * gradient(Wn)                   |
	| 													             |
	|  gradient(Wn) = phi^T * (logistic(phi * w^T) - t)              |
	|  hessian(Wn) = phi^T * R * phi								 |	
	|  R is a diagonal matrix										 |
	|  R ==> Rii = logistic(phi * w^T)i * (1 - logistic(phi * w^T)i) | 
	|																 |	
	*****************************************************************/
	
	while(1)
	{
		cout << "======== ITERATION " << i << " ===========" << endl;
		Matrix<double> H = hessian(w, phi); 
		Matrix<double> G = gradient(w, phi, t);
		double val = 0;
		for (int i = 0; i < G.get_row(); ++i)
		{
			val += G.get_value(i, 0);
		}
		
		//cout << "det of H: ";
		//cout << H.det() << endl;

		if (pow(H.det(), 2) <= 0.0001)
		{
			w = w.transpose() - G;
			w = w.transpose();
		}
		else
		{
			w = w.transpose() - H.inverse() * G;
			w = w.transpose();
		}
		
		w.printout(cout);

		if (pow(val, 2) <= 0.00000001)
		{
			break;
		}

		i++;
	}
}

int main()
{
	vector<data> dataset1;
	vector<data> dataset2;
	int size1 = 10;
	int size2 = 12;
	generate_dataset(dataset1, 5.0, 5.0, 30.0, 10.0, size1);
	generate_dataset(dataset2, 50.0, 15.0, 20.0, 20.0, size2);
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
	build_matrix(phi, target, dataset1, dataset2, 2);
	
	cout << "************ PHI ************" << endl;
	phi.printout(cout);
	cout << "************ TARGET ************" << endl;
	target.printout(cout);
	Matrix<double> w;
	vector<double> temp{1, 1, 1};

	w.append(temp);
	diagonal_R(w, phi);
	cout << "************ GRADIENT ************\n";
	gradient(w, phi, target).printout(cout);
	learn_w(w, phi, target);
	
	/*************** test set *****************/
	
	vector<data> testset1;
	vector<data> testset2;
	size1 = 1;
	size2 = 10;
	generate_dataset(testset1, 5.0, 5.0, 30.0, 10.0, size1);
	generate_dataset(testset2, 50.0, 15.0, 20.0, 20.0, size2);

	Matrix<double> phi_t;
	Matrix<double> target_t;
	build_matrix(phi_t, target_t, testset1, testset2, 2);
	
	Matrix<double> s = logisticM(phi_t * w.transpose()) - target_t;
	s.printout(cout);
	return 0;
}
