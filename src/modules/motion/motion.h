#pragma once
#include "core/module.h"
#include "math/geometry.h"

class Motion : public Module{
public:
    Motion();
    ~Motion();

    void update() override;
        
private:

    Vec3 _startPosition  = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _position       = {-1,-1, 0}; //Absolute mm, mm, rad
    Vec3 _target 	     = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _relTarget      = { 0, 0, 0}; //Relative mm, mm, rad

    Vec3 _calibration 	 = { 1, 1, 1};
    Vec2 _controlPoint   = { 0, 0};

    bool _absolute = true;
};

Motion::Motion() : Module(MOTION){
}

Motion::~Motion(){
}

void Motion::update(){
    _position += Vec3(1,-1,0);
}
