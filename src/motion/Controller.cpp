#include "Controller.h"
#include "debug/Console.h"
#include "Twinsystem.h"

namespace Controller{

    StepControl controller;
    Stepper sA(Pin::Stepper::stepA, Pin::Stepper::dirA), 
            sB(Pin::Stepper::stepB, Pin::Stepper::dirB),
            sC(Pin::Stepper::stepC, Pin::Stepper::dirC);

    volatile int32_t sA_target,
                     sB_target,
                     sC_target;

    bool engaged = false,
         sleeping = false;

    Vec3 calibration;

    u_int32_t speed, accel;

    float feedrate = 100;
    bool wasLastAsync = false;

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

        setCalibration(Settings::primary()); //WARNING
        IHM::addLoad(10);
        IHM::setLoadingMsg("Controller OK");
    }

    void update(){}

    void setCalibration(bool state){
        calibration = (state == Settings::PRIMARY) ? Settings::Calibration::Primary.Holonomic : Settings::Calibration::Secondary.Holonomic;
    }

    void setFeedrate(float value){
        feedrate = value;
        setSpeed(float(Settings::SPEED) * value / 100.0);
    }


    void setSpeed(int s){
        speed = s;
        sA.setMaxSpeed(speed);
        sB.setMaxSpeed(speed);
        sC.setMaxSpeed(speed);
    }

    void setAccel(int a){
        accel = a;
        sA.setAcceleration(accel);
        sB.setAcceleration(accel);
        sC.setAcceleration(accel);
    }

    void engage(bool state){
        if(engaged != state){
            digitalWriteFast(Pin::Stepper::enable, state == Settings::Stepper::ENABLE_POLARITY);
            engaged = state;
        }
    }

    void disengage(){
        if(engaged){
            engaged = false;
            digitalWriteFast(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
        }
    }

    void sleep(bool state){
        if(engaged && sleeping != state){
            digitalWriteFast(Pin::Stepper::enable, state == Settings::Stepper::ENABLE_POLARITY);
            sleeping = state;
        }
    }

    void move(Vec3 target, bool async){
        wasLastAsync = async;
        target.mult(calibration.toMatrix());

        resetPosition(); //Zero all axis
        sA.setTargetAbs(int32_t(target.a));
        sB.setTargetAbs(int32_t(target.b));
        sC.setTargetAbs(int32_t(target.c));

        sA_target = int32_t(target.a);
        sB_target = int32_t(target.b);
        sC_target = int32_t(target.c);

        if(sleeping){
            sleep(false);
            System::wait(200);
        }
        if(async)controller.moveAsync(sA,sB,sC);
        else controller.move(sA,sB,sC);
        
    }

    void resetPosition(){
        sA.setPosition(0);
        sB.setPosition(0);
        sC.setPosition(0);
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

    void resume(){
        Console::info("Controler") << "Resuming..." << Console::endl;
        sA.setTargetAbs(int32_t(sA_target));
        sB.setTargetAbs(int32_t(sB_target));
        sC.setTargetAbs(int32_t(sC_target));

        if(wasLastAsync)controller.moveAsync(sA,sB,sC);
        else controller.move(sA,sB,sC);
    }

    bool arrived(){
        return sA.getPosition() == sA_target
            && sB.getPosition() == sB_target
            && sC.getPosition() == sC_target;
    }

    void emergencyStop(){
        controller.emergencyStop();
        //TODO : Disengage after an emergencystop ?
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

    Vec3 getPosition(){
        Vec3 result;
        result.a = sA.getPosition();
        result.b = sB.getPosition();
        result.c = sC.getPosition();

        result.a /= calibration.a;
        result.b /= calibration.b;
        result.c /= calibration.c;
        return result;
    }

    bool isRunning(){
        return controller.isRunning();
    }

    bool isSleeping(){
        return sleeping;
    }
}