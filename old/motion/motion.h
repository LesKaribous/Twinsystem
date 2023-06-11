#pragma once
#include "settings.h"
#include "system/core/job.h"
#include "system/core/module.h"
#include "system/math/geometry.h"
#include "modules/motion/stepperController.h"

class Motion : public Module, public Job{
public:

    Motion();
    ~Motion();

    void enable() override;
    void disable() override;

    void setFeedrate(int); //0-100%
    void setCalibration(CalibrationProfile c);

    void run()override;
    void pause() override;
    void resume() override;
    void update() override;
    void cancel() override;
    void complete() override;
    void forceCancel();


    Vec3 estimatePosition() const;
    Vec3 getEstimatedAbsPosition() const;

    void go(Vec2);
    void go(float x, float y);
    void turn(float w);
    void align(RobotCompass, float orientation);
    void goAlign(Vec2 target, RobotCompass rc, float orientation);

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

    Vec3 getCurrentSpeed() const; // Get the current speed in mm/s, mm/s, rad/s

    float getTargetDirection() const;
    float getAbsoluteTargetDirection() const;

    bool isAbsolute() const;
    bool isRelative() const;

    bool isRotating() const;
    bool isMoving() const;


private:
    void move(Vec3 target);
    void setSpeed(Vec3 speed); // Set the desired speed in mm/s, mm/s, rad/s
    void updateSpeed(); // Update the current speed towards the desired speed

    Vec3 optmizeRelTarget(Vec3 relTarget);
    Vec3 targetToSteps(Vec3 relativeTarget);
    Vec3 toRelativeTarget(Vec3 absTarget);
    Vec3 toAbsoluteTarget(Vec3 absTarget);

    Vec3 _desiredSpeed = { 0, 0, 0}; // Desired speed in mm/s, mm/s, rad/s
    Vec3 _currentSpeed = { 0, 0, 0}; // Current speed in mm/s, mm/s, rad/s

    Vec3 _startPosition  = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _position       = {-1,-1, 0}; //Absolute mm, mm, rad
    Vec3 _target 	     = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _relTarget      = { 0, 0, 0}; //Relative mm, mm, rad

    Vec3 _calibration 	 = { 1, 1, 1};
    Vec2 _controlPoint   = { 0, 0};

    bool _absolute = true;

    StepperController _steppers;
};