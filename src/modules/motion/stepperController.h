#pragma once
#include "pin.h"
#include "core/lib.h"
#include "core/Job.h"
#include "math/Geometry.h"

#define SLOW 30
#define FAST 100

class StepperController{
private:
    bool _engaged, _sleeping;
    StepControl _controller;

    Stepper _sA;
    Stepper _sB;
    Stepper _sC;

    int32_t _sA_target,
            _sB_target,
            _sC_target,
            _speed, 
            _accel;

    Vec3 _calibration;

    float _feedrate;
    bool _async = true;

    Job _currentJob;

public:
    StepperController();
    ~StepperController(){};

    Job& getCurrentJob();

    void enableAsync();
    void disableAsync();
    
    void move(Vec3 target);

    bool hasFinished();

    void reset();
    void emergencyStop();

    void setSpeed(int32_t speed);
    void setAccel(int32_t accel);
    void setCalibration(bool state);
    void setFeedrate(float feed);

    u_int32_t getAccel();
    u_int32_t getSpeed();

    Vec3 getPosition();

    bool isSleeping();
    bool isAsync();

    // Engaging motors make them ready to move. Motors may be engaged but sleeping !
    void engage();
    // Disengaging motors turn them off. They cannot move at all.
    void disengage();

    //Sleep mode is used to save battery or let the robot move freely. It disable both motors.
    //Note : A move request will exit sleep mode and turn them on.
    void wakeUp();
    void sleep();

    //Inherited
    void update();
    void pause();
    void resume();
    void cancel();
    void forceCancel();
    void complete();
};




