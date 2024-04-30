/*
 * Mat.cpp
 */

#include "KirkMath.hpp"

// --- Constructors --- //
KirkMath::Mat::Mat():
    mat(NULL),
    r(0),
    c(0) {
}

KirkMath::Mat::Mat(KirkMath::Mat const& b) {
	r = b.r; c = b.c;
	mat = new KirkMath::Vec[r];
	for (int i = 0; i < r; i++) {
		mat[i] = b.mat[i];
	}
}

KirkMath::Mat::Mat(KirkMath::Vec b) {
	// Creates a 1 x n matrix from given vector
	r = 1; c = b.getLength();
	mat = new KirkMath::Vec[1];
	mat[0] = b;
}

KirkMath::Mat::Mat(int size) {
	r = size; c = size;
	mat = new KirkMath::Vec[r];
	const int cc = c;
	float * tmpRow = new float[cc];
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (i == j) tmpRow[j] = 1.0f;
			else tmpRow[j] = 0.0f;
		}
		mat[i] = KirkMath::Vec(c, tmpRow);
	}
	delete[] tmpRow;
}

KirkMath::Mat::Mat(int rows, int cols) {
	r = rows; c = cols;
	mat = new KirkMath::Vec[r];
	const int cc = c;
	float * tmpRow = new float[cc];
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (i == j) tmpRow[j] = 1.0f;
			else tmpRow[j] = 0.0f;
		}
		mat[i] = KirkMath::Vec(c, tmpRow);
	}
	delete[] tmpRow;
}

KirkMath::Mat::Mat(int rows, int cols, float * vals) {
	r = rows; c = cols;
	mat = new KirkMath::Vec[r];
	float * tmpRow = new float[c];
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			tmpRow[j] = vals[i*c+j];
		}
		mat[i] = KirkMath::Vec(c, tmpRow);
	}
	delete[] tmpRow;
}

KirkMath::Mat::~Mat() {
	if (mat != NULL) delete [] mat;
}

// --- Access --- //
void KirkMath::Mat::set(int rows, int cols, float * vals=NULL) {
	r = rows; c = cols;
	if (mat != NULL) delete mat;
	mat = new KirkMath::Vec[r];
	const int cc = c;
	float * tmpRow = new float[cc];
	if (vals != NULL) {
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				tmpRow[j] = vals[i*c+j];
			}
			mat[i] = KirkMath::Vec(c, tmpRow);
		}
	} else {
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				tmpRow[j] = (i == j) ? 1.0f : 0.0f;
			}
			mat[i] = KirkMath::Vec(c, tmpRow);
		}
	}
}

KirkMath::Vec KirkMath::Mat::toVec() {
	// If matrix dimensions are 1 x n or n x 1, returns the equivalent vector
	if (r == 1) {
		KirkMath::Vec* toRet = new KirkMath::Vec(c);
		for (int i = 0; i < c; i++) {
			(*toRet)[i] = (*this)[1][i];
		}
		return (*toRet);
	} else if (c == 1) {
		KirkMath::Vec* toRet = new KirkMath::Vec(r);
		for (int i = 0; i < r; i++) {
			(*toRet)[i] = (*this)[i][1];
		}
		return (*toRet);
	} else {
		std::cout << "Exception 5 in kMat::toVec: 1d matrix required\n";
		throw 5;
	}
}

KirkMath::Vec KirkMath::Mat::getRow(int row) {
	return mat[row];
}

KirkMath::Vec KirkMath::Mat::getCol(int col) {
	KirkMath::Vec* toRet = new KirkMath::Vec(r);
	for (int i = 0; i < r; i++) {
		(*toRet)[i] = mat[i][col];
	}
	return (*toRet);
}

// --- Math --- //
float KirkMath::Mat::norm() {
	// return sqrt(max eig of A_H * A)
	return 0.0f;
}

float KirkMath::Mat::trace() {
	float toRet = 0.0f;
	if (r != c) {
		std::cout << "Exception 3 in kMat::trace: square matrix required\n" << std::endl;
		throw 3;
	} else {
		for (int i = 0; i < r; i++) {
			toRet += (mat[i])[i];
		}
	}
	return toRet;
}

KirkMath::Mat KirkMath::Mat::I() {
	if (r != c) {
		std::cout << "Exception 3 in kMat::identity: square matrix required\n";
		throw 3;
	}
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			mat[i][j] = i == j ? 1.0f : 0.0f;
		}
	}
	return (*this);
}

KirkMath::Mat KirkMath::Mat::transpose() {
	const int cc = r * c;
	float * myVals = new float[cc];
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < r; j++) {
			myVals[i*r+j] = mat[j][i];
		}
	}
	KirkMath::Mat* toRet = new KirkMath::Mat(c, r, myVals);
	return (*toRet);
}

