/*
 * Quat.cpp
 */

#include "KirkMath.hpp"

KirkMath::Quat::Quat() {
	scl = 1.0f;
	vec[0] = 0.0f;
	vec[1] = 0.0f;
	vec[2] = 0.0f;
}

KirkMath::Quat::Quat(const KirkMath::Quat &q) {
	scl = q.scl;
	vec[0] = q.vec[0];
	vec[1] = q.vec[1];
	vec[2] = q.vec[2];
}

KirkMath::Quat::Quat(float x, float y, float z) {
	scl = 1.0f;
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

KirkMath::Quat::Quat(float s, float i, float j, float k) {
	/*scl = s;
	vec[0] = i;
	vec[1] = j;
	vec[2] = k;*/
}

KirkMath::Quat::~Quat() {
}

KirkMath::Quat KirkMath::Quat::operator+ (KirkMath::Quat operand) {
	KirkMath::Quat toReturn;
	toReturn.scl = scl + operand.scl;
	toReturn.vec[0] = vec[0] + operand.vec[0];
	toReturn.vec[1] = vec[1] + operand.vec[1];
	toReturn.vec[2] = vec[2] + operand.vec[2];
	return toReturn;
}

KirkMath::Quat KirkMath::Quat::operator- (KirkMath::Quat operand) {
	return (*this) + (operand * -1);
}

KirkMath::Quat KirkMath::Quat::operator* (float operand) {
	KirkMath::Quat toReturn;
	toReturn.scl = scl * operand;
	toReturn.vec[0] = vec[0] * operand;
	toReturn.vec[1] = vec[1] * operand;
	toReturn.vec[2] = vec[2] * operand;
	return toReturn;
}

KirkMath::Quat KirkMath::Quat::operator* (KirkMath::Quat operand) {
	KirkMath::Quat toReturn;
	toReturn.scl = scl*operand.scl - vec[0]*operand.vec[0] - vec[1]*operand.vec[1] - vec[2]*operand.vec[2];
	toReturn.vec[0] = scl*operand.vec[0] + vec[0]*operand.scl + vec[1]*operand.vec[2] - vec[2]*operand.vec[1];
	toReturn.vec[1] = scl*operand.vec[1] - vec[0]*operand.vec[2] + vec[1]*operand.scl + vec[2]*operand.vec[0];
	toReturn.vec[2] = scl*operand.vec[2] + vec[0]*operand.vec[1] - vec[1]*operand.vec[0] + vec[2]*operand.scl;
	return toReturn;
}
	
KirkMath::Quat KirkMath::Quat::operator= (KirkMath::Quat operand) {
	scl = operand.scl;
	vec[0] = operand.vec[0];
	vec[1] = operand.vec[1];
	vec[2] = operand.vec[2];
	return (*this);
}

KirkMath::Quat KirkMath::Quat::inv() {
	float norm = this->mag();
	KirkMath::Quat toReturn = this->conj() * (1 / (norm*norm));
	return toReturn;
}

KirkMath::Quat KirkMath::Quat::conj() {
	KirkMath::Quat toReturn;
	toReturn.scl = scl;
	toReturn.vec[0] = -vec[0];
	toReturn.vec[1] = -vec[1];
	toReturn.vec[2] = -vec[2];
	return toReturn;
}

float KirkMath::Quat::mag() {
	return sqrt(scl*scl + vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

void KirkMath::Quat::normalize() {
	// Ensures the vector is normal
	float vecMag = sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
	vec[0] = vec[0] / vecMag;
	vec[1] = vec[1] / vecMag;
	vec[2] = vec[2] / vecMag;
}

void KirkMath::Quat::scale(KirkMath::Quat scaleBy) {
	// Scales this quaternion by the given scaleBy values
	scl = scl * scaleBy.scl;
	vec[0] = vec[0] * scaleBy.vec[0];
	vec[1] = vec[1] * scaleBy.vec[1];
	vec[2] = vec[2] * scaleBy.vec[2];
}

void KirkMath::Quat::unscale(KirkMath::Quat scaleBy) {
	if (scaleBy.scl != 0.0f) scl = scl / scaleBy.scl;
	if (scaleBy.vec[0] != 0.0f) vec[0] = vec[0] / scaleBy.vec[0];
	if (scaleBy.vec[1] != 0.0f) vec[1] = vec[1] / scaleBy.vec[1];
	if (scaleBy.vec[2] != 0.0f) vec[2] = vec[2] / scaleBy.vec[2];
}

void KirkMath::Quat::set(float w, float x, float y, float z) {
	scl = w;
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

void KirkMath::Quat::set(KirkMath::Quat newQuat) {
	scl = newQuat.scl;
	vec[0] = newQuat.vec[0];
	vec[1] = newQuat.vec[1];
	vec[2] = newQuat.vec[2];
}

void KirkMath::Quat::print() {
	printf("[%f, %fi, %fj, %fk]\n", scl, vec[0], vec[1], vec[2]);
}

void KirkMath::Quat::convertValuedToOperational() {
	float tmp = sin(scl / 2);
	scl = cos(scl / 2);
	vec[0] = tmp * vec[0];
	vec[1] = tmp * vec[1];
	vec[2] = tmp * vec[2];
}

void KirkMath::Quat::convertOperationalToValued() {
	if (scl < -1.0f) scl = -1.0f;
	if (scl > 1.0f) scl = 1.0f;
	scl = 2 * acos(scl);
	float tmp = sin(scl / 2);
	vec[0] = vec[0] / tmp;
	vec[1] = vec[1] / tmp;
	vec[2] = vec[2] / tmp;
}

void KirkMath::Quat::reverseValuedRotation() {
	// Reversed rotation is rotation about the same axis, only in the opposite direction
	scl = -1 * scl;
}

void KirkMath::Quat::reverseOperationalRotation() {
	convertOperationalToValued();
	reverseValuedRotation();
	convertValuedToOperational();
}
