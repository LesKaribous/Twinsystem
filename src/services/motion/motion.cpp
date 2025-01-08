#include "services/motion/motion.h"
#include "utils/geometry.h"
#include "settings.h"
#include "os/console.h"
#include "motion.h"
#include "utils/timer/timer.h"
#include "services/localisation/localisation.h"

#include <cmath>

INSTANTIATE_SERVICE(Motion, motion)

Motion::Motion() : Service(ID_MOTION)
    #ifdef TEENSY35
   ,_sA(Pin::Stepper::stepA, Pin::Stepper::dirA),
    _sB(Pin::Stepper::stepB, Pin::Stepper::dirB),
    _sC(Pin::Stepper::stepC, Pin::Stepper::dirC),
    controller(Settings::Motion::kP, Settings::Motion::kI, Settings::Motion::kD)
    #endif
{}

void Motion::onAttach(){
    Console::info() << "Motion activated" << Console::endl;

    pinMode(Pin::Stepper::enable, OUTPUT);
    disengage();

    #ifdef TEENSY35
    _sA.setPosition(0);
    _sB.setPosition(0);
    _sC.setPosition(0);

    _sA.setPullInOutSpeed(Settings::Motion::PULLIN, Settings::Motion::PULLIN);
    _sB.setPullInOutSpeed(Settings::Motion::PULLIN, Settings::Motion::PULLIN);
    _sC.setPullInOutSpeed(Settings::Motion::PULLIN, Settings::Motion::PULLIN);

    _sA.setInverseRotation(Settings::Stepper::DIR_A_POLARITY);
    _sB.setInverseRotation(Settings::Stepper::DIR_B_POLARITY);
    _sC.setInverseRotation(Settings::Stepper::DIR_C_POLARITY);

    _sA.setMaxSpeed(Settings::Motion::SPEED*Settings::Stepper::STEP_MODE);
    _sB.setMaxSpeed(Settings::Motion::SPEED*Settings::Stepper::STEP_MODE);
    _sC.setMaxSpeed(Settings::Motion::SPEED*Settings::Stepper::STEP_MODE);
    #endif
    setAcceleration(Settings::Motion::ACCEL);
}

// Main loop
void Motion::onUpdate(){
    /*
    if(enabled() && !_sleeping && !isPaused() && isPending()){
        if(hasFinished()){
            complete();
        }
        estimatePosition();
    }
    */
}

// Service routines
void Motion::enable(){
    Service::enable();
    engage();
    sleep();
}

void Motion::disable(){
    Service::disable();
    disengage();
}

// Power routines
void Motion::engage(){
    _engaged = true;
    _sleeping = false;
    digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
}

void Motion::disengage(){
    _engaged = false;
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
}

void Motion::wakeUp(){
    if(_sleeping){
        digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
        _sleeping = false;
    }
    delay(10);
}

void Motion::sleep(){
    //if(isPending()) cancel();
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
    _sleeping = true;
}

void Motion::setAcceleration(int accel){
    #ifdef TEENSY35
    _sA.setAcceleration(accel*Settings::Stepper::STEP_MODE);
    _sB.setAcceleration(accel*Settings::Stepper::STEP_MODE);
    _sC.setAcceleration(accel*Settings::Stepper::STEP_MODE);
    #endif
}

