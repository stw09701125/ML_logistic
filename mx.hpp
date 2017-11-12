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
		vector< vector<T> > matrix;
	public:
		Matrix()
		{
			row = 0;
			col = 0;
		}
		
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
		
		Matrix(const vector< vector<T> >& v)
		{
			row = v.size();
			col = v[0].size();
			matrix.resize(row);
			for (int i(0); i < row; ++i)
			{
				matrix[i].assign(v[i].begin(), v[i].begin());	
			}
		}

		Matrix(const vector< vector<T> > && v)
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
		
		Matrix<T>& operator=(const vector< vector<T> > & v)
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
		
		Matrix<T>& operator=(const vector< vector<T> > && v)
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

		Matrix<T>& operator=(const vector<T> & v)
		{
			row = 1;
			col = v.size();
			{
				vector<vector<T>> temp;
				matrix = move(temp);
			}
			matrix.resize(1);
			matrix[0].assign(v.begin(), v.end());
			return *this;
		}
		
		Matrix<T>& operator=(const vector<T> && v)
		{
			row = 1;
			col = v.size();
			{
				vector<vector<T>> temp;
				matrix = move(temp);
			}
			matrix.resize(1);
			matrix[0] = move(v);
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
		
		T det()
		{
			if (row == 2)
			{
				return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
			}

			T val = 0;

			for (int r = 0; r < row; ++r)
			{
				Matrix<T> cofactor;
				for (int i = 0; i < row; ++i)
				{
					if (i != r)
					{
						vector<T> temp;
						for(int c = 1; c < col; ++c)
						{
							temp.push_back(matrix[i][c]);
						}
						cofactor.append(temp);
					}
				}
				// r % 2 == 0 ---> "+" else ----> "-"
				val += ((r & 1) ? -1 : 1) * cofactor.det() * matrix[r][0];
			}

			return val;
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

		void append(vector<T>& v)
		{
			row += 1;
			if (v.size() != col && col != 0)
			{
				cerr << "size of vec does not fit matrix colunm" << endl;
			}
			else { col = v.size(); }
			matrix.emplace_back(move(v));	
		}
		
		int get_row() const { return row; }
		int get_col() const { return col; }
		
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

#endif
