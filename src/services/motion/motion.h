#pragma once
#include "settings.h"
#include "services/service.h"
#include "utils/job.h"
#include "utils/geometry.h"
#include "positionController.h"
#include "stepperController.h"

//BNO
#include <Wire.h>
#include <SPI.h>

class Motion : public Service, public Job{
public:

    Motion();

    void onAttach() override;
    void onUpdate() override;

    void enable() override;
    void disable() override;

    void engage();// Engaging motors make them ready to move. Motors may be engaged but sleeping !
    void disengage();// Disengaging motors turn them off. They cannot move at all.

    Motion& go(Vec2);
    Motion& go(float x, float y);
    Motion& goPolar(float angle, float dist);
    Motion& turn(float w);
    Motion& align(RobotCompass, float orientation);
    Motion& move(Vec3 target);

    void run()override;
    void pause() override;
    void resume() override;
    void cancel() override;
    void complete() override;
    void forceCancel();

    void control();

    void estimatePosition();
    bool hasFinished();

    //Setters
    //void setStepsVelocity(float velocity); //feed rate will be appied automatically (see setFeedrate)
    void setAbsTarget(Vec3);    //mm, mm, rad
    void setAbsPosition(Vec3);  //mm, mm, rad
    void setAbsolute();
    void setRelative();
    void setAsync(); //Non blocking
    void setSync(); //Blocking

    void setFeedrate(float feed);
    float getFeedrate() const;

    Vec3 getAbsTarget() const;  //Absolute mm, mm, rad
    Vec3 getAbsPosition() const;//Absolute mm, mm, rad

    float getTargetDirection() const;
    float getAbsoluteTargetDirection() const;
    float getTargetDistance() const;

    void resetCompass(); //zero orientation
    float getOrientation();
    void setOrientation(float angle);
    
    bool isAbsolute() const;
    bool isRelative() const;
    bool isRotating() const;
    bool isSleeping() const;
    bool isMoving() const;

    void enableOptimization(); // Use rotation optimization (see optmizeRelTarget)
    void disableOptimization();// disable rotation optimization (see optmizeRelTarget)

    void enableCruiseMode();
    void disableCruiseMode();

private :
    bool m_async = true; //non blocking by default
    bool use_cruise_mode = false; //non blocking by default

    Vec3 optmizeRelTarget(Vec3 relTarget);
    Vec3 toRelativeTarget(Vec3 absTarget);
    Vec3 toAbsoluteTarget(Vec3 absTarget);

    Vec3 _startPosition  = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _position       = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _target 	     = { 0, 0, 0}; //Absolute mm, mm, rad

    Vec2 _controlPoint   = { 0, 0};

    PositionController cruise_controller;
    StepperController stepper_controller;
    float m_feedrate = 1.0;

    bool _engaged, _sleeping;
    bool _absolute = true;
    bool _isMoving = false;
    bool _isRotating = false;
    bool _optimizeRotation = true;
    bool _debug = true;

    SERVICE(Motion);
};
EXTERN_DECLARATION(Motion, motion)