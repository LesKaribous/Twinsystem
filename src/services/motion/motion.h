#pragma once
#include "settings.h"
#include "services/service.h"
#include "utils/job.h"
#include "utils/geometry.h"

#include <deque>
#include <Wire.h>
#include <SPI.h>
#include <SparkFun_Qwiic_OTOS_Arduino_Library.h>

#ifdef TEENSY35
#include <TeensyStep.h>
#endif
#ifdef TEENSY41
#include <TwinsyStep.h>
#endif

class Motion : public Service, public Job{
public:

    Motion();
    // Service class methods
    void onAttach() override;
    void onUpdate() override;
    void enable() override;
    void disable() override;

    // Job class methods
    void run()override;
    void pause() override;
    void resume() override;
    void cancel() override;
    void complete() override;
    void forceCancel();

    // Power routines
    void engage();// Engaging motors make them ready to move. Motors may be engaged but sleeping !
    void disengage();// Disengaging motors turn them off. They cannot move at all.
    void wakeUp(); //Sleep mode is used to save battery or let the robot move freely. It disable both motors.
    void sleep(); //Note : A move request will exit sleep mode and turn them on.

    // Movement routines
    Motion& go(Vec2);
    Motion& go(float x, float y);
    Motion& goPolar(float angle, float dist);
    Motion& turn(float w);
    Motion& align(RobotCompass, float orientation);
    Motion& move(Vec3 target);

    void estimatePosition();
    void resetSteps();
    bool hasFinished();

    //Setters
    void setCalibration(CalibrationProfile c);
    void setAcceleration(int accel);
    void setStepsVelocity(float velocity); //feed rate will be appied automatically (see setFeedrate)
    void setAbsTarget(Vec3);    //mm, mm, rad
    void setAbsPosition(Vec3);  //mm, mm, rad
    void setAbsolute();
    void setRelative();
    void setAsync(); //Non blocking
    void setSync(); //Blocking
    void setFeedrate(float feed);


    //Getters
    Vec3 getAbsTarget() const;  //Absolute mm, mm, rad
    Vec3 getAbsPosition() const;//Absolute mm, mm, rad
    Vec3 getLastSteps() const;

    float getTargetDirection() const;
    float getAbsoluteTargetDirection() const;
    float getTargetDistance() const;
    float getFeedrate() const;

    bool isAbsolute() const;
    bool isRelative() const;
    bool isRotating() const;
    bool isSleeping() const;
    bool isMoving() const;

    // IMU
    void control();
    void openLoop();
    void closeLoop();
    void autoCalibration();

    void readIMU();
    void calibrateIMU();

    
    // Settings
    void enableOptimization(); // Use rotation optimization (see optmizeRelTarget)
    void disableOptimization();// disable rotation optimization (see optmizeRelTarget)


private :
    bool m_async = true; //non blocking by default

    Vec3 optmizeRelTarget(Vec3 relTarget);
    Vec3 targetToSteps(Vec3 relativeTarget);
    Vec3 toRelativeTarget(Vec3 absTarget);
    Vec3 toAbsoluteTarget(Vec3 absTarget);
    Vec3 computeStaturedSpeed(Vec3 targetSpeed);

    //Positions
    Vec3 _startPosition  = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _unsafePosition = {-1,-1, 0}; //Absolute mm, mm, rad : Read by IMU
    Vec3 _position       = {-1,-1, 0}; //Absolute mm, mm, rad
    Vec3 _target 	     = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _stepsTarget 	 = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 _lastSteps      = { 0, 0, 0}; //relative steps

    //Velocity
    Vec3 _targetWheelVelocity = { 0, 0, 0}; //steps/s

    //PID
    Vec3 _lastError 	  = { 0, 0, 0}; //XY, Theta
    Vec3 _integral 	      = { 0, 0, 0}; //XY, Theta

    Vec3 _calibration 	 = { 1, 1, 1};
    Vec2 _controlPoint   = { 0, 0};

    Stepper _sA, _sB, _sC;
    StepControl _steppers;
    RotateControl _sAController,
                  _sBController,
                  _sCController;

    // Settings
    float m_feedrate = 1.0;
    bool _engaged, _sleeping;
    bool _absolute = true;
    bool _isMoving = false;
    bool _isRotating = false;
    bool _optimizeRotation = true;
    bool _debug = true;
    bool _closeLoop = false;
    bool _IMU = false;
    bool _IMU_calibrated = false;

    QwiicOTOS myOtos;

    SERVICE(Motion);
};
EXTERN_DECLARATION(Motion, motion)