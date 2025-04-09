#include "services/motion/motion.h"
#include "utils/geometry.h"
#include "kinematics.h"
#include "settings.h"
#include "os/console.h"
#include "motion.h"
#include "utils/timer/timer.h"
#include "services/localisation/localisation.h"
#include <cmath>

INSTANTIATE_SERVICE(Motion, motion)

Motion::Motion() : Service(ID_MOTION){
    disengage();
}

void Motion::onAttach(){
    Console::info() << "Motion activated" << Console::endl;
    
    _absolute = Settings::Motion::ABSOLUTE;

    Job::reset();
    _startPosition  = {0,0,0};
    _position       = {-1,-1,0};
    _target 	     = {0,0,0};

    _controlPoint   = {0,0};
	_absolute = true;

    pinMode(Pin::Stepper::enable, OUTPUT);

}

void Motion::onUpdate(){
    localisation.onUpdate();
    _position = localisation.getPosition();
        
    if(enabled() && isPending()){
        if(hasFinished()){
            complete();
        }
        estimatePosition();

        if(use_cruise_mode)
            cruise_controller.run();
        else 
            stepper_controller.run();
    }
}

void Motion::control(){
    if(enabled()){
        if(use_cruise_mode)
            cruise_controller.control();
        else
            stepper_controller.control();
    }
}

void Motion::enable(){
    Service::enable();
}

void Motion::disable(){
    Service::disable();
}

void Motion::engage(){
    digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
}

void Motion::disengage(){
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
}

Motion& Motion::go(float x, float y){
    //setStepsVelocity(Settings::Motion::MAX_SPEED);
    _isMoving = true;
    go(Vec2(x, y));
    return *this;
}

Motion& Motion::goPolar(float heading, float dist){
    //setStepsVelocity(Settings::Motion::MAX_SPEED);
    _isMoving = true;
    PolarVec poltarget = PolarVec(heading*DEG_TO_RAD, dist);
    if (_absolute){
        Vec2 target = _position + poltarget.toVec2();
        move(Vec3(target.a, target.b, _position.c*RAD_TO_DEG));
    }
    else{
        Vec2 reltarget = poltarget.toVec2();
        move(Vec3(reltarget.a, reltarget.b, 0));
    }
    return *this;
}

Motion& Motion::turn(float angle){
    /*
    if(_useBNO){
        _position.c = getOrientation();
    }*/
    //setStepsVelocity(Settings::Motion::TURN_SPEED);
    _isMoving = true;
    _isRotating = true;
    if (_absolute) move(Vec3(_position.a, _position.b, angle));
    else move(Vec3(0, 0, angle));
    return *this;
}
  
Motion& Motion::go(Vec2 target){
    //setStepsVelocity(Settings::Motion::MAX_SPEED);
    _isMoving = true;
    if (_absolute) move(Vec3(target.a, target.b, _position.c*RAD_TO_DEG));
    else move(Vec3(target.a, target.b, 0));
    return *this;
}

Motion&  Motion::align(RobotCompass rc, float orientation){
    turn((orientation - getCompassOrientation(rc)));
    return *this;
}

