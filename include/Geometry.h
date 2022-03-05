#pragma once

struct Matrix2x2;

//Vector2
// | a |
// | b |
struct Vec2 { 
    float x, y;

    Vec2& add(Vec2&);
    Vec2& sub(Vec2&);
    Vec2& dist(Vec2&);
    Vec2& mult(float);
    Vec2& div(float);
    Vec2& mult(Matrix2x2);
    Vec2& rotate(float);

    float dot(Vec2& a);
    float mag();
    float magSq();


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