#pragma once
#include "system/core/lib.h"
#include "geometry2D.h"

struct Matrix3x3;
struct Vec2;

//Vector3
// | a |
// | b |
// | c |
struct Vec3 { 
    union {
        struct { float x, y, z; };
        struct { float a, b, c; };
    };

    //Constructors
    Vec3();
    Vec3(const Vec2&, float);
    Vec3(float);
    Vec3(float,float,float);

    //Operation
    Vec3& add(const Vec3&);
    Vec3& sub(const Vec3&);
    Vec3& mult(const float);
    Vec3& div(const float);

    const Vec3& hProduct(const Vec3&); //Perform hadamard product u * v = {u.a * v.a, u.b * v.b, u.c * v.c}
    Vec3& mult(Matrix3x3);
    Vec3& rotateZ(float);

    //Properties
    float mag() const;
    float magSq() const;
    float heading() const;
    float dot(const Vec3&) const;

    Vec3 copy() const;

    //Static methods
    static Vec3  add(const Vec3&, const Vec3&);
    static Vec3  sub(const Vec3&, const Vec3&);
    static float dot(const Vec3&, const Vec3&);
    static float angleBetween(const Vec3&, const Vec3&);
    static float distanceBetween(const Vec3&, const Vec3&);

    // Operators overload
    operator Vec2() const;
    operator String() const;

    float& operator[](int);// Overloading [] operator to access elements in array style

    Vec3& operator= (const Vec2&);
    Vec3& operator= (const Vec3&);
    Vec3& operator+=(const Vec3&);
    Vec3& operator-=(const Vec3&);
    Vec3& operator*=(const Vec3&);
    Vec3& operator/=(const Vec3&);

    Vec3& operator+=(const Vec2&);
    Vec3& operator-=(const Vec2&);
    Vec3& operator+=(float);
    Vec3& operator-=(float);
    Vec3& operator*=(float);
    Vec3& operator/=(float);

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

Vec3 operator+(const Vec3& a, const Vec3& b);
Vec3 operator-(const Vec3& a, const Vec3& b);
Vec3 operator+(const Vec3& a, const Vec2& b);
Vec3 operator-(const Vec3& a, const Vec2& b);

Vec3 operator*(const Vec3& a, const Vec3& u); //hamadard product
Vec3 operator*(const Vec3& a, const Vec3& u); //hamadard division
Vec3 operator*(const Vec3& a, float u); //scalar multiplication
Vec3 operator/(const Vec3& a, float u); //scalar division
