#include "stepperController.h"
#include "settings.h"
#include "debug/Console.h"


StepperController::StepperController() :         
    _sA(Pin::Stepper::stepA, Pin::Stepper::dirA),
    _sB(Pin::Stepper::stepB, Pin::Stepper::dirB),
    _sC(Pin::Stepper::stepC, Pin::Stepper::dirC){

    pinMode(Pin::Stepper::enable, OUTPUT);

    engage();
    sleep();
    reset();    

    _sA_target = _sB_target = _sC_target = 0;
    _speed = Settings::Motion::SPEED;
    _accel = Settings::Motion::ACCEL;

    setCalibration(Settings::Match::PRIMARY); //WARNING default assisgnation

    _feedrate = 100;
    _async = true;
}



void StepperController::pause(){
    _currentJob.pause();
    if(_currentJob.isPaused()){
        _controller.stop();
        
    }
}

void StepperController::resume(){
    _currentJob.resume();
    if(!_currentJob.isPaused()){
        _sA.setTargetAbs(int32_t(_sA_target));
        _sB.setTargetAbs(int32_t(_sB_target));
        _sC.setTargetAbs(int32_t(_sC_target));

        if(_async)_controller.moveAsync(_sA,_sB,_sC);
        else _controller.move(_sA,_sB,_sC);
    }  
}

void StepperController::cancel() {
    _currentJob.cancel();
    if(_currentJob.isCancelled()){
        _controller.stop();
        complete();
    }
}

bool StepperController::hasFinished() {
    return !_controller.isRunning() && (_sA.getPosition() == _sA_target && _sB.getPosition() == _sB_target && _sC.getPosition() == _sC_target);
}

void StepperController::complete() {
    _currentJob.complete();
    
    _sA_target = 0;
    _sB_target = 0;
    _sC_target = 0;

    _sA.setPosition(0);
    _sB.setPosition(0);
    _sC.setPosition(0);
    _sA.setTargetAbs(0);
    _sB.setTargetAbs(0);
    _sC.setTargetAbs(0);
}

void  StepperController::update(){

    Console::trace("StepperController") << "_sA.getPosition() ->" << _sA.getPosition() << Console::endl;
    Console::trace("StepperController") << "_sA_target ->" << _sA_target << Console::endl;
    Console::trace("StepperController") << "_sB.getPosition() ->" << _sB.getPosition() << Console::endl;
    Console::trace("StepperController") << "_sB_target ->" << _sB_target << Console::endl;
    Console::trace("StepperController") << "_sC.getPosition() ->" << _sC.getPosition() << Console::endl;
    Console::trace("StepperController") << "_sC_target ->" << _sC_target << Console::endl;

    Console::trace("StepperController") << "HasFinished -> " << (_sA.getPosition() == _sA_target && _sB.getPosition() == _sB_target && _sC.getPosition() == _sC_target) << Console::endl;
    

    if(_currentJob.isPending()){
        if(hasFinished()){
            _currentJob.complete();
        }
    }

}


bool StepperController::isSleeping(){
    return _sleeping;
}

bool StepperController::isAsync(){
    return _async;
}

Job& StepperController::getCurrentJob(){
    return _currentJob;
}

void StepperController::enableAsync(){
    _async = true;
}
void StepperController::disableAsync(){
    _async = false;
}

void StepperController::setCalibration(bool state){
    _calibration = (state == Settings::Match::PRIMARY) ? Settings::Calibration::Primary.Holonomic : Settings::Calibration::Secondary.Holonomic;
}

void StepperController::setFeedrate(float value){
    _feedrate = value;
    Console::trace("StepperController") << "Set feedrate " << value << Console::endl;
    setSpeed( (Settings::Motion::SPEED * value) / 100.0 );
    delay(10);
}


void StepperController::setSpeed(int32_t s){
    Console::trace("StepperController") << "Set speed " << s << Console::endl;
    _speed = s;

    if(!_controller.isRunning()){
        _sA.setMaxSpeed(_speed);
        _sB.setMaxSpeed(_speed);
        _sC.setMaxSpeed(_speed);
    }else Console::error("StepperController") << __FILE__ << ":" << __LINE__ <<"Do not change Speed while running !" << s << Console::endl;

}

void StepperController::setAccel(int32_t a){
    _accel = a;
    _sA.setAcceleration(_accel);
    _sB.setAcceleration(_accel);
    _sC.setAcceleration(_accel);
}

void StepperController::engage(){
    _engaged = true;
    _sleeping = false;
    digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);

}

void StepperController::disengage(){
    _engaged = false;
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
}

void StepperController::sleep(){
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
    _sleeping = true;
}

void StepperController::wakeUp(){
    if(_sleeping){
        digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
        _sleeping = false;
    }
}

void StepperController::move(Vec3 target){
    _currentJob.start();
    target.mult(_calibration.toMatrix());

    _sA_target = int32_t(target.a);
    _sB_target = int32_t(target.b);
    _sC_target = int32_t(target.c);

    _sA.setTargetAbs(_sA_target);
    _sB.setTargetAbs(_sB_target);
    _sC.setTargetAbs(_sC_target);

    
    Console::trace("StepperController") << "_sA.getPosition() ->" << _sA.getPosition() << Console::endl;
    Console::trace("StepperController") << "_sA_target ->" << _sA_target << Console::endl;
    Console::trace("StepperController") << "_sB.getPosition() ->" << _sB.getPosition() << Console::endl;
    Console::trace("StepperController") << "_sB_target ->" << _sB_target << Console::endl;
    Console::trace("StepperController") << "_sC.getPosition() ->" << _sC.getPosition() << Console::endl;
    Console::trace("StepperController") << "_sC_target ->" << _sC_target << Console::endl;
    

    wakeUp();

    if(_async)_controller.moveAsync(_sA,_sB,_sC);
    else _controller.move(_sA,_sB,_sC);
    
}

void StepperController::reset(){

    setAccel(Settings::Motion::ACCEL);
    setSpeed(Settings::Motion::SPEED);

    _sA.setPosition(0);
    _sA.setTargetRel(0);

    _sB.setPosition(0);
    _sB.setTargetRel(0);

    _sC.setPosition(0);
    _sC.setTargetRel(0);

    _sA.setInverseRotation(Settings::Stepper::DIR_A_POLARITY)
    .setStepPinPolarity(Settings::Stepper::STEP_A_POLARITY);

    _sB.setInverseRotation(Settings::Stepper::DIR_B_POLARITY)
    .setStepPinPolarity(Settings::Stepper::STEP_B_POLARITY);

    _sC.setInverseRotation(Settings::Stepper::DIR_C_POLARITY)
    .setStepPinPolarity(Settings::Stepper::STEP_C_POLARITY);

}

void StepperController::emergencyStop(){
    _controller.emergencyStop();
    //TODO : Disengage after an emergencystop ?
} 

u_int32_t StepperController::getAccel(){
    return _accel;
}

u_int32_t StepperController::getSpeed(){
    return _speed;
}

Vec3 StepperController::getPosition(){
    Vec3 result;
    result.a = _sA.getPosition();
    result.b = _sB.getPosition();
    result.c = _sC.getPosition();

    result.a /= _calibration.a;
    result.b /= _calibration.b;
    result.c /= _calibration.c;
    return result;
}
