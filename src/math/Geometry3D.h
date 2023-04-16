#pragma once
#include "Geometry2D.h"

struct Matrix3x3;
struct Vec2;

//Vector3
// | a |
// | b |
// | c |
struct Vec3 { 
    float a, 
          b, 
          c;

    Vec3();
    Vec3(Vec2, float);
    Vec3(float);
    Vec3(float,float,float);

    Vec3& add(Vec3&);
    Vec3& sub(Vec3&);
    Vec3& dist(Vec3&);
    Vec3& mult(float);
    Matrix3x3 toMatrix();

    Vec3& div(float);
    Vec3& mult(Matrix3x3);
    Vec3& rotateZ(float);

    float dot(Vec3& a);
    float mag();
    float magSq();

    Vec3 copy() const;

    static Vec3 add(Vec3&, Vec3&);
    static Vec3 sub(Vec3&, Vec3&);
    static Vec3 dist(Vec3&, Vec3&);
    static float angleBetween(Vec3&, Vec3&);
    static float dot(Vec3&, Vec3&);

    operator Vec2() const;
};

//Matrix 3x3
// | a   b   c |
// | d   e   f |
// | g   h   i |
struct Matrix3x3 {
    float a, b, c, 
          d, e, f,
          g, h, i;

    Matrix3x3 copy();

    static Matrix3x3 GetIdentity();
    Matrix3x3& transpose();
    Matrix3x3& mult(float);
    Matrix3x3& div(float);
    Matrix3x3& mult(Matrix3x3&);
    Matrix3x3& add(float);
    Matrix3x3& add(Matrix3x3&);
    Matrix3x3& sub(float);
    Matrix3x3& sub(Matrix3x3&);
    float trace();

};



bool operator== (const Vec3&, const Vec3&);
bool operator!= (const Vec3&, const Vec3&);

bool operator== (const Matrix3x3&, const Matrix3x3&);
bool operator!= (const Matrix3x3&, const Matrix3x3&);