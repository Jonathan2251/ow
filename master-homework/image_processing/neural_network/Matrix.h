#pragma warning(disable:4786)
#include<iostream>
#include<vector>

using namespace std;

template <class T> class Matrix { // ¤Gºû¯x°}
public:
	vector<vector<T> > v;
	class error { };
	int row() const { return v.size(); };
	int col() const { 
		if (v.size() > 0)
			return v[0].size();
		else return 0;
	};
	vector<T>& operator[](const int i) { return v[i]; };
	void free() {
		v.resize(0);
	};
	void init(const int aRow, const int aCol) {
		if (row() != aRow || col() != aCol) {
			v.resize(aRow);
			for (int i = 0; i < row(); i++) v[i].resize(aCol);
		}
		for (int i = 0; i < row(); i++) {
			for (int j = 0; j < col(); j++) {
				v[i][j] = 0;
			}
		}
	};
	void copy(Matrix& A) {
		if (row() == A.row() && col() == A.col()) {
			for (int i = 0; i < row(); i++) {
				for (int j = 0; j < col(); j++) {
					v[i][j] = A[i][j];
				}
			}
		}
		else throw error();
	};
	Matrix() { v.resize(0); };
	Matrix(int aRow, int aCol) {
		init(aRow, aCol);
	};
	Matrix(Matrix& A) {
		init(A.row(), A.col());
		copy(A);
	};
	Matrix& operator+(Matrix& A) { 
		static Matrix B;
		B.init(row(), col());
		if (row() == A.row() && col() == A.col()) {
			for (int i = 0; i < row(); i++) {
				for (int j = 0; j < col(); j++) {
					B[i][j] = (*this)[i][j] + A[i][j];
				}
			}
		}
		else throw error();
		return B;
	};
	Matrix& operator-(Matrix& A) {
		static Matrix B;
		B.init(row(), col());
		if (row() == A.row() && col() == A.col()) {
			for (int i = 0; i < row(); i++) {
				for (int j = 0; j < col(); j++) {
					B[i][j] = (*this)[i][j] - A[i][j];
				}
			}
		}
		else throw error();
		return B;
	};
	void operator+=(Matrix& A) { 
		if (row() == A.row() && col() == A.col()) {
			for (int i = 0; i < row(); i++) {
				for (int j = 0; j < col(); j++) {
					(*this)[i][j] += A[i][j];
				}
			}
		}
		else throw error();
	};
	Matrix& transpose() { 
		static Matrix B;
		if (B.row() != col() || B.col() != row()) {
			B.free();
			B.init(col(), row());
		}
		for (int i = 0; i < row(); i++) {
			for (int j = 0; j < col(); j++) {
				B[j][i] = v[i][j];
			}
		}
		return B;
	};
	Matrix& operator*(const double scaler) { 
		static Matrix B;
		if (B.row() != row() || B.col() != col()) {
			B.free();
			B.init(row(), col());
		}
		for (int i = 0; i < row(); i++) {
			for (int j = 0; j < col(); j++) {
				B[i][j] = (*this)[i][j] * scaler;
			}
		}
		return B;
	};
	Matrix& operator*(Matrix& A) { 
		static Matrix B;
		if (B.row() != row() || B.col() != A.col()) {
			B.free();
			B.init(row(), A.col());
		}
		for (int i = 0; i < B.row(); i++) {
			for (int j = 0; j < B.col(); j++) {
				B[i][j] = 0;
				for (int k = 0; k < col(); k++) {
					B[i][j] += (*this)[i][k]*A[k][j];
				}
			}
		}
		return B;
	};
	void operator=(Matrix& A) {
		if (row() != A.row() || col() != A.col()) {
			free();
			init(A.row(), A.col());
		}
		copy(A);
	};
	friend void operator<<(ostream& os, Matrix& A) {
		for (int i = 0; i < A.row(); i++) {
			for (int j = 0; j < A.col(); j++) {
				os << A[i][j] << " ";
			}
			os << endl;
		}
	};
};