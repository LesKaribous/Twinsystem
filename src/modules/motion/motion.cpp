#include "modules/motion/motion.h"
#include "math/geometry.h"
#include "math/kinematics.h"
#include "settings.h"
#include "debug/console.h"


Motion::Motion() : Module(MOTION){
    _calibration = Settings::Match::PRIMARY ?  
    Settings::Calibration::Primary.Cartesian : Settings::Calibration::Secondary.Cartesian;
    
    _absolute = Settings::Motion::ABSOLUTE;
    steppers.setFeedrate(100);

    _currentJob.reset();
    _startPosition  = {0,0,0};
    _position       = {-1,-1,0};
    _target 	     = {0,0,0};

    _calibration 	= Settings::Calibration::Primary.Cartesian;
    _controlPoint   = {0,0};
	_absolute = true;
}

Motion::~Motion(){}

void Motion::pause(){
    _currentJob.pause();
    steppers.pause();
}

void Motion::resume(){
    _currentJob.resume();
    steppers.resume();
}

void Motion::cancel() {
    _currentJob.cancel();
    steppers.pause();
    
    Vec3 newPos = estimatePosition();

    Console::info("Motion") << "Start position : " << _startPosition << Console::endl;
    Console::info("Motion") << "Estimate position : " << newPos << Console::endl;
    Console::info("Motion") << "Target was : " << _target << Console::endl;
    _startPosition = _position = newPos;

    steppers.cancel();
}

void Motion::forceCancel() {
    _currentJob.cancel();
    steppers.emergencyStop();
    Vec3 newPos = estimatePosition();

    Console::info("Motion") << "Start position : " << _startPosition << Console::endl;
    Console::info("Motion") << "Estimate position : " << newPos << Console::endl;
    Console::info("Motion") << "Target was : " << _target << Console::endl;
    _startPosition = _position = newPos;
    steppers.forceCancel();
}

void Motion::complete() {
    _currentJob.complete();

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

Job& Motion::getCurrentJob(){
    return _currentJob;
}

void Motion::enable(){
    Module::enable();
    steppers.engage();
}

void Motion::disable(){
    Module::disable();
    steppers.disengage();
}

void Motion::setCalibration(CalibrationProfile c){
    _calibration = c.Cartesian;
}


void Motion::setFeedrate(int p){
    steppers.setFeedrate(constrain(p, 0, 100));
}

void Motion::update(){
    Console::trace("Motion") << _currentJob.toString() << Console::endl;
    steppers.update();
    if(_currentJob.isPending()){
        if(!steppers.getCurrentJob().isPending()){
            complete();
        }
    }
    Console::trace("Motion") << _currentJob.toString() << Console::endl;
}

void  Motion::goAwait(float x, float y){
    Vec2 target = {x, y};
    goAwait(target);
}

void  Motion::goAsync(float x, float y){
    Vec2 target = {x, y};
    goAsync(target);
}

void  Motion::turnAwait(float angle){
    Console::info("Motion") << "Turn :" << angle << Console::endl;

    steppers.setFeedrate(60);
    if (_absolute) moveAwait({_position.a, _position.b, angle });
    else moveAwait({0, 0, angle});
}

void  Motion::turnAsync(float angle){
    Console::info("Motion") << "Turn :" << angle << Console::endl;

    steppers.setFeedrate(60);
    if (_absolute) moveAsync({_position.a, _position.b, angle });
    else moveAsync({0, 0, angle});
}

void  Motion::goAwait(Vec2 target){
    steppers.setFeedrate(100);
    Console::info("Motion") << "Go :" << target << Console::endl;
    if (_absolute) moveAwait({target.a, target.b, _position.c*RAD_TO_DEG});
    else moveAwait({target.a, target.b, 0});
}

void  Motion::goAsync(Vec2 target){
    steppers.setFeedrate(100);
    Console::info("Motion") << "Go :" << target << Console::endl;
    if (_absolute) moveAsync({target.a, target.b, _position.c*RAD_TO_DEG});
    else moveAsync({target.a, target.b, 0});
}


void Motion::goAlignAsync(Vec2 target, RobotCompass rc, float orientation){
    float rotation = (orientation - getCompassOrientation(rc));
    steppers.setFeedrate(100);
    Console::info("Motion") << "GoAlign :" << target << Console::endl;

    if (_absolute) moveAsync({target.a, target.b, rotation});
    else moveAsync({target.a, target.b, rotation - _position.c*RAD_TO_DEG});
}

void  Motion::alignAwait(RobotCompass rc, float orientation){
    Console::info("Motion") << "Align :" << Console::endl;
    turnAwait((orientation - getCompassOrientation(rc)));
}
void  Motion::alignAsync(RobotCompass rc, float orientation){
    Console::info("Motion") << "Align :" << Console::endl;
    turnAsync( orientation - getCompassOrientation(rc));
}


//Raw relative move request
void  Motion::moveAwait(Vec3 target){
    _currentJob.reset();
    //Set new target
    target.c *= DEG_TO_RAD;
    
    if(!_absolute){
        if(target.magSq() == 0){//magSq is faster thatn mag
            Console::error("Motion") << "Move is null" << Console::endl;
            _currentJob.complete(); //robot is null
            return;
        }
        _target = toAbsoluteTarget(target);
    }else{
        if(target == _position){
            Console::error("Motion") << "Move is null" << Console::endl;
            _currentJob.complete(); //robot is null
            return;
        }
        _target = target;
    }

    _relTarget  = toRelativeTarget(_target);
    _relTarget = optmizeRelTarget(_relTarget);

    Console::info("Motion") << "Current position : " << getAbsPosition() << Console::endl;
    Console::info("Motion") << "Current target   : " << getAbsTarget() << Console::endl;
    Console::info("Motion") << "Current relative target : " << _relTarget << Console::endl;
    
    _currentJob.start(); //robot is moving from now

    steppers.disableAsync();
    steppers.move(targetToSteps(_relTarget)); //Execute move await
    complete();
}

//Raw relative move request
void  Motion::moveAsync(Vec3 target){
    _currentJob.reset();
    //Set new target
    target.c *= DEG_TO_RAD;
    if(!_absolute){
        if(target.magSq() == 0){ //magSq is faster thatn mag
            Console::error("Motion") << "Move is null" << Console::endl;
            _currentJob.cancel();
            return;
        }
        _target = toAbsoluteTarget(target);
    }else{
        if(target == _position){
            Console::error("Motion") << "Move is null" << Console::endl;
            _currentJob.cancel();
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
    
    _currentJob.start(); //robot is moving from now

    steppers.enableAsync();
    steppers.move(targetToSteps(_relTarget)); //Execute move await
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
    if(steppers.getPosition().a == 0 && steppers.getPosition().b == 0 && steppers.getPosition().c == 0) return _position;
    Vec3 currentStepperPos = steppers.getPosition();
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
    return (_currentJob.isPending() && _relTarget.c != 0);
}

bool Motion::isMoving() const{
    return (_currentJob.isPending() &&_target != _position);
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

