#include "modules/motion/motion.h"
#include "system/math/geometry.h"
#include "system/math/kinematics.h"
#include "settings.h"
#include "system/debug/console.h"


Motion::Motion() : Module(MOTION){
    _calibration = Settings::Match::PRIMARY ?  
    Settings::Calibration::Primary.Cartesian : Settings::Calibration::Secondary.Cartesian;
    
    _absolute = Settings::Motion::ABSOLUTE;
    _steppers.setFeedrate(100);

    Job::reset();
    _startPosition  = {0,0,0};
    _position       = {-1,-1,0};
    _target 	     = {0,0,0};

    _calibration 	= Settings::Calibration::Primary.Cartesian;
    _controlPoint   = {0,0};
	_absolute = true;
}

Motion::~Motion(){}

void Motion::run(){
    update();
}

void Motion::pause(){
    Job::pause();
    _steppers.pause();
}

void Motion::resume(){
    Job::resume();
    _steppers.resume();
}

void Motion::cancel() {
    Job::cancel();
    _steppers.pause();
    
    Vec3 newPos = estimatePosition();

    Console::info("Motion") << "Start position : " << _startPosition << Console::endl;
    Console::info("Motion") << "Estimate position : " << newPos << Console::endl;
    Console::info("Motion") << "Target was : " << _target << Console::endl;
    _startPosition = _position = newPos;

    _steppers.cancel();
}

void Motion::forceCancel() {
    Job::cancel();
    _steppers.emergencyStop();
    Vec3 newPos = estimatePosition();

    Console::info("Motion") << "Start position : " << _startPosition << Console::endl;
    Console::info("Motion") << "Estimate position : " << newPos << Console::endl;
    Console::info("Motion") << "Target was : " << _target << Console::endl;
    _startPosition = _position = newPos;
    _steppers.forceCancel();
}

void Motion::complete() {
    Job::complete();

    Vec3 newPos = estimatePosition();

    Console::info("Motion") << "Start position : " << _startPosition << Console::endl;
    Console::info("Motion") << "Estimate position : " << newPos << Console::endl;
    Console::info("Motion") << "Final position : " << _position << Console::endl;
    Console::info("Motion") << "Target was : " << _target << Console::endl;
    //Need to take actual rotation when adding relative target
    //_position = _startPosition.add(_target.rotateZ(-_position.c));
    _position = _target;
    _startPosition = _position;
}

void Motion::enable(){
    Module::enable();
    _steppers.engage();
}

void Motion::disable(){
    Module::disable();
    _steppers.disengage();
}

void Motion::setCalibration(CalibrationProfile c){
    _calibration = c.Cartesian;
}


void Motion::setFeedrate(int p){
    _steppers.setFeedrate(constrain(p, 0, 100));
}

void Motion::update(){
    Console::trace("Motion") << Job::toString() << Console::endl;
    _steppers.run();
    if(Job::isPending()){
        if(!_steppers.isPending()){
            complete();
        }
    }
    Console::trace("Motion") << Job::toString() << Console::endl;
}

void  Motion::go(float x, float y){
    Vec2 target = {x, y};
    go(target);
}

void  Motion::turn(float angle){
    Console::info("Motion") << "Turn :" << angle << Console::endl;

    _steppers.setFeedrate(60);
    if (_absolute) move({_position.a, _position.b, angle });
    else move({0, 0, angle});
}

void  Motion::go(Vec2 target){
    _steppers.setFeedrate(100);
    Console::info("Motion") << "Go :" << target << Console::endl;
    if (_absolute) move({target.a, target.b, _position.c*RAD_TO_DEG});
    else move({target.a, target.b, 0});
}

void Motion::goAlign(Vec2 target, RobotCompass rc, float orientation){
    float rotation = (orientation - getCompassOrientation(rc));
    _steppers.setFeedrate(100);
    Console::info("Motion") << "GoAlign :" << target << Console::endl;

    if (_absolute) move({target.a, target.b, rotation});
    else move({target.a, target.b, rotation - _position.c*RAD_TO_DEG});
}

void  Motion::align(RobotCompass rc, float orientation){
    Console::info("Motion") << "Align :" << Console::endl;
    turn((orientation - getCompassOrientation(rc)));
}

