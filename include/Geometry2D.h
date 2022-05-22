#pragma once
#include "Geometry3D.h"

struct Matrix2x2;
struct Vec3;

//Vector2
// | a |
// | b |
struct Vec2 { 
    float a, b;

    Vec2();
    Vec2(float);
    Vec2(float, float);

    Vec2& add(Vec2&);
    Vec2& sub(Vec2&);
    Vec2& dist(Vec2&);
    Vec2& mult(float);
    Matrix2x2 toMatrix();

    Vec2& setMagnitude(float);
    Vec2& setHeading(float);
    Vec2& normalize();

    Vec2& div(float);
    Vec2& mult(Matrix2x2);
    Vec2& rotate(float);

    float mag();
    float magSq();
    float heading();

    Vec2 copy();

    static Vec2 add(Vec2&, Vec2&);
    static Vec2 sub(Vec2&, Vec2&);
    static Vec2 dist(Vec2&, Vec2&);
    static float angleBetween(Vec2&, Vec2&);
    static float dot(Vec2&, Vec2&);
    static float det(Vec2&, Vec2&);

    void operator= (const Vec3&);

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
