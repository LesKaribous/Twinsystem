#pragma once

#include "kinematics.h"

//Kinematics
Vec3 ik(Vec3 target){
    float fsq3s2 = sqrt(3.0f)/2.0,
            L = Settings::Geometry::RADIUS;
        		      
    Matrix3x3 P = {
        0,   1 , L,
     -fsq3s2, -0.5, L,
      fsq3s2, -0.5, L
    };

    return target.mult(P);
}

Vec3 fk(Vec3 target){
    float   fsq3s3 = sqrt(3.0f)/3.0,		  
            f1s3 = 1.0f/3.0f,
            f2s3 = 2.0f/3.0f,
            L = Settings::Geometry::RADIUS;
    
    
    Matrix3x3 P = {
            0, -fsq3s3, fsq3s3,
        f2s3,   -f1s3 , -f1s3,
        f1s3/L, f1s3/L, f1s3/L
    };
    
    target.mult(P);
    
    return target;
}