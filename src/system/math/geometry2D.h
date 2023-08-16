#pragma once
#include "geometry3D.h"

#define _USE_MATH_DEFINES // for C++
#include <cmath>

struct Matrix2x2;
struct Vec3;

//Vector2
// | a |
// | b |
struct Vec2 {
    union {
        struct { float x, y; };
        struct { float a, b; };
    };

    //TODO : Vec2::sub(cakeBrownSW.copy(), Vec2(motion.GetAbsPosition())).heading())

    Vec2();
    Vec2(float);
    Vec2(float, float);
    static Vec2 fromString(const String& str);

    Vec2& add(const Vec2&);
    Vec2& sub(const Vec2&);
    Vec2& dist(const Vec2&);
    Vec2& mult(float);
    Matrix2x2 toMatrix() const;

    Vec2& setMagnitude(float);
    Vec2& setHeading(float);
    Vec2& normalize();

    Vec2& div(float);
    Vec2& mult(Matrix2x2);
    Vec2& rotate(float);

    float mag();
    float magSq();
    float heading();

    Vec2 copy() const;

    static Vec2 add(const Vec2&, const Vec2&);
    static Vec2 sub( const Vec2&, const Vec2&);
    static Vec2 dist( const Vec2&, const Vec2&);
    static float angleBetween(const Vec2&, const Vec2&);
    static float dot( const Vec2&, const Vec2&);
    static float det( const Vec2&, const Vec2&);

    // Operators overload
    float& operator[](int i);
    Vec2& operator= (const Vec2&);
    Vec2& operator+=(const Vec2& u);
    Vec2& operator-=(const Vec2& u);
    Vec2& operator+=(float u);
    Vec2& operator-=(float u);
    Vec2& operator*=(float u);
    Vec2& operator/=(float u);
    //conversion operator
    operator String() const;
};



//PolarVec
// | heading |
// |   mag   |
struct PolarVec {
    float heading, mag;

    PolarVec();
    PolarVec(float heading , float mag);

    Vec2 toVec2();
};


//Matrix 2x2
// | a   b |
// | c   d |
struct Matrix2x2 {
    float a, b, 
          c, d;

    static Matrix2x2 GetIdentity();

    Matrix2x2 copy();

    Matrix2x2& transpose();
    Matrix2x2& invert();
    Matrix2x2& mult(float);
    Matrix2x2& div(float);
    Matrix2x2& mult(Matrix2x2&);
    Matrix2x2& add(float);
    Matrix2x2& add(Matrix2x2&);
    Matrix2x2& sub(float);
    Matrix2x2& sub(Matrix2x2&);
    float trace();



};

//Operators overload

bool operator== (const Vec2&, const Vec2&);
bool operator!= (const Vec2&, const Vec2&);

bool operator== (const Matrix2x2&, const Matrix2x2&);
bool operator!= (const Matrix2x2&, const Matrix2x2&);

Vec2 operator+(const Vec2& a, const Vec2& b);
Vec2 operator-(const Vec2& a, const Vec2& b);

Vec2 operator*(const Vec2& a, const Vec2& u); //hamadard product
Vec2 operator/(const Vec2& a, const Vec2& u); //hamadard division
Vec2 operator*(const Vec2& a, float u); //scalar multiplication
Vec2 operator/(const Vec2& a, float u); //scalar division