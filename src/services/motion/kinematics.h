#pragma once

#include "settings.h"
#include "system/math/geometry.h"

//Kinematics
Vec3 ik(Vec3 target){
    float fsq3s2 = sqrt(3.0f)/2.0,
            L = Settings::Geometry::RADIUS,
            R = Settings::Geometry::WHEEL_RADIUS;  
        		  
    Vec3 transform(1,1,1);
    target *= transform;

    
    Matrix3x3 P = {
        0,   -1 , L,
       fsq3s2, 0.5, L,
      -fsq3s2, 0.5, L
    };

    return target.mult(P).mult(1/R);
}

Vec3 fk(Vec3 target){
    float   f1sq3 = 1.0f / sqrt(3.0f),		  
            //fsq3s3 = sqrt(3.0f)/3.0,		  
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

    Vec3 transform(1,1,1);
    target *= transform;

    return target;
}