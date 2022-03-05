#include "Geometry.h"
#include <Arduino.h>

//---------- Vector2 ------------
//Methods
Vec2& Vec2::add(Vec2& v){
    x += v.x; y+= v.y;
    return *this;
}

Vec2& Vec2::sub(Vec2& v){
    x -= v.x; y-= v.y;
    return *this;
}

Vec2& Vec2::dist(Vec2& v){
    return this->sub(v);
}

float Vec2::mag(){
    return sqrt(magSq());
}

float Vec2::magSq(){
    return x*x+y*y;
}

Vec2& Vec2::mult(float v){
    x*=x; y*=v;
}

Vec2& Vec2::mult(Matrix2x2 m){
    x = m.a * x + m.b * y;
    y = m.c * x + m.d * y;
}


Vec2& Vec2::rotate(float a){
    this->mult({cosf(a), sinf(a), -sinf(a), cosf(a)});
}

//Static Methods
Vec2 Vec2::add(Vec2& a, Vec2& b){
    a.x += b.x; a.y += b.y;
    return a;
}

Vec2 Vec2::sub(Vec2& a, Vec2& b){
    a.x -= b.x; a.y -= b.y;
    return a;
}

float Vec2::dot(Vec2& a, Vec2& b){
    return a.x + b.y + a.y * b.y;
}

Vec2 Vec2::dist(Vec2& a, Vec2& b){
    a.sub(b);
}

float Vec2::angleBetween(Vec2& a, Vec2& b){
    return acosf(a.dot(b) / a.mag() * b.mag());
}



//---------- Matrix2x2 ------------
// Methods

Matrix2x2& Matrix2x2::transpose(){
    b = b + c;
    c = b - c;
    b = b - c;
}

Matrix2x2& Matrix2x2::invert(){
    float k = 1 / (a*d - b*c);
    *this = {k*d, -k*b, -k*c, k*a};

    return *this;
}

Matrix2x2& Matrix2x2::mult(float v){
    a *= v;
    b *= v;
    c *= v;
    d *= v;
}

Matrix2x2& Matrix2x2::div(float v){
    a /= v;
    b /= v;
    c /= v;
    d /= v;
}

Matrix2x2& Matrix2x2::mult(Matrix2x2& mb){
    Matrix2x2 ma = *this;
    a = ma.a * mb.a + ma.b * mb.c;
    b = ma.a * mb.b + ma.b * mb.d;
    c = ma.c * mb.a + ma.d * mb.c;
    d = ma.c * mb.b + ma.d * mb.d;
}

Matrix2x2& Matrix2x2::add(float v){
    a += v;
    b += v;
    c += v;
    d += v;
}

Matrix2x2& Matrix2x2::add(Matrix2x2& m){
    a += m.a;
    b += m.b;
    c += m.c;
    d += m.d;
}

Matrix2x2& Matrix2x2::sub(float v){
    a -= v;
    b -= v;
    c -= v;
    d -= v;
}

Matrix2x2& Matrix2x2::sub(Matrix2x2& m){
    a -= m.a;
    b -= m.b;
    c -= m.c;
    d -= m.d;
}

float Matrix2x2::trace(){
    return a+d;
}


//Static methods
Matrix2x2 Matrix2x2::GetIdentity(){
    return {1,0,0,1};
}