//Raw relative move request
Motion&  Motion::move(Vec3 target){ //target is in world frame of reference
    if(!enabled()) {
        Console::error("Motion") << "Motion not enabled" << Console::endl;
        return *this;
    }
    Job::reset();//Start a new job
    
    target.c *= DEG_TO_RAD; //Convert to rotation to radian
    if(isRelative()){
        if(target.magSq() == 0){ //Test if move is 0 length
            Console::error("Motion") << "Move is null" << Console::endl;
            complete();
            return *this;
        }
        target = toAbsoluteTarget(target); //convert to ABS target
    }else{
        if(target == _position){
            Console::error("Motion") << "Move is null" << Console::endl;
            complete();
            return *this;
        }
    }
    _target = target; Console::trace("Motion") << "Target is " << _target << Console::endl;

    //to relative step target 
    Console::trace("Motion") << "Position is " << _position << Console::endl;
    //Vec3 _relTarget = toRelativeTarget(_target); Console::trace("Motion") << "Relative target is " << _relTarget << Console::endl;
    //if(_optimizeRotation) _relTarget = optmizeRelTarget(_relTarget);
    //_stepsTarget = targetToSteps(_relTarget); Console::trace("Motion") << "Steps Target is " << _stepsTarget << Console::endl;
    
    Console::println("start");
    Job::start(); //robot is moving from now

    //resetSteps();
    if(use_cruise_mode){
        cruise_controller.reset();
        cruise_controller.setPosition(_position);
        cruise_controller.setTarget(_target);
    }else{
    Vec3 _relTarget = toRelativeTarget(_target);
        if(_optimizeRotation) _relTarget = optmizeRelTarget(_relTarget);
        Vec3 steps = ik(_relTarget);
        Console::println(steps);
        Console::println(_target);
        Console::println(_relTarget);
        stepper_controller.reset();
        stepper_controller.setTarget(steps.a, steps.b, steps.c);
    }


    if(m_async){
        if(use_cruise_mode) cruise_controller.start();
        else stepper_controller.start();
        Console::println("start");
    }
    else{
        if(use_cruise_mode){
            cruise_controller.start();
            Console::println("start");
            while (cruise_controller.isPending()){
                cruise_controller.run();
            }
        }else{ 
            stepper_controller.start();
            Console::println("start");
            while (stepper_controller.isPending()){
                stepper_controller.run();
            }
        }

        complete();
    }
    
    return *this;
}


void Motion::run(){
    onUpdate();
}

void Motion::pause(){
    Job::pause();
    if(use_cruise_mode){
        cruise_controller.pause();
    }else{
        stepper_controller.pause();
    }
}

void Motion::resume(){
    Job::resume();
    if(use_cruise_mode){
        cruise_controller.resume();
    }else{
        stepper_controller.resume();
    }
}

bool Motion::hasFinished() {
    if(use_cruise_mode)
        return cruise_controller.isCompleted();
    else
        return stepper_controller.isCompleted();
}

void Motion::cancel() {
    Job::cancel();
    if(Job::m_state == JobState::CANCELLED){
        if(use_cruise_mode){
            while (!cruise_controller.isCompleted()){
                cruise_controller.run();
            }
        }else{
            while (!stepper_controller.isCompleted()){
                stepper_controller.run();
            }
        }
    }

    estimatePosition();
    _isMoving = false;
    _isRotating = false;
    //_startPosition = _position = _startPosition + controller.getPosition();
    localisation.onUpdate();
    _startPosition = _position = localisation.getPosition();
    stepper_controller.reset();
    cruise_controller.reset();

    // _sA.setPosition(0);
    // _sB.setPosition(0);
    // _sC.setPosition(0);
    // _sA.setTargetAbs(0);
    // _sB.setTargetAbs(0);
    // _sC.setTargetAbs(0);
}

void Motion::complete() {
    Job::complete();
    if(use_cruise_mode){
        cruise_controller.complete();
    }else{
        stepper_controller.complete();
    }

    _isMoving = false;
    _isRotating = false;

    //_startPosition = _position = _startPosition + controller.getPosition();
    localisation.onUpdate();
    _startPosition = _position = localisation.getPosition();
    stepper_controller.reset();
    cruise_controller.reset();
    
    // _sA.setPosition(0);
    // _sB.setPosition(0);
    // _sC.setPosition(0);
    // _sA.setTargetAbs(0);
    // _sB.setTargetAbs(0);
    // _sC.setTargetAbs(0);
    Console::println("complete");
}

void Motion::forceCancel() {
    Job::cancel();
    if(Job::m_state == JobState::CANCELLED){
        cruise_controller.complete();
        stepper_controller.complete();
    }
    estimatePosition();
    _isMoving = false;
    _isRotating = false;
    //_startPosition = _position = _startPosition + controller.getPosition();
    localisation.onUpdate();
    _startPosition = _position = localisation.getPosition();
    cruise_controller.reset();
    stepper_controller.reset();

    // _sA.setPosition(0);
    // _sB.setPosition(0);
    // _sC.setPosition(0);
    // _sA.setTargetAbs(0);
    // _sB.setTargetAbs(0);
    // _sC.setTargetAbs(0);
}

Vec3 Motion::toRelativeTarget(Vec3 absTarget){
    absTarget.sub(_position); 		 //Convert  target relativeto Absolute
    absTarget.rotateZ(_position.c);
    return absTarget;
}

