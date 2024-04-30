/*
 * Trans.cpp
 */

#include "KirkMath.hpp"

KirkMath::Trans::Trans() {
	_pos = new KirkMath::Quat(0.0f, 0.0f, 0.0f, 0.0f);
	_rot = new KirkMath::Quat(0.0f, 1.0f, 0.0f, 0.0f);
	_scl = new KirkMath::Quat(0.0f, 1.0f, 1.0f, 1.0f);
}

KirkMath::Trans::~Trans() {
	delete _pos;
	delete _rot;
	delete _scl;
}

void KirkMath::Trans::setPos(float x, float y, float z) {
	// Change position quaternion
	_pos->set(0.0f, x, y, z);
}

void KirkMath::Trans::addPos(float x, float y, float z) {
	// Increment position by given amount
	KirkMath::Quat diff = KirkMath::Quat(0.0f, x, y, z);
	(*_pos) = (*_pos) + diff;
}

void KirkMath::Trans::setRot(float w, float x, float y, float z) {
	// Change rotation quaternion
	_rot->set(w, x, y, z);
}

void KirkMath::Trans::addRot(float w, float x, float y, float z) {
	// Add new rotation after current rotation (both are VALUED)
	KirkMath::Quat* newRot = new KirkMath::Quat(w, x, y, z);
	newRot->convertValuedToOperational();
	_rot->convertValuedToOperational();
	_rot->set((*newRot) * (*_rot));
	_rot->convertOperationalToValued();
}

void KirkMath::Trans::setScl(float x, float y, float z) {
	_scl->set(0.0f, x, y, z);
}

void KirkMath::Trans::addScl(float x, float y, float z) {
	// Adjust scaling by given percentages
	_scl->vec[0] = _scl->vec[0] * x;
	_scl->vec[1] = _scl->vec[1] * y;
	_scl->vec[2] = _scl->vec[2] * z;
}

KirkMath::Quat KirkMath::Trans::getPos() {
	KirkMath::Quat toReturn = (*_pos);
	return toReturn;
}

KirkMath::Quat KirkMath::Trans::getRot() {
	KirkMath::Quat toReturn = (*_rot);
	return toReturn;
}

KirkMath::Quat KirkMath::Trans::getScl() {
	KirkMath::Quat toReturn = (*_scl);
	return toReturn;
}

KirkMath::Trans KirkMath::Trans::operator= (KirkMath::Trans operand) {
	(*_pos) = (*operand._pos);
	(*_rot) = (*operand._rot);
	(*_scl) = (*operand._scl);
	return (*this);
}

KirkMath::Quat KirkMath::Trans::applyTransformation(KirkMath::Quat subject) {
	// Transform parent vector into child vector through frame:
	// Subtract transposition, apply rotation, and scale
	KirkMath::Quat toReturn = subject - (*_pos);
	KirkMath::Quat* operationalRotation = new KirkMath::Quat(*_rot);
	operationalRotation->convertValuedToOperational();
	toReturn = ((*operationalRotation) * toReturn * operationalRotation->inv());
	toReturn.scale(*_scl);
	return toReturn;
}

KirkMath::Quat KirkMath::Trans::reverseTransformation(KirkMath::Quat subject) {
	// Transform child vector into parent vector through reverse frame:
	// Reverse rotation, unscale, then add back transposition
	KirkMath::Quat* rotation = new KirkMath::Quat(*_rot);
	rotation->reverseValuedRotation();
	rotation->convertValuedToOperational();
	KirkMath::Quat toReturn = (*rotation) * subject * rotation->inv();
	toReturn.unscale(*_scl);
	return toReturn + (*_pos);
}
	
void KirkMath::Trans::glApply() {
	// Apply the transformation defined by this rotation and position
	glTranslatef(_pos->getVecI(), _pos->getVecJ(), _pos->getVecK());
	glPushMatrix();
	glRotatef(_rot->getScl() * 180.0f / float(M_PI), _rot->getVecI(), _rot->getVecJ(), _rot->getVecK());
	glScalef(_scl->vec[0], _scl->vec[1], _scl->vec[2]);
}

void KirkMath::Trans::glUnapply() {
	// Unapply transformation and shift back translation
	glPopMatrix();
	glTranslatef(-1 * _pos->getVecI(), -1 * _pos->getVecJ(), -1 * _pos->getVecK());
}

void KirkMath::Trans::print() {
	// Print both quaternions
	printf("Translation:       "); _pos->print();
	printf("Rotation (valued): "); _rot->print();
	printf("Scale:             "); _scl->print();
}

void KirkMath::Trans::update(KirkMath::Quat* linearVelocity, KirkMath::Quat* angularVelocity, float dt) {
	// Position is updated linearly
	(*_pos) = (*_pos) + ((*linearVelocity) * dt);
	
	// Angular velocity and rotation must be combined operationally
	KirkMath::Quat * operationalAngularVelocity = new KirkMath::Quat(*angularVelocity);
	operationalAngularVelocity->scl = operationalAngularVelocity->scl * dt;
	if (angularVelocity->scl == 0.0f || angularVelocity->scl == 2 * M_PI) {
		// No angular velocity; keep old rotation
	} else if (_rot->scl == 0.0f || _rot->scl == 2 * M_PI) {
		// Frame has not rotated yet; apply one step of angular velocity
		(*_rot) = (*operationalAngularVelocity);
	} else {
		// Convert both to operational; combine; revert to valued
		operationalAngularVelocity->convertValuedToOperational();
		_rot->convertValuedToOperational();
		(*_rot) = (*operationalAngularVelocity) * (*_rot);
		_rot->convertOperationalToValued();
	}
		
	// Reduce angle to below 2 * Pi
	if (_rot->getScl() > 2 * M_PI) {
		float angle = fmod(_rot->getScl(), (float)(2 * M_PI));
		_rot->scl = angle;
	}
	
	// Normalize angle of rotation
	_rot->normalize();
}
