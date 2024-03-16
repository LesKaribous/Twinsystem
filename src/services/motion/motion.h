#pragma once
#include "settings.h"
#include "services/service.h"
#include "utils/job.h"
#include "utils/geometry.h"

#include <TeensyStep.h>
class Motion : public Service, public Job{
public:

    Motion();

    void onAttach() override;
    void onUpdate() override;

    void enable() override;
    void disable() override;

    void engage();// Engaging motors make them ready to move. Motors may be engaged but sleeping !
    void disengage();// Disengaging motors turn them off. They cannot move at all.
    void wakeUp(); //Sleep mode is used to save battery or let the robot move freely. It disable both motors.
    void sleep(); //Note : A move request will exit sleep mode and turn them on.

    Motion& go(Vec2);
    Motion& go(float x, float y);
    Motion& goPolar(float angle, float dist);
    Motion& turn(float w);
    Motion& align(RobotCompass, float orientation);
    Motion& goAlign(Vec2 target, RobotCompass rc, float orientation);
    Motion& move(Vec3 target);
    
    //void setFeedrate(int); //0-100%
    void setCalibration(CalibrationProfile c);

    void run()override;
    void pause() override;
    void resume() override;
    void cancel() override;
    void complete() override;
    void forceCancel();

    void estimatePosition();
    void resetSteps();
    bool hasFinished();

    //Setters
    void setAbsTarget(Vec3);    //mm, mm, rad
    void setAbsPosition(Vec3);  //mm, mm, rad
    void setAbsolute();
    void setRelative();
    void setAsync(); //Non blocking
    void setSync(); //Blocking
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

    

private :
    bool m_async = false; //blocking by default
    Vec3 optmizeRelTarget(Vec3 relTarget);
    Vec3 targetToSteps(Vec3 relativeTarget);
    Vec3 toRelativeTarget(Vec3 absTarget);
    Vec3 toAbsoluteTarget(Vec3 absTarget);

    Vec3 _startPosition  = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _position       = {-1,-1, 0}; //Absolute mm, mm, rad
    Vec3 _target 	     = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _stepsTarget 	 = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _lastSteps      = { 0, 0, 0}; //relative steps

    Vec3 _calibration 	 = { 1, 1, 1};
    Vec2 _controlPoint   = { 0, 0};

    Stepper _sA, _sB, _sC;
    StepControl _steppers;
    bool _engaged, _sleeping;
    bool _absolute = true;
    bool _optimizeRotation = true;
    bool _debug = true;

    SERVICE(Motion);
};