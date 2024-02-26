#pragma once
#include "settings.h"
#include "system/core/service.h"
#include "system/core/lib.h"
#include "system/core/job.h"

#include <deque>



class Motion : public Service, public Job{
public:

    Motion();
    ~Motion ();

    void enable() override;
    void disable() override;

    void go(Vec2);
    void go(float x, float y);
    void goTurn(float x, float y, float theta);
    void turn(float w);
    void align(RobotCompass, float orientation);
    void goAlign(Vec2 target, RobotCompass rc, float orientation);

    void setFeedrate(int); //0-100%
    void setCalibration(CalibrationProfile c);

    void run()override;
    void pause() override;
    void resume() override;
    void update() override;
    void cancel() override;
    void complete() override;
    void forceCancel();

    
    void resetCompass(); //zero orientation
    float getOrientation(); //rad

    void resetSteps();

    //Setters
    void setAbsTarget(Vec3);    //mm, mm, rad
    void setAbsPosition(Vec3);  //mm, mm, rad

    void setAbsolute();
    void setRelative();

    //Getters
    Vec3 getAbsTarget() const;  //Absolute mm, mm, rad
    Vec3 getAbsPosition() const;//Absolute mm, mm, rad

    Vec3 getLastSteps() const;
    float getTargetDirection() const;
    float getAbsoluteTargetDirection() const;

    bool isAbsolute() const;
    bool isRelative() const;
    bool isRotating() const;
    bool isSleeping() const;
    bool isMoving() const;

    void enableOptimization(); // Use rotation optimization (see optmizeRelTarget)
    void disableOptimization();// disable rotation optimization (see optmizeRelTarget)

    void engage();// Engaging motors make them ready to move. Motors may be engaged but sleeping !
    void disengage();// Disengaging motors turn them off. They cannot move at all.

    //Sleep mode is used to save battery or let the robot move freely. It disable both motors.
    //Note : A move request will exit sleep mode and turn them on.
    void wakeUp();
    void sleep();

    void move(Vec3 target);
private:
    void positionControl(float dt);
    void speedControl(float dt);
    void estimatePosition();
    void estimateVelocity(float dt);

    Vec3 computeStaturedSpeed(Vec3 target);
    Vec3 optmizeRelTarget(Vec3 relTarget);
    Vec3 targetToSteps(Vec3 relativeTarget);
    Vec3 toRelativeTarget(Vec3 absTarget);
    Vec3 toAbsoluteTarget(Vec3 absTarget);

    float _lastStepsCount    = 0;
    Vec3 _lastStepsSum      = { 0, 0, 0}; //Steps
    std::deque<Vec3> _lastStepsHistory; //Steps

    Vec3 _startPosition  = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _position       = {-1,-1, 0}; //Absolute mm, mm, rad
    Vec3 _target 	     = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _velocity 	     = { 0, 0, 0}; //Absolute mm, mm, rad /s
    Vec3 _wheelVelocity       = { 0, 0, 0}; //Absolute steps / s
    Vec3 _targetWheelVelocity = { 0, 0, 0}; //Absolute steps / s

    Vec2 accelCorr; //Correct acceleration bias

    //PID
    Vec3 _lastError 	  = { 0, 0, 0}; //Absolute mm, mm
    Vec3 _integral 	      = { 0, 0, 0}; //Absolute mm, mm

    Vec3 _velLastError 	  = { 0, 0, 0}; //Absolute mm, mm
    Vec3 _velIntegral 	  = { 0, 0, 0}; //Absolute mm, mm

    Vec3 _calibration 	 = { 1, 1, 1};
    Vec2 _controlPoint   = { 0, 0};

    unsigned lastPIDTick = 0;
    bool _absolute = true;
    bool _optimizeRotation = true;
    bool _engaged, _sleeping;

    float compassOffset;

    //Stepper control
    Stepper _sA, _sB, _sC;
    RotateControl _sAController,
                  _sBController,
                  _sCController;

    elapsedMillis _targetTimer;
    elapsedMillis _outputTimer;

    // Check I2C device address and correct line below (by default address is 0x29 or 0x28)
    //                                   id, address
    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire2);
};