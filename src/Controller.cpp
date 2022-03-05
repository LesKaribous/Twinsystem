#include "Controller.h"
#include "Config.h"

namespace Controller{

    StepControl controller;
    Stepper sA(Pin::Stepper::stepA, Pin::Stepper::dirA), 
            sB(Pin::Stepper::stepB, Pin::Stepper::dirB),
            sC(Pin::Stepper::stepC, Pin::Stepper::dirC);

    bool engaged = false,
         sleeping = false;

    struct CalibrationProfile{
        float linearFactor;
        float rotationalFactor;
        float leftStepperFactor;
        float rightStepperFactor;
    } calibration;

    u_int32_t speed, accel;

    void init(){
         sA
        .setAcceleration(Config::Speed::defaultAccel)
        .setMaxSpeed(Config::Speed::defaultSpeed)
        .setInverseRotation(true);
        //.setStepPinPolarity(LOW);

        sB
        .setAcceleration(Config::Speed::defaultAccel)
        .setMaxSpeed(Config::Speed::defaultSpeed)
        .setInverseRotation(false);
        //.setStepPinPolarity(LOW);

        sC
        .setAcceleration(Config::Speed::defaultAccel)
        .setMaxSpeed(Config::Speed::defaultSpeed)
        .setInverseRotation(false);
        //.setStepPinPolarity(LOW);

            //------Déclaration des I/O------
        pinMode(Pin::Stepper::enable, OUTPUT);

        engage();
        sleep(); 
    
    }

    void setCalibration(bool state){
        if (state){
            calibration = {
                Config::Calibration::Primary::linearFactor,   
                Config::Calibration::Primary::rotationalFactor,
                Config::Calibration::Primary::leftStepperFactor,
                Config::Calibration::Primary::rightStepperFactor
            };
        }else {
            calibration = {
                Config::Calibration::Secondary::linearFactor,   
                Config::Calibration::Secondary::rotationalFactor,
                Config::Calibration::Secondary::leftStepperFactor,
                Config::Calibration::Secondary::rightStepperFactor
            };
        }
        
    }

    void engage(bool state){
        if(engaged != state){
            digitalWrite(Pin::Stepper::sleep, state);
            engaged = state;
        }
    }

    void disengage(){
        if(engaged){
            engaged = false;
            digitalWrite(Pin::Stepper::sleep, LOW);
        }
    }

    void sleep(bool state){
        if(engaged && sleeping != state){
            digitalWrite(Pin::Stepper::sleep, state);
            sleeping = state;
        }
    }


    void move(u_int32_t stepsRight, u_int32_t stepsLeft, bool abs, bool async){
            if(abs){
                sRight.setTargetAbs(stepsRight);
                sLeft.setTargetAbs(stepsLeft);
            }else{
                sRight.setTargetRel(stepsRight);
                sLeft.setTargetRel(stepsLeft);
            }

            if(sleeping) sleep(false);
            
            if(async)controller.moveAsync(sRight,sLeft);
            else controller.move(sRight,sLeft);
    }


    void turn(long steps, bool abs, bool async){
        if(engaged){
            move(steps * calibration.rotationalFactor * calibration.rightStepperFactor,
                -steps * calibration.rotationalFactor * calibration.leftStepperFactor,
                 abs, async);
        }
    }

    void go(long steps, bool abs, bool async){
        if(engaged){
            if(sleeping) sleep(false);
            move(steps * calibration.linearFactor * calibration.rightStepperFactor,
                 steps * calibration.linearFactor * calibration.leftStepperFactor,
                 abs, async);
        }
    }


    void reset(){
        sRight.setPosition(0);
        sRight.setTargetRel(0);
        sLeft.setPosition(0);
        sLeft.setTargetRel(0);
    }

    void stop(bool async){
        if(async)controller.stopAsync();
        else controller.stop();
    }

    void emergencyStop(){
        controller.emergencyStop();
        //TODO : Disengage after an emergencystop ?
    }


    void setSpeed(u_int32_t speed){
        sLeft
        .setMaxSpeed(speed);
        sRight
        .setMaxSpeed(speed);
    }

    void setAccel(u_int32_t accel){
        sLeft
        .setAcceleration(accel);
        sRight
        .setAcceleration(accel);
    }

    u_int32_t getAccel(){
        return accel;
    }

    u_int32_t getSpeed(){
        return speed;
    }

    int getCurrentSpeed(){
        return controller.getCurrentSpeed();
    }

    bool isRunning(){
        return controller.isRunning();
    }

    bool isSleeping(){
        return sleeping;
    }
}