KirkMath::Mat KirkMath::Mat::invert() {
	float d = det();
	if (d*d < SMALL_FLOAT*SMALL_FLOAT) {
		std::cout << "Exception 6 in kMat::invert: insufficient rank for operation\n";
		throw 6;
	}
	return (*this);
}

float KirkMath::Mat::det() {
	if (r != c) {
		std::cout << "Exception 3 in kMat::operator^: square matrix required\n";
		throw 3;
	}
	float toRet = 0.0f;
	if (r == 2) {
		toRet = (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
		return toRet;
	}
	KirkMath::Mat* subMat = new KirkMath::Mat(r-1, c-1);
	for (int j = 0; j < c; j++) {
		for (int si = 1; si < r; si++) {
			for (int sj = j+1; sj < j+c; sj++) {
				(*subMat)[si-1][sj-j-1] = (*this)[si][sj];
			}
		}
		toRet += mat[0][j] * subMat->det();
	}
	return toRet;
}

// --- Operators ---//

std::ostream& KirkMath::operator<<(std::ostream &os, const KirkMath::Mat& myMat) {
	os << std::endl;
	for (int i = 0; i < myMat.r; i++) {
		if (i == 0 || i == myMat.r - 1) os << "+";
		else os << "|";
		os << myMat.mat[i];
		if (i == 0 || i == myMat.r - 1) os << "+";
		else os << "|" << std::endl;
		if (i == 0) os << std::endl;
	}
	return os;
}

std::ostream& KirkMath::operator<<(std::ostream &os, KirkMath::Mat *myMat) {
	os << (*myMat);
	return os;
}

KirkMath::Vec& KirkMath::Mat::operator[](int i) {
	if (i < 0) {
		std::cout << "Exception 1 in kMat::operator[]: invalid index\n";
		throw 1;
	}
	if (r == 0 || c == 0 || mat == NULL) {
		std::cout << "Exception 4 in kMat::operator[]: matrix not initialized\n";
		throw 4;
	}
	i = i % r;
	return mat[i];
}

KirkMath::Vec KirkMath::Mat::operator *(KirkMath::Vec b) {
	if (b.getLength() != c) {
		std::cout << "Exception 2 in kMat::*(kVec): incompatible dimensions\n";
		throw 2;
	}
	KirkMath::Mat* eqMat = new KirkMath::Mat(b);
	return ((*this) * eqMat->transpose()).toVec();
}

KirkMath::Mat KirkMath::Mat::operator *(KirkMath::Mat b) {
	if (c != b.r) {
		std::cout << "Exception 2 in kMat::*(kMat): incompatible dimensions\n";
		throw 2;
	}
	KirkMath::Mat * toRet = new KirkMath::Mat(r, b.c);
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < b.c; j++) {
			(*toRet)[i][j] = mat[i] * b.getCol(i);
		}
	}
	return (*toRet);
}

KirkMath::Mat KirkMath::Mat::operator *(float b) {
	KirkMath::Mat * toRet = new KirkMath::Mat(r, c);
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			(*toRet)[i][j] = mat[i][j] * b;
		}
	}
	return (*toRet);
}

KirkMath::Mat KirkMath::Mat::operator +(KirkMath::Mat b) {
	if (c != b.c || r != b.r) {
		std::cout << "Exception 2 in kMat::+(kMat): incompatible dimensions\n";
		throw 2;
	}
	KirkMath::Mat* toRet = new KirkMath::Mat(r,c);
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			(*toRet)[i][j] = mat[i][j] + b[i][j];
		}
	}
	return (*toRet);
}

KirkMath::Mat KirkMath::Mat::operator -(KirkMath::Mat b) {
	return (*this) + (b * -1);
}

KirkMath::Mat KirkMath::Mat::operator /(float b) {
	return (*this) * (1 / b);
}

KirkMath::Mat KirkMath::Mat::operator =(KirkMath::Mat b) {
	if (mat != NULL) delete [] mat;
	r = b.r; c = b.c;
	mat = new KirkMath::Vec[r];
	for (int i = 0; i < r; i++) {
		mat[i] = b.mat[i];
	}
	return (*this);
}

KirkMath::Mat KirkMath::Mat::operator ^(int b) {
	if (c != r) {
		std::cout << "Exception 3 in kMat::operator^: square matrix required\n";
		throw 3;
	}
	KirkMath::Mat* toRet = new KirkMath::Mat(r, c);
	if (b == 0) return toRet->I();
	(*toRet) = (*this);
	for (int i = 1; i < abs(b); i++) {
		(*toRet) = (*toRet) * (*this);
	}
	if (b < 0) (*toRet) = toRet->invert();
	return (*toRet);
}
