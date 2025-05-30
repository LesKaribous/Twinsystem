#include "services/motion/motion.h"
#include "utils/geometry.h"
#include "kinematics.h"
#include "settings.h"
#include "os/console.h"
#include "motion.h"
#include "utils/timer/timer.h"
#include "services/localisation/localisation.h"
#include <cmath>

SINGLETON_INSTANTIATE(Motion, motion)

Motion::Motion() : Service(ID_MOTION),
m_sA(Pin::Stepper::stepA, Pin::Stepper::dirA, !Settings::Stepper::DIR_A_POLARITY), 
m_sB(Pin::Stepper::stepB, Pin::Stepper::dirB, !Settings::Stepper::DIR_B_POLARITY),  
m_sC(Pin::Stepper::stepC, Pin::Stepper::dirC, !Settings::Stepper::DIR_C_POLARITY){
    disengage();
}

void Motion::attach(){
    Console::info() << "Motion activated" << Console::endl;

    Job::reset();

    stepper_controller.setSteppers(&m_sA, &m_sB, &m_sC);
    cruise_controller.setSteppers(&m_sA, &m_sB, &m_sC);
    
    _absolute = Settings::Motion::ABSOLUTE;

    _startPosition  = {0,0,0};
    _position       = {-1,-1,0};
    _target 	     = {0,0,0};

    _controlPoint   = {0,0};
	_absolute = true;
    cancelOnCollide(true);
    pinMode(Pin::Stepper::enable, OUTPUT);
}

void Motion::onRunning(){
    if(isBusy()){
        if(hasFinished()){
            complete();
        }
        
        bool canceled = false;
        if(current_move_cruised) canceled = cruise_controller.isCanceled();
        if(stepper_controller) canceled = stepper_controller.isCanceled();
        if(canceled){
            Console::error("Motion") << "Move command was canceled" << Console::endl; 
            onCanceled();
        }

        /*
        RUN_EVERY(
        Console::info("cruise_controller.collision()") << cruise_controller.collision() << Console::endl;
        Console::info("use_cancel_on_collide") << use_cancel_on_collide << Console::endl;
        ,100)
        /**/

        if( use_cancel_on_collide && cruise_controller.collision()) cruise_controller.cancel();

        /*
        if(current_move_cruised)
            cruise_controller.run();
        else 
            stepper_controller.run();
        */
    }
}

void Motion::onPausing()
{
    bool finished = false;

    if(current_move_cruised){
        Vec3 position = estimatedPosition();
        if(position != _position && current_move_cruised){
            _position = position;
            cruise_controller.setPosition(position);
        }
        cruise_controller.exec();
        finished = cruise_controller.isCanceled();
    }else{
        stepper_controller.exec();
        finished = stepper_controller.isCanceled();
    }

    if(finished) onPaused();
}

void Motion::onCanceling(){
    bool finished = false;

    if(current_move_cruised){
        Vec3 position = estimatedPosition();
        if(position != _position && current_move_cruised){
            _position = position;
            cruise_controller.setPosition(position);
        }
        cruise_controller.exec();
        finished = cruise_controller.isCompleted();
    }else{
        stepper_controller.exec();
        finished = stepper_controller.isCompleted();
    }

    if(finished) onCanceled();
}

void Motion::onPaused(){
    //if\(!hasJob\(\)\) return;
    Job::onPaused();
    _isMoving = false;
    _isRotating = false;
    Console::info("Motion") << "Move paused successfully" << Console::endl;

    cruise_controller.reset();
    stepper_controller.reset();

    _startPosition = _position = estimatedPosition();
}

void Motion::onCanceled(){
    //if\(!hasJob\(\)\) return;
    Job::onCanceled();
    _isMoving = false;
    _isRotating = false;

    _startPosition = _position = estimatedPosition();

    cruise_controller.reset();
    stepper_controller.reset();
    
    Console::info("Motion") << "Move cancelled successfully" << Console::endl;
}

