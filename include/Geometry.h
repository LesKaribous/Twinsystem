#pragma once

struct Matrix2x2;

//Vector2
// | a |
// | b |
struct Vec2 { 
    float a, b;

    Vec2& add(Vec2&);
    Vec2& sub(Vec2&);
    Vec2& dist(Vec2&);
    Vec2& mult(float);
    Matrix2x2 toMatrix();

    Vec2& div(float);
    Vec2& mult(Matrix2x2);
    Vec2& rotate(float);

    float dot(Vec2& a);
    float mag();
    float magSq();

    Vec2 copy();

    static Vec2 add(Vec2&, Vec2&);
    static Vec2 sub(Vec2&, Vec2&);
    static Vec2 dist(Vec2&, Vec2&);
    static float angleBetween(Vec2&, Vec2&);
    static float dot(Vec2&, Vec2&);
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

struct Matrix3x3;

//Vector3
// | a |
// | b |
// | c |
struct Vec3 { 
    float a, 
          b, 
          c;


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

    Vec3 copy();

    static Vec3 add(Vec3&, Vec3&);
    static Vec3 sub(Vec3&, Vec3&);
    static Vec3 dist(Vec3&, Vec3&);
    static float angleBetween(Vec3&, Vec3&);
    static float dot(Vec3&, Vec3&);
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


//Operators overload
bool operator== (const Vec2&, const Vec2&);
bool operator!= (const Vec2&, const Vec2&);

bool operator== (const Vec3&, const Vec3&);
bool operator!= (const Vec3&, const Vec3&);

bool operator== (const Matrix2x2&, const Matrix2x2&);
bool operator!= (const Matrix2x2&, const Matrix2x2&);

bool operator== (const Matrix3x3&, const Matrix3x3&);
bool operator!= (const Matrix3x3&, const Matrix3x3&);