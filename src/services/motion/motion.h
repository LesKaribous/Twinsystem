#pragma once
#include "settings.h"
#include "services/service.h"
#include "utils/job.h"
#include "utils/geometry.h"

//BNO
#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BNO055.h>
//#include <utility/imumaths.h>
#include <SPI.h>

#ifdef TEENSY35
#include <TeensyStep.h>
#endif

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
    Motion& move(Vec3 target);
    
    //void setFeedrate(int); //0-100%
    void setCalibration(CalibrationProfile c);
    void setAcceleration(int accel);

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
    void setStepsVelocity(float velocity); //feed rate will be appied automatically (see setFeedrate)
    void setAbsTarget(Vec3);    //mm, mm, rad
    void setAbsPosition(Vec3);  //mm, mm, rad
    void setAbsolute();
    void setRelative();
    void setAsync(); //Non blocking
    void setSync(); //Blocking

    void setFeedrate(float feed);
    float getFeedrate() const;
    //Getters
    Vec3 getAbsTarget() const;  //Absolute mm, mm, rad
    Vec3 getAbsPosition() const;//Absolute mm, mm, rad

    Vec3 getLastSteps() const;
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


private :
    bool m_async = true; //non blocking by default
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

    float compassReference = 0;
    float compassOffset = 0;
    //float m_angular_error = 0;
    bool _useBNO = false;
    long lastBNOCheck = 0;

    /*
    Stepper _sA, _sB, _sC;
    StepControl _steppers;
    */
    float m_feedrate = 1.0;

    bool _engaged, _sleeping;
    bool _absolute = true;
    bool _isMoving = false;
    bool _isRotating = false;
    bool _optimizeRotation = true;
    bool _debug = true;

    //Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire2);

    SERVICE(Motion);
};
EXTERN_DECLARATION(Motion, motion)