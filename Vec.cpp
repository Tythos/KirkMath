/*
 * Vec.cpp
 */

#include "KirkMath.hpp"

// --- Constructors --- //
KirkMath::Vec::Vec() {
	len = 0;
	vec = NULL;
}

KirkMath::Vec::Vec(int length) {
	len = length;
	vec = new float[len];
}

KirkMath::Vec::Vec(int length, float vals[]) {
	len = length;
	vec = new float[len];
	for (int i = 0; i < length; i++) {
		vec[i] = vals[i];
	}
}

KirkMath::Vec::Vec(KirkMath::Vec const& b) {
	len = b.len;
	vec = new float[len];
	for (int i = 0; i < len; i++) {
		vec[i] = b.vec[i];
	}
}

KirkMath::Vec::~Vec() {
	delete vec;
}

// --- Access --- //
void KirkMath::Vec::set(int length, float vals[]) {
	if (vec != NULL) delete vec;
	len = length;
	vec = new float[len];
	for (int i = 0; i < len; i++) {
		vec[i] = vals[i];
	}
}

// --- Math --- //
float KirkMath::Vec::abs() {
	float toRet = 0.0;
	for (int i = 0; i < len; i++) {
		toRet += vec[i] * vec[i];
	}
	return sqrt(toRet);
}

// --- Operators --- //
std::ostream& KirkMath::operator<<(std::ostream& os, const KirkMath::Vec &myVec) {
	int i = 0;
	os << "[";
	for (i = 0; i < myVec.len-1; i++) {		
		os << myVec.vec[i] << ", ";
	}
	if (myVec.len > 0) os << myVec.vec[myVec.len-1];
	os << "]";
	return os;
}

std::ostream& KirkMath::operator<<(std::ostream &os, KirkMath::Vec *myVec) {
	os << (*myVec);
	return os;
}

float& KirkMath::Vec::operator[](int i) {
	if (i < 0) {
		std::cout << "Exception 1 in operator[]: invalid index\n";
		throw 1;
	}
	i = len > 0 ? i % len : i;
	return vec[i];
}

float KirkMath::Vec::operator *(KirkMath::Vec b) { // Dot product
	float toRet = 0.0;
	int minLen = this->len < b.len ? this->len : b.len;
	for (int i = 0; i < minLen; i++) {
		toRet += this->vec[i] * b.vec[i];
	}
	return toRet;
}

KirkMath::Vec KirkMath::Vec::operator %(KirkMath::Vec b) {  // Cross product
	if (this->len != b.len) {
		std::cout << "Exception 2 in operator %: incompatible dimensions\n";
		throw 2;
	}
	const int l = len;
	float * newVec = new float[l];
	for (int i = 0; i < len; i++) {
		newVec[i] = vec[i+1] * b[i+2] - b[i+1] * vec[i+2];
	}
	KirkMath::Vec* toRet = new KirkMath::Vec(len, newVec);
	return (*toRet);
}

KirkMath::Vec KirkMath::Vec::operator +(KirkMath::Vec b) {
	if (this->len != b.len) {
		std::cout << "Exception 2 in operator +: incompatible dimensions\n";
		throw 2;
	}
	const int l = len;
	float * newVec = new float[len];
	for (int i = 0; i < len; i++) {
		newVec[i] = vec[i] + b[i];
	}
	KirkMath::Vec* toRet = new KirkMath::Vec(len, newVec);
	return (*toRet);
}

KirkMath::Vec KirkMath::Vec::operator -(KirkMath::Vec b) {
	return (*this) + (b * -1);
}

KirkMath::Vec KirkMath::Vec::operator *(float b) {
	const int l = len;
	float * newVec = new float[l];
	for (int i = 0; i < len; i++) {
		newVec[i] = vec[i] * b;
	}
	KirkMath::Vec* toRet = new KirkMath::Vec(len, newVec);
	return (*toRet);
}

KirkMath::Vec KirkMath::Vec::operator /(float b) {
	return (*this) * (1 / b);
}

KirkMath::Vec KirkMath::Vec::operator =(KirkMath::Vec b) {
	if (vec != NULL) delete vec;
	len = b.len;
	vec = new float[len];
	for (int i = 0; i < len; i++) {
		vec[i] = b[i];
	}
	return (*this);
}
