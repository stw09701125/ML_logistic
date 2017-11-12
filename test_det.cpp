#include <iostream>
#include <vector>
#include "mx.hpp"

using namespace std;

int main()
{
	Matrix<int> M;
	vector<int> v0{1, 2, 3};
	vector<int> v1{1, 1, 2};
	vector<int> v2{2, 3, 4};
	M.append(v0);
	M.printout(cout);
	M.append(v1);
	M.printout(cout);
	M.append(v2);
	M.printout(cout);
	cout << M.det() << endl;
	return 0;
}
