#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <unordered_map>
#include <string>
using namespace std;

#ifndef MX
#define MX

template <typename T>
class Matrix
{
	protected:
		int row;
		int col;
		vector<vector<T>> matrix;
	public:
		Matrix(){}
		
		Matrix(const int r)
		{
			row = r;
			col = r;
			matrix.resize(r);
			for (int i(0); i < r; ++i)
			{
				matrix[i].resize(r);
				matrix[i][i] = 1;
			}
		}
		
		Matrix(const int r, const int c)
		{
			row = r;
			col = c;
			matrix.resize(r);
			for (int i(0); i < r; ++i)
			{
				matrix[i].resize(c);
			}
		}
		
		Matrix(const vector<vector<T>>& v)
		{
			row = v.size();
			col = v[0].size();
			matrix.resize(row);
			for (int i(0); i < row; ++i)
			{
				matrix[i].assign(v[i].begin(), v[i].begin());	
			}
		}

		Matrix(const vector<vector<T>> && v)
		{
			row = v.size();
			col = v[0].size();
			matrix.resize(row);
			for (int i(0); i < row; ++i)
			{
				matrix[i] = move(v[i]);
			}
		}
		
		Matrix(const Matrix<T> & m)
		{
			//matrix.assign(m.matrix.begin(), m,matrix.end()); dose not work!!
			row = m.get_row();
			col = m.get_col();
			matrix.resize(row);
			for (int i(0); i < row; ++i)
			{
				matrix[i].assign(m.matrix[i].begin(), m.matrix[i].end());
			}
		}
		
		Matrix(Matrix<T> && m)
		{
			row = move(m.row);
			col = move(m.col);
			//matrix = move(m.matrix);
			matrix.resize(row);
			for (int i(0); i < row; ++i)
			{
				matrix[i] = move(m.matrix[i]);
			}
		}
		
		Matrix<T>& operator=(const Matrix<T> & m)
		{
			row = m.row;
			col = m.col;
			{
				vector<vector<T>> temp;
				matrix = move(temp);
			}
			matrix.resize(row);
			for (int i(0); i < row; ++i)
			{
				matrix[i].assign(m.matrix[i].begin(), m.matrix[i].end());
			}
			return *this;
		}
		
		Matrix<T>& operator=(const Matrix<T> && m)
		{
			row = move(m.row);
			col = move(m.col);
			{
				vector<vector<T>> temp;
				matrix = move(temp);
			}
			matrix.resize(row);
			for (int i(0); i < row; ++i)
			{
				matrix[i] = move(m.matrix[i]);
			}
			return *this;
		}
		
		Matrix<T>& operator=(const vector<vector<T>> & v)
		{
			row = v.size();
			col = v[0].size();
			{
				vector<vector<T>> temp;
				matrix = move(temp);
			}
			matrix.resize(row);
			for (int i(0); i < row; ++i)
			{
				matrix[i].assign(v[i].begin(), v[i].end());
			}
			return *this;
		}
		
		Matrix<T>& operator=(const vector<vector<T>> && v)
		{
			row = v.size();
			col = v[0].size();
			{
				vector<vector<T>> temp;
				matrix = move(temp);
			}
			matrix.resize(row);
			for (int i(0); i < row; ++i)
			{
				matrix[i] = move(v[i]);
			}
			return *this;
		}
		
		Matrix<T> operator+(const Matrix<T> & m)
		{
			if (row == m.row && col == m.col)
			{
				Matrix<T> temp(row, col);
				for (int i(0); i < row; ++i)
				{
					for (int j(0); j < col; ++j)
					{
						temp.matrix[i][j] = matrix[i][j] + m.matrix[i][j];
					}
				}
				return temp;
			}
			else
			{
				Matrix<T> temp;
				cerr << "The dimension of the two matrixes does not fit." << endl;
				return temp;
			}
		}
		
		Matrix<T> operator-(const Matrix<T> & m)
		{
			if (row == m.row && col == m.col)
			{
				Matrix<T> temp(row, col);
				for (int i(0); i < row; ++i)
				{
					for (int j(0); j < col; ++j)
					{
						temp.matrix[i][j] = matrix[i][j] - m.matrix[i][j];
					}
				}
				return temp;
			}
			else
			{
				Matrix<T> temp;
				cerr << "The dimension of the two matrixes does not fit." << endl;
				return temp;
			}
		}
		
		void multiple(Matrix<T>* t, const Matrix<T> h, const int i)
		{
			for (int j(0); j < h.col; ++j)
			{
				for (int all(0); all < h.row; ++all)
				{
					t->matrix[i][j] += matrix[i][all] * h.matrix[all][j];
				}
			}
		}
		