Vec3 Motion::toAbsoluteTarget(Vec3 relTarget){
    relTarget.rotateZ(-_position.c);
    relTarget.add(_position); 		 //Convert  target relativeto Absolute
    return relTarget;
}

bool Motion::isSleeping() const{
    return _sleeping;
}

void Motion::enableOptimization(){// Use rotation optimization (see optmizeRelTarget)
    _optimizeRotation = true;
} 
void Motion::disableOptimization(){
    // disable rotation optimization (see optmizeRelTarget)
    _optimizeRotation = false;
}

void Motion::enableCruiseMode(){
    if(isMoving())
        Console::error("Motion") << "Cruise mode cannot be toggled while moving" << Console::endl;
    use_cruise_mode = true;
}

void Motion::disableCruiseMode(){
    if(isMoving())
        Console::error("Motion") << "Cruise mode cannot be toggled while moving" << Console::endl;
    use_cruise_mode = false;
}

void  Motion::estimatePosition(){ //We are not using the estimated velocity to prevent error accumulation. We used last steps instead.

    localisation.onUpdate();
    Vec3 position = localisation.getPosition();
    if(position != _position){
        _position = position;
        cruise_controller.setPosition(localisation.getPosition());
    }
    /*
    Vec3 steps = getLastSteps(); //Read steps counter
    Vec3 delta = steps - _lastSteps;
    _lastSteps = steps;

    Vec3 angularDelta = (delta / Settings::Stepper::STEP_MODE) * (PI/200);
    Vec3 linearDelta = angularDelta * Settings::Geometry::WHEEL_RADIUS;

    //Calculate XYZ delta
    Vec3 del = fk(linearDelta);

    float orientation = _position.c;
    orientation += del.c;

    del.rotateZ(orientation); //Transform to workd space

    _position += del/_calibration;
    */
}

Vec3 Motion::optmizeRelTarget(Vec3 relTarget){
    while(relTarget.c > PI) relTarget.c -= 2.0f*PI;
    while(relTarget.c <= -PI) relTarget.c += 2.0f*PI;
    return relTarget;
}

void Motion::resetCompass(){

}

void Motion::setOrientation(float orientation){
    /*
    if(!_useBNO) return;
    while(orientation > PI) orientation-= 2.0f*PI;
    while(orientation <= -PI) orientation += 2.0f*PI;
    compassOffset -= getOrientation() - orientation;
    Console::println(compassOffset*RAD_TO_DEG);
    */
}

float Motion::getOrientation(){
    //if(!_useBNO) return _position.c;
    /*
    if(millis() - lastBNOCheck > 50){
        sensors_event_t eventAngular;
        bno.getEvent(&eventAngular, Adafruit_BNO055::VECTOR_EULER);
        float orientation = (- DEG_TO_RAD * eventAngular.orientation.x) - compassReference + compassOffset;

        orientation = fmod(orientation, TWO_PI);
        if(orientation < 0) orientation += 2.0f*PI;
        return  orientation;
    }else return _position.c;
    */
    return 0;
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

float Motion::getTargetDirection() const
{
    return 0.0f;
}

float Motion::getAbsoluteTargetDirection() const
{
    return Vec2(_target - _position).heading();
}

float Motion::getTargetDistance() const{
    return Vec2(_target - _position).mag();
}

bool  Motion::isAbsolute() const{
    return _absolute;
}

bool  Motion::isRelative() const{
    return !_absolute;
}

bool Motion::isRotating() const{
    return (Job::isPending() &&  _isRotating);
}

bool Motion::isMoving() const{
    return (Job::isPending() && _isMoving);
}

void  Motion::setAbsPosition(Vec3 newPos){
    THROW(newPos);
    _position = newPos; 
    localisation.setPosition(newPos);
    //if(_useBNO) setOrientation(newPos.c);
}

void Motion::setAbsTarget(Vec3 newTarget)
{
    _target = newTarget;
}

void  Motion::setAbsolute(){
    _absolute = true;
}

void  Motion::setRelative(){
    _absolute = false;
}

void Motion::setAsync(){
    m_async = true;
}
void Motion::setSync(){
    m_async = false;
}

void Motion::setFeedrate(float feed){
    m_feedrate = max(min(feed, 1.0), 0.1);
}

float Motion::getFeedrate() const{
    return m_feedrate;
}
