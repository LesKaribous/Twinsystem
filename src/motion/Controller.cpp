#include "Controller.h"
#include "debug/Console.h"
#include "Twinsystem.h"



namespace TwinSystem{
    
    Controller::Controller(){

        pinMode(Pin::Stepper::enable, OUTPUT);

        Engage();
        Sleep();
        Reset();    

        sA = CreateShared<Stepper>(Pin::Stepper::stepA, Pin::Stepper::dirA); 
        sB = CreateShared<Stepper>(Pin::Stepper::stepB, Pin::Stepper::dirB);
        sC = CreateShared<Stepper>(Pin::Stepper::stepC, Pin::Stepper::dirC);

        sA_target = sB_target = sC_target = 0;
        engaged = false,
        sleeping = false;

        SetCalibration(Settings::primary()); //WARNING

        feedrate = 100;
        wasLastAsync = false;
    }

    void update(){}

    void Controller::SetCalibration(bool state){
        calibration = (state == Settings::PRIMARY) ? Settings::Calibration::Primary.Holonomic : Settings::Calibration::Secondary.Holonomic;
    }

    void Controller::SetFeedrate(float value){
        feedrate = value;
        SetSpeed(float(Settings::SPEED) * value / 100.0);
    }


    void Controller::SetSpeed(int s){
        speed = s;
        sA->setMaxSpeed(speed);
        sB->setMaxSpeed(speed);
        sC->setMaxSpeed(speed);
    }

    void Controller::SetAccel(int a){
        accel = a;
        sA->setAcceleration(accel);
        sB->setAcceleration(accel);
        sC->setAcceleration(accel);
    }

    void Controller::Engage(bool state){
        if(engaged != state){
            digitalWriteFast(Pin::Stepper::enable, state == Settings::Stepper::ENABLE_POLARITY);
            engaged = state;
        }
    }

    void Controller::Disengage(){
        if(engaged){
            engaged = false;
            digitalWriteFast(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
        }
    }

    void Controller::Sleep(bool state){
        if(engaged && sleeping != state){
            digitalWriteFast(Pin::Stepper::enable, state == Settings::Stepper::ENABLE_POLARITY);
            sleeping = state;
        }
    }

    void Controller::Move(Vec3 target, bool async){
        wasLastAsync = async;
        target.mult(calibration.toMatrix());

        ResetPosition(); //Zero all axis
        sA->setTargetAbs(int32_t(target.a));
        sB->setTargetAbs(int32_t(target.b));
        sC->setTargetAbs(int32_t(target.c));

        sA_target = int32_t(target.a);
        sB_target = int32_t(target.b);
        sC_target = int32_t(target.c);

        if(sleeping){
            Sleep(false);
            //System::wait(200);
        }
        if(async)controller.moveAsync(*sA,*sB,*sC);
        else controller.move(*sA,*sB,*sC);
        
    }

    void Controller::ResetPosition(){
        sA->setPosition(0);
        sB->setPosition(0);
        sC->setPosition(0);
    }

    void Controller::Reset(){
        sA->setPosition(0);
        sA->setTargetRel(0);

        sB->setPosition(0);
        sB->setTargetRel(0);

        sC->setPosition(0);
        sC->setTargetRel(0);

        sA->setAcceleration(Settings::ACCEL)
        .setMaxSpeed(Settings::SPEED)
        .setInverseRotation(Settings::Stepper::DIR_A_POLARITY)
        .setStepPinPolarity(Settings::Stepper::STEP_A_POLARITY);

        sB->setAcceleration(Settings::ACCEL)
        .setMaxSpeed(Settings::SPEED)
        .setInverseRotation(Settings::Stepper::DIR_B_POLARITY)
        .setStepPinPolarity(Settings::Stepper::STEP_B_POLARITY);

        sC->setAcceleration(Settings::ACCEL)
        .setMaxSpeed(Settings::SPEED)
        .setInverseRotation(Settings::Stepper::DIR_C_POLARITY)
        .setStepPinPolarity(Settings::Stepper::STEP_C_POLARITY);

    }

    void Controller::Stop(bool async){       
        if(async)controller.stopAsync();
        else controller.stop();
    }

    void Controller::Resume(){
        Console::info("Controler") << "Resuming..." << Console::endl;
        sA->setTargetAbs(int32_t(sA_target));
        sB->setTargetAbs(int32_t(sB_target));
        sC->setTargetAbs(int32_t(sC_target));

        if(wasLastAsync)controller.moveAsync(*sA,*sB,*sC);
        else controller.move(*sA,*sB,*sC);
    }

    bool Controller::isArrived(){
        return sA->getPosition() == sA_target
            && sB->getPosition() == sB_target
            && sC->getPosition() == sC_target;
    }

    void Controller::EmergencyStop(){
        controller.emergencyStop();
        //TODO : Disengage after an emergencystop ?
    } 

    u_int32_t Controller::GetAccel(){
        return accel;
    }

    u_int32_t Controller::GetSpeed(){
        return speed;
    }

    Vec3 Controller::GetPosition(){
        Vec3 result;
        result.a = sA->getPosition();
        result.b = sB->getPosition();
        result.c = sC->getPosition();

        result.a /= calibration.a;
        result.b /= calibration.b;
        result.c /= calibration.c;
        return result;
    }

    bool Controller::isRunning(){
        return controller.isRunning();
    }

    bool Controller::isSleeping(){
        return sleeping;
    }
}