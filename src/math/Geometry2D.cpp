#include "Geometry2D.h"
#include "Geometry3D.h"
#include <math.h>
#include <Arduino.h>
#include "Settings.h"

//---------- Polar Vec --------

PolarVec::PolarVec(){
    heading = 0;
    mag = 0;
}

PolarVec::PolarVec(float _heading , float _mag){
    heading = _heading;
    mag = _mag;
}

Vec2 PolarVec::toVec2(){
    Vec2 result(mag*cosf(-heading),mag*sinf(-heading));
    //Vec2 result(1,0);
    //result.setHeading(heading);
    //result.setMagnitude(mag);
    return result;
}


//---------- Vector2 ------------
//Methods

Vec2::Vec2(){
    a = b = 0.0f;
}

/*
Vec2::Vec2(float v){
    a = b = v;
}*/

Vec2::Vec2(float x, float y){
    a = x; b = y;
}

Vec2 Vec2::copy(){
    return *this;
}

Vec2& Vec2::add(Vec2& v){
    a += v.a;
    b += v.b;
    return *this;
}

Vec2& Vec2::sub(Vec2& v){
    a -= v.a; 
    b -= v.b;
    return *this;
}

Vec2& Vec2::dist(Vec2& v){
    return this->sub(v);
}

float Vec2::mag(){
    return sqrt(magSq());
}

float Vec2::heading(){
    return -atan2(b,a);
}

float Vec2::magSq(){
    return a*a+b*b;
}

Matrix2x2 Vec2::toMatrix() const{
    return {a, 0,
            0, b};
}


Vec2& Vec2::setMagnitude(float newMag){
    if(mag() != 0){
        normalize();
        a *= newMag;
        b *= newMag;
    }else a = newMag;
    return *this;
}

Vec2& Vec2::setHeading(float head){
    Vec2 xAxis(1,0);
    rotate(Vec2::angleBetween( xAxis, *this));
    return *this;
}

Vec2& Vec2::normalize(){
    if(mag() != 0){
        float cMag = mag();
        a /= cMag * a > 0 ? 1 : -1;
        b /= cMag * b > 0 ? 1 : -1;
    }
    return *this;
}


Vec2& Vec2::mult(float v){
    a*=v; 
    b*=v;
    return *this;
}

Vec2& Vec2::mult(Matrix2x2 m){
    float 
    na = m.a * a + m.b * b,
    nb = m.c * a + m.d * b;
    a = na;
    b = nb;
    return *this;
}


Vec2& Vec2::rotate(float x){

    while(x > PI) x -= 2.0f*PI;
    while(x < -PI) x += 2.0f*PI;

    this->mult( { cosf(x), -sinf(x), 
                  sinf(x), cosf(x)} );
    return *this;
}

//Static Methods
Vec2 Vec2::add(Vec2& a, Vec2& b){
    a.a += b.a; 
    a.b += b.b;
    return a;
}

Vec2 Vec2::sub(Vec2& a, Vec2& b){
    a.a -= b.a; a.b -= b.b;
    return a;
}

float Vec2::dot(Vec2& a, Vec2& b){
    return a.a * b.a + a.b * b.b;
}

float Vec2::det(Vec2& a, Vec2& b){
    return a.a * b.a - a.b * b.b;
}

Vec2 Vec2::dist(Vec2& a, Vec2& b){
    a.sub(b);
    return a;
}

float Vec2::angleBetween(Vec2& a, Vec2& b){
    return atan2(det(a,b), dot(a,b));
    //return acosf( Vec2::dot(a,b) / a.mag() * b.mag() );
}


//---------- Matrix2x2 ------------
// Methods

Matrix2x2 Matrix2x2::copy(){
    return *this;
}


Matrix2x2& Matrix2x2::transpose(){
    b = b + c;
    c = b - c;
    b = b - c;
    return *this;
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
    return *this;
}

Matrix2x2& Matrix2x2::div(float v){
    a /= v;
    b /= v;
    c /= v;
    d /= v;
    return *this;
}

Matrix2x2& Matrix2x2::mult(Matrix2x2& mb){
    Matrix2x2 ma = *this;
    float
    na = ma.a * mb.a + ma.b * mb.c,
    nb = ma.a * mb.b + ma.b * mb.d,
    nc = ma.c * mb.a + ma.d * mb.c,
    nd = ma.c * mb.b + ma.d * mb.d;

    a = na; b = nb; c = nc; d = nd;
    
    return *this;
}

Matrix2x2& Matrix2x2::add(float v){
    a += v;
    b += v;
    c += v;
    d += v;
    return *this;
}

Matrix2x2& Matrix2x2::add(Matrix2x2& m){
    a += m.a;
    b += m.b;
    c += m.c;
    d += m.d;
    return *this;
}

Matrix2x2& Matrix2x2::sub(float v){
    a -= v;
    b -= v;
    c -= v;
    d -= v;
    return *this;
}

Matrix2x2& Matrix2x2::sub(Matrix2x2& m){
    a -= m.a;
    b -= m.b;
    c -= m.c;
    d -= m.d;
    return *this;
}

float Matrix2x2::trace(){
    return a+d;
}


//Static methods
Matrix2x2 Matrix2x2::GetIdentity(){
    return {1,0,
            0,1};
}



// Operators overload

void Vec2::operator= (const Vec3& copy){
    a = copy.a;
    b = copy.b;
}

bool operator== (const Vec2& a, const Vec2& b){ 
    return (a.a == b.a &&
                a.b == b.b);
}
bool operator!= (const Vec2& a, const Vec2& b){ 
    return (a.a != b.a ||
                a.b != b.b);
}

bool operator== (const Matrix2x2& a, const Matrix2x2& b){ 
    return (a.a == b.a &&
            a.b == b.b && 
            a.c == b.c &&
            a.d == b.d );
}
bool operator!= (const Matrix2x2& a, const Matrix2x2& b){ 
    return (a.a != b.a ||
            a.b != b.b || 
            a.c != b.c ||
            a.d != b.d  );
}

