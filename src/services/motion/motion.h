#pragma once
#include "settings.h"
#include "services/service.h"
#include "utils/job.h"
#include "utils/geometry.h"
#include "pid.h"

#include <deque>



#ifdef TEENSY35
#include <TeensyStep.h>
#endif
#ifdef TEENSY41
#include <TwinsyStep.h>
#endif

class Motion : public Service{
public:

    Motion();
    // Service class methods
    void onAttach() override;
    void onUpdate() override;
    void enable() override;
    void disable() override;

    // Power routines
    void engage();// Engaging motors make them ready to move. Motors may be engaged but sleeping !
    void disengage();// Disengaging motors turn them off. They cannot move at all.
    void wakeUp(); //Sleep mode is used to save battery or let the robot move freely. It disable both motors.
    void sleep(); //Note : A move request will exit sleep mode and turn them on.



    //Setters
    void setCalibration(CalibrationProfile c);

    void setAcceleration(int accel);
    void setVelocity(float velocity); //feed rate will be appied automatically (see setFeedrate)
    
    float getFeedrate() const;
    void setFeedrate(float feed);

    void resetSteps();
    Vec3 getLastSteps() const;
    float getMoveDirection() const;

    void control();
    void openLoop();
    void closeLoop();
    void autoCalibration();
    

private :
    Vec3 computeStaturedSpeed(Vec3 targetSpeed);

    //Positions
    Vec3 m_stepsTarget 	 = { 0, 0, 0}; //Absolute mm, mm, rad
    Vec3 m_lastSteps      = { 0, 0, 0}; //relative steps

    //Velocity
    Vec3 m_targetWheelVelocity = { 0, 0, 0}; //steps/s
    PID<Vec3> controller;
    
    

    #ifdef TEENSY35
    Stepper _sA, _sB, _sC;
    StepControl _steppers;
    RotateControl _sAController,
                  _sBController,
                  _sCController;
    #endif

    // Settings
    float m_feedrate = 1.0;
    bool _engaged, _sleeping;
    bool  m_closeLoop = false;


    SERVICE(Motion);
};
EXTERN_DECLARATION(Motion, motion)