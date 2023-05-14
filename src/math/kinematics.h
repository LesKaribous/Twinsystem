#pragma once

#include "settings.h"
#include "math/geometry.h"

//Kinematics
Vec3 ik(Vec3 target){
    float c60 = cosf(M_PI/3.0f),
            s60 = sinf(M_PI/3.0f),
            L = Settings::Geometry::RADIUS,
            R = Settings::Geometry::WHEEL_RADIUS;
    
    
    Vec3 transform(-1,1,1);
    target *= transform;

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
            f2s3 = 2.0f/3.0f,
            L = Settings::Geometry::RADIUS,
            R = Settings::Geometry::WHEEL_RADIUS;
    

    Matrix3x3 P = {
            0, -f1sq3, f1sq3,
        -f2s3,   f1s3 , f1s3,
        f1s3/L, f1s3/L, f1s3/L
    };
    
    target.mult(P);
    target.mult(R);

    Vec3 transform(-1,-1,1);
    target *= transform;

    return target;
}