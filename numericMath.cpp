/*
 * numericMath.cpp
 */

#include "KirkMath.hpp"

int KirkMath::numericMath::gIterLimit = 0;
float KirkMath::numericMath::gTolerance = 0.001f;

float KirkMath::numericMath::bisectionRoot(float(*f)(float), float low, float high) {
    // Solve for the root of function f via bisection from low, high
    float mid = (low + high) / 2.0f;
    int iter = 0;
    while (f(low) * f(high) < 0 && fabs(f(mid)) > gTolerance) {
        mid = (low + high) / 2.0f;
        if (f(low) * f(mid) < 0) high = mid;
        else if (f(high) * f(mid) < 0) low = mid;
        iter++;
    }
    return mid;
}
	
float KirkMath::numericMath::bisectionSolve(float(*f)(float), float low, float high, float target) {
    // Solve for the root of function f via bisection from low, high
    float mid = (low + high) / 2.0f;
    int iter = 0;
    while ((f(low)-target) * (f(high)-target) < 0 && fabs(f(mid)-target) > gTolerance && iter < gIterLimit) {
        mid = (low + high) / 2.0f;
        if ((f(low)-target) * (f(mid)-target) < 0) high = mid;
        else if ((f(high)-target) * (f(mid)-target) < 0) low = mid;
        iter++;
    }
    if (iter == gIterLimit)
        printf("Numerical Error: Iteration limit reached w/o convergence via bisection\n");
    return mid;
}
	
float KirkMath::numericMath::getTolerance() {
    return gTolerance;
}
	
void KirkMath::numericMath::setTolerance(float value) {
    gTolerance = value;
}
	
int KirkMath::numericMath::getIterLimit() {
    return gIterLimit;
}
	
void KirkMath::numericMath::setIterLimit(int value) {
    gIterLimit = value;
}

float KirkMath::numericMath::fmod(float a, float b) {
    if (b == 0) return a;
    int result = static_cast<int>( a / b );
    return a - static_cast<float>( result ) * b;
}
	
int KirkMath::numericMath::nextpoweroftwo(int x) {
    float arg = (float)x;
    double logbase2 = log(arg) / log(2.0f);
    double d = pow(2, ceil(logbase2));
    return (int)(d + 0.5);
}	
