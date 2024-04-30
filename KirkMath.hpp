/**
 * KirkMath.hpp
 */

#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <stdio.h>
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_opengl.h"
#define SMALL_FLOAT 0.0001f

namespace KirkMath {
    class Vec {
    private:
    protected:
        float * vec;
        int len;
    public:
        // Constructors
        Vec();
        Vec(int length);
        Vec(int length, float vals[]);
        Vec(Vec const& b);
        ~Vec();
        
        // Access
        int getLength() { return len; }
        void set(int length, float vals[]);
        
        // Math
        float abs();
        
        // Operators
        friend std::ostream& operator <<(std::ostream &os, const Vec &myVec);
        friend std::ostream& operator <<(std::ostream &os, Vec *myVec);
        float& operator[](int i);
        float operator *(Vec b); // Dot product
        Vec operator %(Vec b);  // Cross product
        Vec operator +(Vec b);
        Vec operator -(Vec b);
        Vec operator *(float b);
        Vec operator /(float b);
        Vec operator =(Vec b);
    };
    
    class Mat {
    private:
    protected:
        Vec * mat;
        int r; int c;
    public:
        // Constructors
        Mat();
        Mat(Mat const& b);
        Mat(Vec b);
        Mat(int size);
        Mat(int rows, int cols);
        Mat(int rows, int cols, float * vals);
        ~Mat();
        
        // Access
        void set(int rows, int cols, float * vals);
        Vec toVec();
        Vec getRow(int row);
        Vec getCol(int col);
        
        // Math
        float norm();
        float trace();
        Mat I();
        Mat transpose();
        int rank();
        Mat invert();
        float det();
        
        // Operators
        friend std::ostream& operator <<(std::ostream &os, const Mat &myMat);
        friend std::ostream& operator <<(std::ostream &os, Mat *myMat);
        Vec& operator[](int i);
        Vec operator *(Vec b);
        Mat operator *(Mat b);
        Mat operator *(float b);
        Mat operator +(Mat b);
        Mat operator -(Mat b);
        Mat operator /(float b);
        Mat operator =(Mat b);
        Mat operator ^(int b);
    };

    class Quat {
    private:
    protected:
        float scl;
        float vec[3];
        friend class Trans;
    public:
        // Constructors
        Quat();
        Quat(const Quat &q);
        Quat(float x, float y, float z);
        Quat(float s, float i, float j, float k);
        ~Quat();
        
        // Operators
        Quat operator+ (Quat operand);
        Quat operator- (Quat operand);
        Quat operator* (float operand);
        Quat operator* (Quat operand);
        Quat operator= (Quat operand);
        
        // Methods
        Quat conj();
        Quat inv();
        float mag();
        void normalize();
        void scale(Quat scaleBy);
        void unscale(Quat scaleBy);
        void set(float w, float x, float y, float z);
        void set(Quat newQuat);
        void print();
        float getScl() { return scl; }
        float getVecI() { return vec[0]; }
        float getVecJ() { return vec[1]; }
        float getVecK() { return vec[2]; }
        
        // Rotation
        // Note the difference between OPERATIONAL and VALUED rotational quaternion,
        // and remember that some quaternions will be merely vectors being
        // transformed by other quaternions or matrices
        void convertValuedToOperational();
        void convertOperationalToValued();
        void reverseValuedRotation();
        void reverseOperationalRotation();
    };

    class Trans {
    private:
    protected:
        Quat * _pos; // Position of child frame. Only i, j, k are used; scl should always be 0
        Quat * _rot; // Rotation of child frame about parent frame. Does NOT store a rotation quaternion, but rather the strict angle-and-vector rotation values
        Quat * _scl; // Scaling of object along local axes
    public:
        Trans();
        ~Trans();
        void setPos(float x, float y, float z);
        void addPos(float x, float y, float z);
        void setRot(float w, float x, float y, float z);
        void addRot(float w, float x, float y, float z);
        void setScl(float x, float y, float z);
        void addScl(float x, float y, float z);
        Quat getPos();
        Quat getRot();
        Quat getScl();
        Trans operator= (Trans operand);
        Quat applyTransformation(Quat subject);
        Quat reverseTransformation(Quat subject);
        void glApply();
        void glUnapply();
        void print();
        void update(Quat* linearVelocity, Quat* angularVelocity, float dt);
    };    

    namespace numericMath {
        float getTolerance();
        void setTolerance(float value);
        int getIterLimit();
        void setIterLimit(int value);
        float bisectionRoot(float(*f)(float), float low, float high);
        float bisectionSolve(float(*f)(float), float low, float high, float target);
        float fmod(float a, float b);
        int nextpoweroftwo(int x);
        //int newRound(double x);
    }    
}
