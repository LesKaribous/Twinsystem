#include "Geometry.h"
#include "Settings.h"

#include <Arduino.h>

Path::Path(Point _origin, Point _end){
    origin = _origin;
    end = _end;
}

Path::~Path(){}

Trajectory::Trajectory(Point _origin) 
    : Path(_origin,_origin){}

Trajectory::Trajectory(Point _origin, Point _end)
    : Path(_origin,_end){}

Trajectory::~Trajectory(){
    for(int i = 0; i < path.size(); i++){
        path[i]->~Path();
    }
}



Vec3 ik(Vec3 target){
    float c60 = cosf(PI/3.0f),
            s60 = sinf(PI/3.0f),
            L = Settings::RADIUS,
            R = Settings::WHEEL_RADIUS;
    
    if(Settings::purple()){
        Vec3 transform(1,-1,-1);
        target.mult(transform.toMatrix());
    }

    Matrix3x3 P = {
            0,   1 , L,
        -s60, -c60, L,
            s60, -c60, L
    };

    return target.mult(P).mult(1/R);
}

Vec3 fk(Vec3 target){
    float f1sq3 = 1.0f / sqrt(3.0f),		  
            f1s3 = 1.0f/3.0f,
            f2s3 = 1.0f/3.0f,
            L = Settings::RADIUS,
            R = Settings::WHEEL_RADIUS;
    

    Matrix3x3 P = {
            0, -f1sq3, f1sq3,
        -f2s3,   f1s3 , f1s3,
        f1s3/L, f1s3/L, f1s3/L
    };
    
    P.mult(R);
    target.mult(P);

    if(Settings::purple()){
        Vec3 transform(1,-1,-1);
        target.mult(transform.toMatrix());
    }

    return target;
}