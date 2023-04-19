#include "Geometry3D.h"
#include <Math.h>

//---------- Vector3 ------------
//Methods

Vec3::Vec3(){
    a = b = c = 0.0f;
}

Vec3::Vec3(Vec2 v, float f){
    a = v.a; b = v.b; c = f; 
}

Vec3::Vec3(float v){
    a = b = c = v;
}

Vec3::Vec3(float x,float y,float z){
    a = x; b = y; c = z;
}

Vec3 Vec3::copy() const{
    return *this;
}

Vec3& Vec3::add(Vec3& v){
    a += v.a; 
    b += v.b;  
    c += v.c;
    return *this;
}

Vec3& Vec3::sub(Vec3& v){
    a -= v.a; 
    b -= v.b; 
    c -= v.c;
    return *this;
}

Vec3& Vec3::dist(Vec3& v){
    return this->sub(v);
}

float Vec3::dot(Vec3& a){
    Vec3::dot(*this, a);
}

float Vec3::mag(){
    return sqrt(magSq());
}

float Vec3::magSq(){
    return a*a + b*b + c*c;
}

Vec3& Vec3::mult(float v){
    a *= v; 
    b *= v;
    c *= v;
    return *this;
}

Vec3& Vec3::mult(Matrix3x3 m){
    float na = m.a * a + m.b * b + m.c * c;
    float nb = m.d * a + m.e * b + m.f * c;
    float nc = m.g * a + m.h * b + m.i * c;
    a = na;
    b = nb;
    c = nc;
    return *this;
}

Matrix3x3 Vec3::toMatrix(){
    return {a,0,0,
            0,b,0,
            0,0,c};
}

Vec3& Vec3::rotateZ(float x){

    this->mult({ cosf(x), -sinf(x), 0, 
                 sinf(x),  cosf(x), 0,
                    0   ,     0   , 1 } );

    return *this;
}

//Static Methods
Vec3 Vec3::add(Vec3& a, Vec3& b){
    a.a += b.a; a.b += b.b;
    return a;
}

Vec3 Vec3::sub(Vec3& a, Vec3& b){
    a.a -= b.a; a.b -= b.b;
    return a;
}

float Vec3::dot(Vec3& a, Vec3& b){
    return a.a * b.a + a.b * b.b + a.c * b.c;
}

Vec3 Vec3::dist(Vec3& a, Vec3& b){
    a.sub(b);
    return a;
}

float Vec3::angleBetween(Vec3& a, Vec3& b){
    return acosf(a.dot(b) / a.mag() * b.mag());
}


//---------- Matrix3x3 ------------
// Methods

Matrix3x3 Matrix3x3::copy(){
    return *this;
}

Matrix3x3& Matrix3x3::transpose(){

    Matrix3x3 cpy = *this;

    b = cpy.d;
    c = cpy.g;
    d = cpy.b;
    f = cpy.h;
    g = cpy.c;
    h = cpy.f;
    return *this;
}

Matrix3x3& Matrix3x3::mult(float v){
    a *= v;
    b *= v;
    c *= v;
    d *= v;
    e *= v;
    f *= v;
    g *= v;
    h *= v;
    i *= v;
    return *this;
}

Matrix3x3& Matrix3x3::div(float v){
    a /= v;
    b /= v;
    c /= v;
    d /= v;
    e /= v;
    f /= v;
    g /= v;
    h /= v;
    i /= v;
    return *this;
}

Matrix3x3& Matrix3x3::mult(Matrix3x3& mb){
    Matrix3x3 ma = *this;
    float 
    na = ma.a * mb.a + ma.b * mb.d + ma.c * mb.g,
    nb = ma.a * mb.b + ma.b * mb.e + ma.c * mb.h,
    nc = ma.a * mb.c + ma.b * mb.f + ma.c * mb.i,
    nd = ma.d * mb.a + ma.e * mb.d + ma.f * mb.g,
    ne = ma.d * mb.b + ma.e * mb.e + ma.f * mb.h,
    nf = ma.d * mb.c + ma.e * mb.f + ma.f * mb.i,
    ng = ma.g * mb.a + ma.h * mb.d + ma.i * mb.g,
    nh = ma.g * mb.b + ma.h * mb.e + ma.i * mb.h,
    ni = ma.g * mb.c + ma.h * mb.f + ma.i * mb.i;

    a = na; b = nb; c = nc; 
    d = nd; e = ne; f = nf; 
    g = ng; h = nh; i = ni;

    return *this;
}

Matrix3x3& Matrix3x3::add(float v){
    a += v;
    b += v;
    c += v;
    d += v;
    e += v;
    f += v;
    g += v;
    h += v;
    i += v;
    return *this;
}

Matrix3x3& Matrix3x3::add(Matrix3x3& m){
    a += m.a;
    b += m.b;
    c += m.c;
    d += m.d;
    e += m.e;
    f += m.f;
    g += m.g;
    h += m.h;
    i += m.i;
    return *this;
}

Matrix3x3& Matrix3x3::sub(float v){
    a -= v;
    b -= v;
    c -= v;
    d -= v;
    e -= v;
    f -= v;
    g -= v;
    h -= v;
    i -= v;
    return *this;
}

Matrix3x3& Matrix3x3::sub(Matrix3x3& m){
    a -= m.a;
    b -= m.b;
    c -= m.c;
    d -= m.d;
    e -= m.e;
    f -= m.f;
    g -= m.g;
    h -= m.h;
    i -= m.i;
    return *this;
}

float Matrix3x3::trace(){
    return a+e+i;
}


//Static methods
Matrix3x3 Matrix3x3::GetIdentity(){
    return {1,0,0,
            0,1,0,
            0,0,1};
}

Vec3::operator Vec2() const{ 
    Vec2 result;
    result.a = a;
    result.b = b;
    return result; 
}

bool operator== (const Vec3& a, const Vec3& b){ 
    return (a.a == b.a &&
            a.b == b.b && 
            a.c == b.c  );
}
bool operator!= (const Vec3& a, const Vec3& b){ 
    return (a.a != b.a ||
            a.b != b.b || 
            a.c != b.c  );
}


bool operator== (const Matrix3x3& a, const Matrix3x3& b){ 
    return (a.a == b.a &&
            a.b == b.b && 
            a.c == b.c &&
            a.d == b.d &&
            a.e == b.e &&
            a.f == b.f &&
            a.g == b.g &&
            a.h == b.h &&
            a.i == b.i );
}
bool operator!= (const Matrix3x3& a, const Matrix3x3& b){ 
    return (a.a != b.a ||
            a.b != b.b || 
            a.c != b.c ||
            a.d != b.d ||
            a.e != b.e ||
            a.f != b.f ||
            a.g != b.g ||
            a.h != b.h ||
            a.i != b.i );
}