//Raw relative move request
void  Motion::move(Vec3 target){
    if(!m_enabled) {
        Console::error("Motion") << "Motion not enabled" << Console::endl;
        return;
    }
    Job::reset();
    //Set new target
    target.c *= DEG_TO_RAD;
    if(!_absolute){
        if(target.magSq() == 0){ //magSq is faster thatn mag
            Console::error("Motion") << "Move is null" << Console::endl;
            Job::cancel();
            return;
        }
        _target = toAbsoluteTarget(target);
    }else{
        if(target == _position){
            Console::error("Motion") << "Move is null" << Console::endl;
            Job::cancel();
            return;
        }
        _target = target;
    }

    //Optimize target rotation while taking actual pos into acount
    _relTarget  = toRelativeTarget(_target);
    _relTarget = optmizeRelTarget(_relTarget);

    Console::info("Motion") << "Current position : " << getAbsPosition() << Console::endl;
    Console::info("Motion") << "Current target   : " << getAbsTarget() << Console::endl;
    Console::info("Motion") << "Current relative target : " << _relTarget << Console::endl;
    
    Job::start(); //robot is moving from now

    _steppers.enableAsync();
    _steppers.move(targetToSteps(_relTarget)); //Execute move await
}

Vec3 Motion::optmizeRelTarget(Vec3 relTarget){
    while(relTarget.c > PI) relTarget.c -= 2.0f*PI;
    while(relTarget.c <= -PI) relTarget.c += 2.0f*PI;
    return relTarget;
}

Vec3 Motion::targetToSteps(Vec3 relTarget){
    relTarget *= _calibration; 					//Apply cartesian calibration
    relTarget = ik(relTarget);				  					//Apply inverse kinematics result in steps
    relTarget.mult(Settings::Stepper::STEP_MODE * RAD_TO_DEG); 	//Apply stepping multiplier
    return relTarget;
}

Vec3 Motion::toRelativeTarget(Vec3 absTarget){
    absTarget.sub(_position); 		 //Convert Absolute target to relative
    absTarget.rotateZ(_position.c);
    return absTarget;
}

Vec3 Motion::toAbsoluteTarget(Vec3 relTarget){
    relTarget.rotateZ(-_position.c);
    relTarget.add(_position); 		 //Convert  target relativeto Absolute
    return relTarget;
}


Vec3  Motion::estimatePosition() const{
    if(_steppers.getPosition().a == 0 && _steppers.getPosition().b == 0 && _steppers.getPosition().c == 0) return _position;
    Vec3 currentStepperPos = _steppers.getPosition();
    Vec3 relativePosition = fk(currentStepperPos);
    
    relativePosition.a /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;
    relativePosition.b /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;
    relativePosition.c /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;

    relativePosition.a /= _calibration.a;
    relativePosition.b /= _calibration.b;
    relativePosition.c /= _calibration.c;

    return _startPosition.copy().add(relativePosition.rotateZ(-_position.c));
}


Vec3 Motion::getEstimatedAbsPosition() const{
    return estimatePosition();
}


// -------------------------------
// ----- Setters and Getters -----
// -------------------------------

Vec3 Motion::getAbsPosition() const{
    return _position;
}

Vec3  Motion::getAbsTarget() const{
    return _target;
}

Vec3  Motion::getRelTarget() const{
    return _relTarget;
}

float Motion::getTargetDirection() const{
    return Vec2(_relTarget).heading();
}

float Motion::getAbsoluteTargetDirection() const{
    return Vec2(_relTarget).heading() - _position.c;
}

bool  Motion::isAbsolute() const{
    return _absolute;
}

bool  Motion::isRelative() const{
    return !_absolute;
}


bool Motion::isRotating() const{
    return (Job::isPending() && _relTarget.c != 0);
}

bool Motion::isMoving() const{
    return (Job::isPending() &&_target != _position);
}



void  Motion::setAbsPosition(Vec3 newPos){
    _position = newPos; 
}

void  Motion::setAbsTarget(Vec3 newTarget){
    _target = newTarget;
}


void  Motion::setAbsolute(){
    _absolute = true;
}

void  Motion::setRelative(){
    _absolute = false;
}