		Matrix<T> operator*(const Matrix<T> & m)
		{
			if (col == m.row)
			{
				Matrix<T> temp(row, m.col);	
				Matrix<T>* t = &temp;
				int idx_final(0);
				for (int i(0); i < row - 1; ++i)
				{
					thread TH(&Matrix::multiple, this, t, ref(m), i);
					TH.detach();
				}
				thread HH(&Matrix::multiple, this, t, ref(m), row - 1);
				HH.join();
				return temp;
			}
			else
			{
				Matrix<T> temp;
				cerr << "The dimension of the two matrixes dose not fit." << endl;
				return temp;
			}
		}
		
		Matrix<T>& operator*(const T a)
		{
			for (int i(0); i < row; ++i)
			{
				for (int j(0); j < col; ++j)
				{
					matrix[i][j] *= a;
				}
			}
			return *this;
		}
		
		Matrix<T>& transpose()
		{
			Matrix<T> temp(col, row);
			for (int i(0); i < row; ++i)
			{
				for (int j(0); j < col; ++j)
				{
					temp.matrix[j][i] = matrix[i][j];
				}
			}
			*this = move(temp);
			return *this;
		}
		
		Matrix<double>& inverse()
		{
			if (row != col)
			{
				cerr << "The matrix is not a square matrix." << endl;
				return *this;
			}
			else
			{
				Matrix<double> identity(row, col);
				for (int i(0); i < row; ++i)
				{
					identity.matrix[i][i] = 1;
				}
				//forward
				for (int i(0); i < col; ++i)
				{
					for (int j(i + 1); j < row; ++j)
					{
						double rate = -(matrix[j][i] / matrix[i][i]);
						for (int k(0); k < row; ++k)
						{
							matrix[j][k] += matrix[i][k] * rate;
							identity.matrix[j][k] += identity.matrix[i][k] * rate;
						}
					}
				}
				//backward
				for (int i(row - 1); i >= 0; --i)
				{
					for (int j(i - 1); j >= 0; --j)
					{
						double rate = -(matrix[j][i] / matrix[i][i]);
						for (int k(0); k < row; ++k)
						{
							matrix[j][k] += matrix[i][k] * rate;
							identity.matrix[j][k] += identity.matrix[i][k] * rate;
						}
					}
				}
				//divide
				for (int i(0); i < row; ++i)
				{
					double pivot = matrix[i][i];
					for (int k(0); k < row; ++k)
					{
						identity.matrix[i][k] /= pivot;			
					}
				}
				*this = move(identity);
				return *this;
			}
		}
		
		T get_value(const int r, const int c)
		{
			if (r > row - 1 || c > col - 1)
			{
				cerr << "Position [" << r << "]" << "[" << c << "]" << " is out off range!!" << endl;
				return 0;
			}
			else
			{
				return matrix[r][c]; 
			}
		}	
		
		void change_value(const int r, const int c, const T n)
		{
			if (r > row - 1 || c > col - 1)
			{
				cerr << "Position [" << r << "]" << "[" << c << "]" << " is out off range!!" << endl;
			}
			else
			{
				matrix[r][c] = n; 
			}		
		}
		
		int get_row() const {return row;}
		int get_col() const {return col;}
		
		void printout(ostream& output)
		{
			for (int i(0); i < row; ++i)
			{
				for (int j(0); j < col; ++j)
				{
					output << matrix[i][j] << " ";
				}
				output << endl;
			}
		}
		
		~Matrix(){}
};


void build_matrix(Matrix<double>& A, Matrix<double>& b, unordered_map<double, vector<double>>& data, int basis)
{
	vector<vector<double>> x;
	vector<vector<double>> y;
	x.resize(data.size());
	y.resize(data.size());
	int idx(0);
	for (auto& p : data)
	{
		double temp(1.0);
		x[idx].resize(basis + 1);
		for (int i(0); i < p.second.size(); ++i)
		{
			for (int j(basis); j >= 0 ; --j)
			{
				x[idx][j] = temp;
				temp *= p.second[i];
			}
		}
		
		y[idx].push_back(p.first);		
		++idx;	
	}
	A = move(x);
	b = move(y);
}

void read_data(istream& input, unordered_map<double, vector<double>>& data)
{
	string s;
	while(getline(input, s))
	{
		vector<double> temp;
		int pos(0);
		while(s.find(',', pos) != string::npos)
		{
			int f = s.find(',', pos);
			string num = s.substr(pos, f - pos);
			pos = f + 1;
			temp.push_back(stod(num));
		}
		string last_num = s.substr(s.find_last_of(',') + 1);
		data.insert(make_pair(stod(last_num), temp));
	}
}

Matrix<double> fit_regre_with_regu(Matrix<double>& A, Matrix<double>& b, double lambda)
{
	Matrix<double> temp(A);
	A.transpose();
	Matrix<double> identity(A.get_row()); 
	return ((((A * temp) + (identity * lambda)).inverse()) * A) * b;
}

#endif
