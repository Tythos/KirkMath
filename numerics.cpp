/*
 * numerics.cpp
 */

#include "KirkMath.hpp"

const int KirkMath::numerics::gIterLimit = 0;
const float KirkMath::numerics::gTolerance = 0.001f;

float KirkMath::numerics::bisectionRoot(float(*f)(float), float low, float high) {
    // Solve for the root of function f via bisection from low, high
    float mid = (low + high) / 2.0f;
    int iter = 0;
    while (f(low) * f(high) < 0 && fabs(f(mid)) > KirkMath::numerics::gTolerance) {
        mid = (low + high) / 2.0f;
        if (f(low) * f(mid) < 0) high = mid;
        else if (f(high) * f(mid) < 0) low = mid;
        iter++;
    }
    return mid;
}
	
float KirkMath::numerics::bisectionSolve(float(*f)(float), float low, float high, float target) {
    // Solve for the root of function f via bisection from low, high
    float mid = (low + high) / 2.0f;
    int iter = 0;
    while ((f(low)-target) * (f(high)-target) < 0 && fabs(f(mid)-target) > KirkMath::numerics::gTolerance && iter < KirkMath::numerics::gIterLimit) {
        mid = (low + high) / 2.0f;
        if ((f(low)-target) * (f(mid)-target) < 0) high = mid;
        else if ((f(high)-target) * (f(mid)-target) < 0) low = mid;
        iter++;
    }
    if (iter == KirkMath::numerics::gIterLimit)
        printf("Numerical Error: Iteration limit reached w/o convergence via bisection\n");
    return mid;
}

float KirkMath::numerics::fmod(float a, float b) {
    if (b == 0) return a;
    int result = static_cast<int>( a / b );
    return a - static_cast<float>( result ) * b;
}
	
int KirkMath::numerics::nextpoweroftwo(int x) {
    float arg = (float)x;
    double logbase2 = log(arg) / log(2.0f);
    double d = pow(2, ceil(logbase2));
    return (int)(d + 0.5);
}	