/*
void Motion::pause(){
    Job::pause();
    setAcceleration(Settings::Motion::STOP_DECCEL);

    #ifdef TEENSY35
    _steppers.stopAsync(); // set new speed
    #endif
}

void Motion::resume(){
    Job::resume();
    setAcceleration(Settings::Motion::ACCEL);
    #ifdef TEENSY35
    _sA.setTargetAbs(_stepsTarget.a);
    _sB.setTargetAbs(_stepsTarget.b);
    _sC.setTargetAbs(_stepsTarget.c);
    //_steppers.moveAsync(_sA, _sB, _sC); // set new speed
    #endif
}

bool Motion::hasFinished() {
    // THROW(_sA.getPosition());
    // THROW(_sB.getPosition());
    // THROW(_sC.getPosition());
    // THROW(_stepsTarget);
    #ifdef TEENSY35
    return (_sA.getPosition() == _stepsTarget.a && _sB.getPosition() == _stepsTarget.b && _sC.getPosition() == _stepsTarget.c);
    #else
    return true;
    #endif
}

void Motion::cancel() {
    Job::cancel();
    if(Job::m_state == JobState::CANCELLED){
        #ifdef TEENSY35
        _steppers.stopAsync();
        #endif
    }
    estimatePosition();
    _isMoving = false;
    _isRotating = false;
    _startPosition = _position;
    _lastSteps = _stepsTarget = Vec3(0,0,0);

    #ifdef TEENSY35
    _sA.setPosition(0);
    _sB.setPosition(0);
    _sC.setPosition(0);
    _sA.setTargetAbs(0);
    _sB.setTargetAbs(0);
    _sC.setTargetAbs(0);

    _sAController.stop();
    _sBController.stop();
    _sCController.stop();

    _sAController.overrideSpeed(0);
    _sBController.overrideSpeed(0);
    _sCController.overrideSpeed(0);
    #endif
}

void Motion::complete() {
    Job::complete();
    _isMoving = false;
    _isRotating = false;
    _startPosition = _position;// = _target;
    _lastSteps = _stepsTarget = Vec3(0,0,0);

    #ifdef TEENSY35
    _sA.setPosition(0);
    _sB.setPosition(0);
    _sC.setPosition(0);
    _sA.setTargetAbs(0);
    _sB.setTargetAbs(0);
    _sC.setTargetAbs(0);

    _sAController.stop();
    _sBController.stop();
    _sCController.stop();

    _sAController.overrideSpeed(0);
    _sBController.overrideSpeed(0);
    _sCController.overrideSpeed(0);
    #endif
    //Console::println("complete");
}

void Motion::forceCancel() {
    Job::cancel();
    if(Job::m_state == JobState::CANCELLED){
        #ifdef TEENSY35
        _steppers.emergencyStop();
        #endif
    }
    estimatePosition();
    _isMoving = false;
    _isRotating = false;
    _startPosition = _position;
    _lastSteps = _stepsTarget = Vec3(0,0,0);

    #ifdef TEENSY35
    _sA.setPosition(0);
    _sB.setPosition(0);
    _sC.setPosition(0);
    _sA.setTargetAbs(0);
    _sB.setTargetAbs(0);
    _sC.setTargetAbs(0);
    #endif
}

*/



// IMU and PID
void Motion::control(){
    
}


Vec3 Motion::computeStaturedSpeed(Vec3 targetSpeed){
    // Maximum speed constraints (in mm/s or similar units)
    float maxSpeed = Settings::Motion::SPEED;

    
    if(fabs(targetSpeed.a) < 100) targetSpeed.a = 0;
    if(fabs(targetSpeed.b) < 100) targetSpeed.b = 0;
    if(fabs(targetSpeed.c) < 100) targetSpeed.c = 0;
    

    //staturation
    float M = std::max(std::max(targetSpeed.x, targetSpeed.y), targetSpeed.z);
    if(M > maxSpeed){
        targetSpeed *= maxSpeed/M;
    }

    return targetSpeed;
}




void Motion::openLoop(){
    m_closeLoop = false;
}
void Motion::closeLoop(){
    m_closeLoop = localisation.useIMU(); //activate only if IMU available
}

void Motion::autoCalibration(){
    Serial.println("Calibrating IMU scales...");
    Serial.println("Warning robot will move during this sequence...");
    delay(2000);
    Serial.println("Not implemented...");

}












// -------------------------------
// ----- Setters and Getters -----
// -------------------------------


Vec3 Motion::getLastSteps() const{
    #ifdef TEENSY35
    Vec3 lastSteps = Vec3(  _sA.getPosition(), 
                            _sB.getPosition(), 
                            _sC.getPosition() );
    #else
    Vec3 lastSteps = Vec3(0,0,0);
    #endif

    return lastSteps;
}

void Motion::resetSteps(){
    #ifdef TEENSY35
    _sA.setPosition(0); 
    _sB.setPosition(0); 
    _sC.setPosition(0);
    #endif
}

float Motion::getMoveDirection() const{
    return 0;
}


void Motion::setVelocity(float v){
    #ifdef TEENSY35
    _sA.setMaxSpeed(min(max(m_feedrate * v*Settings::Stepper::STEP_MODE, Settings::Motion::PULLIN*Settings::Stepper::STEP_MODE), Settings::Motion::SPEED*Settings::Stepper::STEP_MODE));
    _sB.setMaxSpeed(min(max(m_feedrate * v*Settings::Stepper::STEP_MODE, Settings::Motion::PULLIN*Settings::Stepper::STEP_MODE), Settings::Motion::SPEED*Settings::Stepper::STEP_MODE));
    _sC.setMaxSpeed(min(max(m_feedrate * v*Settings::Stepper::STEP_MODE, Settings::Motion::PULLIN*Settings::Stepper::STEP_MODE), Settings::Motion::SPEED*Settings::Stepper::STEP_MODE));
    #endif
}


void Motion::setFeedrate(float feed){
    m_feedrate = max(min(feed, 1.0), 0.1);
}

float Motion::getFeedrate() const{
    return m_feedrate;
}
