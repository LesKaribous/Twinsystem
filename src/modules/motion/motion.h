#pragma once
#include "core/job.h"
#include "core/module.h"
#include "math/geometry.h"
#include "modules/motion/stepperController.h"

class Motion : public Module{
public:
    Motion();
    ~Motion();

    //Inherited
    Job& getCurrentJob();

    void update();
    void pause();
    void resume();
    void cancel();
    void complete();

    void updatePosition();

    void turnAwait(float);
    void turnAsync(float);

    void goAwait(Vec2);
    void goAsync(Vec2);
    void goAwait(float x, float y);
    void goAsync(float x, float y);

    void goAlignAsync(Vec2 target, RobotCompass rc, float orientation);

    void alignAwait(RobotCompass, float orientation);
    void alignAsync(RobotCompass, float orientation);

    Vec3 optmizeRelTarget(Vec3 relTarget);
    Vec3 targetToSteps(Vec3 relativeTarget);
    Vec3 toRelativeTarget(Vec3 absTarget);
    Vec3 toAbsoluteTarget(Vec3 absTarget);

private:
    void moveAwait(Vec3 target);
    void moveAsync(Vec3 target);

public:
    //Setters

    //Absolute mm, mm, rad
    void setAbsTarget(Vec3);    //mm, mm, rad

    //Absolute mm, mm, rad
    void setAbsPosition(Vec3);  //mm, mm, rad

    void setAbsolute();
    void setRelative();

    //Getters
    //Absolute mm, mm, rad
    Vec3 getAbsPosition() const;
    //Absolute mm, mm, rad
    Vec3 getAbsTarget() const;
    //Absolute mm, mm, rad
    Vec3 getRelTarget() const;

    float getTargetDirection() const;
    float getAbsoluteTargetDirection() const;

    bool isAbsolute() const;
    bool isRelative() const;

    bool isRotating() const;
    bool isMoving() const;


public : 
    StepperController steppers;
        
private:

    Job _currentJob;

    Vec3 _startPosition  = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _position       = {-1,-1, 0}; //Absolute mm, mm, rad
    Vec3 _target 	     = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _relTarget      = { 0, 0, 0}; //Relative mm, mm, rad

    Vec3 _calibration 	 = { 1, 1, 1};
    Vec2 _controlPoint   = { 0, 0};

    bool _absolute = true;
};