void Motion::control(){
    if(enabled()){
        /*
        RUN_EVERY(
            Vec3 position = estimatedPosition();
            if(position != _position && current_move_cruised){
                _position = position;
                cruise_controller.setPosition(position);
            }
        , Settings::Motion::PID_MIN_INTERVAL);
        */

        if(current_move_cruised)
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
    if(_engaged) return;
    _engaged = true;
    digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
}

void Motion::disengage(){
    if(!_engaged) return;
    _engaged = false;
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
}

Motion& Motion::go(float x, float y){
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

Motion &Motion::goPolarAlign(float heading, float dist, RobotCompass rc, float orientation){
    //setStepsVelocity(Settings::Motion::MAX_SPEED);
    _isMoving = true;
    PolarVec poltarget = PolarVec(heading*DEG_TO_RAD, dist);
    float angle = (orientation - getCompassOrientation(rc));

    if (_absolute){
        Vec2 target = _position + poltarget.toVec2();
        move(Vec3(target.a, target.b, angle));
        
    }
    else{
        Vec2 reltarget = poltarget.toVec2();
        move(Vec3(reltarget.a, reltarget.b, angle));
        
    }
    return *this;
}

Motion& Motion::turn(float angle){
    /*
    if(_useBNO){
        _position.c = getOrientation();
    }*/
    //setStepsVelocity(Settings::Motion::TURN_SPEED);
    //TODO push Motion properties
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

Motion&  Motion::goAlign(Vec2 target, RobotCompass rc, float orientation){
    _isMoving = true;
    _isRotating = true;

    float angle = (orientation - getCompassOrientation(rc));

    if (_absolute) move(Vec3(target.a, target.b, angle));
    else move(Vec3(target.a, target.b, angle));

    return *this;
}

Motion&  Motion::move(Vec3 target){ //target is in world frame of reference
    if(!enabled()) {
        Console::error("Motion") << "Motion not enabled" << Console::endl;
        return *this;
    }
    if(isRunning()) cancel();
    Job::reset();//Start a new job

    engage();

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
    _target = target; Console::info("Motion") << "Target is " << _target << Console::endl;

    //to relative step target 
    Console::info("Motion") << "Position is " << _position << Console::endl;
    //Vec3 _relTarget = toRelativeTarget(_target); Console::trace("Motion") << "Relative target is " << _relTarget << Console::endl;
    //if(_optimizeRotation) _relTarget = optmizeRelTarget(_relTarget);
    //_stepsTarget = targetToSteps(_relTarget); Console::trace("Motion") << "Steps Target is " << _stepsTarget << Console::endl;
    Vec3 _relTarget = toRelativeTarget(_target);

    //resetSteps();
    if(true || use_cruise_mode && _relTarget.mag() > Settings::Motion::MIN_CRUISE_DISTANCE && localisation.enabled()){
        //Cuise mode
        cruise_controller.reset();
        stepper_controller.reset();
        cruise_controller.setFeedrate( isRotating() ? m_feedrate * 0.5 : m_feedrate);
        cruise_controller.setPosition(_position);
        cruise_controller.setTarget(_target);
        current_move_cruised = true;
    }else{
        //Stepper Controller
        current_move_cruised = false;
        if(_optimizeRotation) _relTarget = optmizeRelTarget(_relTarget);
        Vec3 steps = ik(_relTarget);

        Console::info("Motion") << "New move :" << "steps(" << steps << "), target(" << _target << "), relTarget(" << _relTarget << ")" << Console::endl;
        
        cruise_controller.reset();
        stepper_controller.reset();
        stepper_controller.setFeedrate( isRotating() ? m_feedrate * 0.5 : m_feedrate);
        stepper_controller.setTarget(steps.a, steps.b, steps.c);
        
    }

    start();

    return *this;
}

void Motion::run(){
    if(!enabled()) return;
    if(isPausing()){
        onPausing();
    }else if(isCanceling()){
        onCanceling();
    }else if(isRunning()){
        onRunning();
    }
}

void Motion::exec(){
    if(!enabled()) return;

    if(isPausing()){
        onPausing();
    }else if(isCanceling()){
        onCanceling();
    }else if(isRunning()){
        onRunning();
    }
}

void Motion::start(){
    //if\(!hasJob\(\)\) return;
    //if(isPending()) forceCancel();
    if(isPending()) return;

    Job::start();
    if(m_async){
        if(current_move_cruised) cruise_controller.start();
        else stepper_controller.start();
        Console::println(String("start motion ") + String(current_move_cruised ? "(cruised)" : ""));
    }
    else{
        if(current_move_cruised){
            cruise_controller.start();
            Console::println(String("start motion ") + String(current_move_cruised ? "(cruised)" : ""));
            while (cruise_controller.isBusy()){
                cruise_controller.exec();
            }
        }else{ 
            stepper_controller.start();
            Console::println(String("start motion ") + String(current_move_cruised ? "(cruised)" : ""));
            while (stepper_controller.isBusy()){
                stepper_controller.exec();
            }
        }

        complete();
    }
}

void Motion::pause(){
    //if\(!hasJob\(\)\) return;
    Job::pause();
    if(!Job::isPausing()) return;

    if(current_move_cruised){
        cruise_controller.cancel();
    }else{
        stepper_controller.cancel();
    }
}

void Motion::resume(){
    //if\(!hasJob\(\)\) return;
    if(!Job::isPaused()) return;
    Job::resume();

    if(current_move_cruised){
        Vec3 pos = estimatedPosition();
        cruise_controller.reset();
        stepper_controller.reset();
        cruise_controller.setPosition(pos);
        cruise_controller.setFeedrate(m_feedrate);
        cruise_controller.setTarget(_target);
        

        if(m_async){
            cruise_controller.start();
            Console::println("start");
        }
        else{
            cruise_controller.start();
            Console::println("start");
            while (cruise_controller.isPending()){
                cruise_controller.exec();
            }
            complete();
        }
    }else{
        _position = estimatedPosition();
        cruise_controller.reset();
        stepper_controller.reset();

        Vec3 _relTarget = toRelativeTarget(_target);
        if(_optimizeRotation) _relTarget = optmizeRelTarget(_relTarget);
        Vec3 steps = ik(_relTarget);
        stepper_controller.setFeedrate(m_feedrate);
        stepper_controller.setTarget(steps.a, steps.b, steps.c);
        
        if(m_async){
            stepper_controller.start();
            Console::println("resume");
        }else{
            stepper_controller.start();
            Console::println("resume");
            while (stepper_controller.isPending()){
                stepper_controller.exec();
            }
            complete();
        }
    }
}

Vec3 Motion::estimatedPosition(){

    if(localisation.enabled()){
        localisation.run();
        return localisation.getPosition();
    }else{
        return _position + stepper_controller.getDisplacement();
    }

}

bool Motion::hasFinished() {
    if(current_move_cruised)
        return cruise_controller.isCompleted();
    else
        return stepper_controller.isCompleted();
}

void Motion::cancel() {
    //if\(!hasJob\(\)\) return;
    if(isCanceling() || isCanceled() || isCompleted()) return;
    Job::cancel();
    if(isCanceling()){
        if(current_move_cruised){
            cruise_controller.cancel();
        }else{
            stepper_controller.cancel();
        }
    }
}

void Motion::complete() {
    _isMoving = false;
    _isRotating = false;

    _startPosition = _position = estimatedPosition();

    cruise_controller.reset();
    stepper_controller.reset();
    
    //if\(!hasJob\(\)\) return;
    Job::complete();
    Console::println("complete");
}



void Motion::forceCancel() {
    _isMoving = false;
    _isRotating = false;

    _startPosition = _position = estimatedPosition();

    cruise_controller.reset();
    stepper_controller.reset();

    //if\(!hasJob\(\)\) return;
    Job::forceCancel();
    Console::println("canceled");
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

void Motion::cancelOnCollide(bool state){
    use_cancel_on_collide = state;
}

Vec3 Motion::optmizeRelTarget(Vec3 relTarget){
    while(relTarget.c > PI) relTarget.c -= 2.0f*PI;
    while(relTarget.c <= -PI) relTarget.c += 2.0f*PI;
    return relTarget;
}















// -------------------------------
// ----- Setters and Getters -----
// -------------------------------

void Motion::setOrientation(float orientation){
    _position.c = orientation;
}

float Motion::getOrientation(){
    return _position.c;
}

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
    return (_isRotating);
}

bool Motion::isMoving() const{
    return (_isMoving);
}

void  Motion::setAbsPosition(Vec3 newPos){
    _position = newPos; 
    _startPosition = newPos;
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
    m_feedrate = max(min(feed, 1.0), 0.05);
}

float Motion::getFeedrate() const{
    return m_feedrate;
}
