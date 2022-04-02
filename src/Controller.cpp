#include "Controller.h"
#include "Settings.h"
#include "Debugger.h"

namespace Controller{

    StepControl controller;
    Stepper sA(Pin::Stepper::stepA, Pin::Stepper::dirA), 
            sB(Pin::Stepper::stepB, Pin::Stepper::dirB),
            sC(Pin::Stepper::stepC, Pin::Stepper::dirC);

    bool engaged = false,
         sleeping = false;

    CalibrationProfile calibration;

    u_int32_t speed, accel;

    void init(){

            //------Déclaration des I/O------
        pinMode(Pin::Stepper::enable, OUTPUT);
        engage();
        sleep();

            //------Déclaration des I/O------
        pinMode(Pin::Stepper::stepA, OUTPUT);
        pinMode(Pin::Stepper::stepB, OUTPUT);
        pinMode(Pin::Stepper::stepC, OUTPUT);

        pinMode(Pin::Stepper::dirA, OUTPUT);
        pinMode(Pin::Stepper::dirB, OUTPUT);
        pinMode(Pin::Stepper::dirC, OUTPUT);

        reset();    

        setCalibration(true); //Primary WARNING

    }

    void setCalibration(bool state){
        if (state == Settings::PRIMARY){
            calibration = Settings::Calibration::Primary;
        }else {
            calibration = Settings::Calibration::Secondary;
        }
    }

    void engage(bool state){
        if(engaged != state){
            digitalWrite(Pin::Stepper::enable, state == Settings::Stepper::ENABLE_POLARITY);
            engaged = state;
        }
    }

    void disengage(){
        if(engaged){
            engaged = false;
            digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
        }
    }

    void sleep(bool state){
        if(engaged && sleeping != state){
            digitalWrite(Pin::Stepper::enable, state == Settings::Stepper::ENABLE_POLARITY);
            sleeping = state;
        }
    }


    void move(Vec3 target, bool async){

        /*
        target.mult(calibration.Holonomic.toMatrix());

        Debugger::log("Calibration:");
		Debugger::log(target.a);
		Debugger::log(target.b);
		Debugger::log(target.c);
        */

        sA.setTargetRel(int32_t(target.a));
        sB.setTargetRel(int32_t(target.b));
        sC.setTargetRel(int32_t(target.c));

        if(sleeping) sleep(false);
        
        if(async)controller.moveAsync(sA,sB,sC);
        else controller.move(sA,sB,sC);
    }

    void reset(){
        sA.setPosition(0);
        sA.setTargetRel(0);

        sB.setPosition(0);
        sB.setTargetRel(0);

        sC.setPosition(0);
        sC.setTargetRel(0);

        sA
        .setAcceleration(Settings::ACCEL)
        .setMaxSpeed(Settings::SPEED)
        .setInverseRotation(Settings::Stepper::DIR_A_POLARITY)
        .setStepPinPolarity(Settings::Stepper::STEP_A_POLARITY);

        sB
        .setAcceleration(Settings::ACCEL)
        .setMaxSpeed(Settings::SPEED)
        .setInverseRotation(Settings::Stepper::DIR_B_POLARITY)
        .setStepPinPolarity(Settings::Stepper::STEP_B_POLARITY);

        sC
        .setAcceleration(Settings::ACCEL)
        .setMaxSpeed(Settings::SPEED)
        .setInverseRotation(Settings::Stepper::DIR_C_POLARITY)
        .setStepPinPolarity(Settings::Stepper::STEP_C_POLARITY);

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
        sA.setMaxSpeed(speed);
        sB.setMaxSpeed(speed);
        sC.setMaxSpeed(speed);
    }

    void setAccel(u_int32_t accel){
        sA.setAcceleration(accel);
        sB.setAcceleration(accel);
        sC.setAcceleration(accel);
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