#include "geometry3D.h"
//---------- Vector3 ------------
//Methods

Vec3::Vec3(){
    a = b = c = 0.0f;
}

Vec3::Vec3(const Vec2& v, float f){
    a = v.a; b = v.b; c = f; 
}

Vec3::Vec3(float v){
    a = b = c = v;
}

Vec3::Vec3(float x,float y,float z){
    a = x; b = y; c = z;
}

Vec3 Vec3::fromString(const String& str){
    bool error = false;
    String v = str;
    v = v.trim(); 
    
    String x_str = "";
    String y_str = "";
    String z_str = "";

    if(v.startsWith("[") && v.endsWith("]")){
        int i = 1; //skip the [
        
        while(v.charAt(i) != ','){
            if(!isdigit(v.charAt(i))) error = true;
            x_str += v.charAt(i++);
        }

        i++; //skip the ,

        while(v.charAt(i) != ','){
            if(!isdigit(v.charAt(i))) error = true;
            y_str += v.charAt(i++);
        }

        i++; //skip the ,
        
        while(v.charAt(i) != ']'){
            if(!isdigit(v.charAt(i))) error = true;
            z_str += v.charAt(i++);
        }


    }else error = true;
    
    if(error){
       //Console::error("Vec2") << "Syntax error in Vec2 constructor. Given :" << str << Console::endl;
    }else{
        float x = x_str.toFloat();
        float y = y_str.toFloat();
        float z = z_str.toFloat();
        return Vec3(x, y,z);
    }
    return Vec3(0,0,0);
    
}

Vec3 Vec3::copy() const{
    return *this;
}

Vec3& Vec3::add(const Vec3& v){
    a += v.a; 
    b += v.b;  
    c += v.c;
    return *this;
}

Vec3& Vec3::sub(const Vec3& v){
    a -= v.a; 
    b -= v.b; 
    c -= v.c;
    return *this;
}


float Vec3::dot(const Vec3& a) const{
    return Vec3::dot(*this, a);
}

float Vec3::mag() const{
    return sqrt(magSq());
}

float Vec3::magSq() const{
    return a*a + b*b + c*c;
}

Vec3& Vec3::mult(float v){
    a *= v; 
    b *= v;
    c *= v;
    return *this;
}

Vec3& Vec3::div(float v){
    a /= v; 
    b /= v;
    c /= v;
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

Vec3& Vec3::rotateZ(float x){

    this->mult({ cosf(x), -sinf(x), 0, 
                 sinf(x),  cosf(x), 0,
                    0   ,     0   , 1 } );

    return *this;
}

//Static Methods
Vec3 Vec3::add(const Vec3& a, const Vec3& b){
    Vec3 result = a;

    result.a += result.a; 
    result.b += result.b; 
    result.c += result.c;
    
    return result;
}

Vec3 Vec3::sub(const Vec3& a,const Vec3& b){
    Vec3 result = a;

    result.a -= result.a; 
    result.b -= result.b; 
    result.c -= result.c;

    return result;
}

float Vec3::dot(const Vec3& a,const Vec3& b){
    return a.a * b.a + a.b * b.b + a.c * b.c;
}

float Vec3::distanceBetween(const Vec3& a,const Vec3& b){
    return sub(a,b).mag();
}

float Vec3::angleBetween(const Vec3& a, const Vec3& b){
    return acosf(dot(a,b) / a.mag() * b.mag());
}


Vec3::operator Vec2() const{
    Vec2 v = Vec2(a, b);
    return v;
}

Vec3::operator String() const{
    String v = "[" + String(a,4)  + "," + String(b,4) + "," + String(c,4) + "]";
    return v;
}


float& Vec3::operator[](int i){

    
    if(i == 0) return x;
    if(i == 1) return y;
    if(i == 2) return z;

    //Console::error("Vec3") << "Index out of bounds" << Console::endl; 
    return x;
}


Vec3& Vec3::operator= (const Vec2& u){
    a=u.a;
    b=u.b;
    return *this;
}

Vec3& Vec3::operator= (const Vec3& u){
    a=u.a;
    b=u.b;
    c=u.c;
    return *this;
}
Vec3& Vec3::operator+=(const Vec3& u){
    a+=u.a;
    b+=u.b;
    c+=u.c;
    return *this;
}
Vec3& Vec3::operator-=(const Vec3& u){
    a-=u.a;
    b-=u.b;
    c-=u.c;
    return *this;
}
Vec3& Vec3::operator*=(const Vec3& u){
    a*=u.a;
    b*=u.b;
    c*=u.c;
    return *this;
}
Vec3& Vec3::operator/=(const Vec3& u){
    a/=u.a;
    b/=u.b;
    c/=u.c;
    return *this;
}
Vec3& Vec3::operator+=(const Vec2& u){
    a+=u.a;
    b+=u.b;
    return *this;
}
Vec3& Vec3::operator-=(const Vec2& u){
    a-=u.a;
    b-=u.b;
    return *this;
}

Vec3& Vec3::operator+=(float u){
    a+=u;
    b+=u;
    c+=u;
    return *this;
}
Vec3& Vec3::operator-=(float u){
    a-=u;
    b-=u;
    c-=u;
    return *this;
}
Vec3& Vec3::operator*=(float u){
    a*=u;
    b*=u;
    c*=u;
    return *this;
}
Vec3& Vec3::operator/=(float u){
    a/=u;
    b/=u;
    c/=u;
    return *this;
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

Vec3 operator+(const Vec3& u, const Vec3& v){
    return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}
Vec3 operator-(const Vec3& u, const Vec3& v){
    return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}
Vec3 operator+(const Vec3& u, const Vec2& v){
    return Vec3(u.x + v.x, u.y + v.y, u.z);
}
Vec3 operator-(const Vec3& u, const Vec2& v){
    return Vec3(u.x - v.x, u.y - v.y, u.z);
}

Vec3 operator*(const Vec3& u, const Vec3& v){ //hamadard product
    return Vec3(u.a * v.a, u.b * v.b, u.c * v.c);
} 
Vec3 operator/(const Vec3& u, const Vec3& v){ //hamadard division
    return Vec3(u.a / v.a, u.b / v.b, u.c / v.c);
} 
Vec3 operator*(const Vec3& u, float v){ //scalar multiplication
    return Vec3(u.a*v, u.b*v, u.c*v);
} 
Vec3 operator/(const Vec3& u, float v){ //scalar division
    return Vec3(u.a/v, u.b/v, u.c/v);
} 
