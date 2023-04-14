#include "StepperController.h"
#include "debug/Console.h"
#include "Twinsystem.h"



namespace TwinSystem{
    
    StepperController::StepperController() :         
        sA(Pin::Stepper::stepA, Pin::Stepper::dirA),
        sB(Pin::Stepper::stepB, Pin::Stepper::dirB),
        sC(Pin::Stepper::stepC, Pin::Stepper::dirC){}

    void StepperController::Initialize(){
        if(!_initialized){
            _initialized = true;
            pinMode(Pin::Stepper::enable, OUTPUT);

            Engage();
            Sleep();
            Reset();    

            sA_target = sB_target = sC_target = 0;
            engaged = false,
            sleeping = false;

            SetCalibration(Settings::Match::PRIMARY); //WARNING default assisgnation

            feedrate = 100;
            wasLastAsync = false;
        }
    }

	void StepperController::OnEvent(Event& e){
		//EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		//dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));
	}

    void update(){}

    void StepperController::SetCalibration(bool state){
        calibration = (state == Settings::Match::PRIMARY) ? Settings::Calibration::Primary.Holonomic : Settings::Calibration::Secondary.Holonomic;
    }

    void StepperController::SetFeedrate(float value){
        feedrate = value;
        SetSpeed(float(Settings::Motion::SPEED) * value / 100.0);
    }


    void StepperController::SetSpeed(int s){
        speed = s;
        sA.setMaxSpeed(speed);
        sB.setMaxSpeed(speed);
        sC.setMaxSpeed(speed);
    }

    void StepperController::SetAccel(int a){
        accel = a;
        sA.setAcceleration(accel);
        sB.setAcceleration(accel);
        sC.setAcceleration(accel);
    }

    void StepperController::Engage(bool state){
        if(engaged != state){
            digitalWriteFast(Pin::Stepper::enable, state == Settings::Stepper::ENABLE_POLARITY);
            engaged = state;
        }
    }

    void StepperController::Disengage(){
        if(engaged){
            engaged = false;
            digitalWriteFast(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
        }
    }

    void StepperController::Sleep(bool state){
        if(engaged && sleeping != state){
            digitalWriteFast(Pin::Stepper::enable, state == Settings::Stepper::ENABLE_POLARITY);
            sleeping = state;
        }
    }

    void StepperController::Move(Vec3 target, bool async){
        wasLastAsync = async;
        target.mult(calibration.toMatrix());

        ResetPosition(); //Zero all axis
        sA.setTargetAbs(int32_t(target.a));
        sB.setTargetAbs(int32_t(target.c));
        sC.setTargetAbs(int32_t(target.b));

        sA_target = int32_t(target.a);
        sB_target = int32_t(target.c);
        sC_target = int32_t(target.b);

        if(sleeping){
            Sleep(false);
            //System::wait(200);
        }
        if(async)controller.moveAsync(sA,sB,sC);
        else controller.move(sA,sB,sC);
        
    }

    void StepperController::ResetPosition(){
        sA.setPosition(0);
        sB.setPosition(0);
        sC.setPosition(0);
    }

    void StepperController::Reset(){
        sA.setPosition(0);
        sA.setTargetRel(0);

        sB.setPosition(0);
        sB.setTargetRel(0);

        sC.setPosition(0);
        sC.setTargetRel(0);

        sA.setAcceleration(Settings::Motion::ACCEL)
        .setMaxSpeed(Settings::Motion::SPEED)
        .setInverseRotation(Settings::Stepper::DIR_A_POLARITY)
        .setStepPinPolarity(Settings::Stepper::STEP_A_POLARITY);

        sB.setAcceleration(Settings::Motion::ACCEL)
        .setMaxSpeed(Settings::Motion::SPEED)
        .setInverseRotation(Settings::Stepper::DIR_B_POLARITY)
        .setStepPinPolarity(Settings::Stepper::STEP_B_POLARITY);

        sC.setAcceleration(Settings::Motion::ACCEL)
        .setMaxSpeed(Settings::Motion::SPEED)
        .setInverseRotation(Settings::Stepper::DIR_C_POLARITY)
        .setStepPinPolarity(Settings::Stepper::STEP_C_POLARITY);

    }

    void StepperController::Stop(bool async){       
        if(async)controller.stopAsync();
        else controller.stop();
    }

    void StepperController::Resume(){
        Console::info("Controler") << "Resuming..." << Console::endl;
        sA.setTargetAbs(int32_t(sA_target));
        sB.setTargetAbs(int32_t(sB_target));
        sC.setTargetAbs(int32_t(sC_target));

        if(wasLastAsync)controller.moveAsync(sA,sB,sC);
        else controller.move(sA,sB,sC);
    }

    bool StepperController::isArrived(){
        return sA.getPosition() == sA_target
            && sB.getPosition() == sB_target
            && sC.getPosition() == sC_target;
    }

    void StepperController::EmergencyStop(){
        controller.emergencyStop();
        //TODO : Disengage after an emergencystop ?
    } 

    u_int32_t StepperController::GetAccel(){
        return accel;
    }

    u_int32_t StepperController::GetSpeed(){
        return speed;
    }

    Vec3 StepperController::GetPosition(){
        Vec3 result;
        result.a = sA.getPosition();
        result.b = sB.getPosition();
        result.c = sC.getPosition();

        result.a /= calibration.a;
        result.b /= calibration.b;
        result.c /= calibration.c;
        return result;
    }

    bool StepperController::isRunning(){
        return controller.isRunning();
    }

    bool StepperController::isSleeping(){
        return sleeping;
    }
}