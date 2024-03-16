#include "services/motion/motion.h"
#include "utils/geometry.h"
#include "kinematics.h"
#include "settings.h"
#include "console.h"
#include "motion.h"

INSTANTIATE_SERVICE(Motion)

Motion::Motion() : Service(ID_MOTION),         
    _sA(Pin::Stepper::stepA, Pin::Stepper::dirA),
    _sB(Pin::Stepper::stepB, Pin::Stepper::dirB),
    _sC(Pin::Stepper::stepC, Pin::Stepper::dirC){
}


void Motion::onAttach(){
    Console::info() << "Motion activated" << Console::endl;
    
    _absolute = Settings::Motion::ABSOLUTE;

    Job::reset();
    _startPosition  = {0,0,0};
    _position       = {-1,-1,0};
    _target 	     = {0,0,0};

    _calibration 	= Settings::Calibration::Primary.Cartesian;
    _controlPoint   = {0,0};
	_absolute = true;

    pinMode(Pin::Stepper::enable, OUTPUT);
    disengage();

    _sA.setPosition(0);
    _sB.setPosition(0);
    _sC.setPosition(0);

    _sA.setPullInOutSpeed(Settings::Motion::PULLIN, Settings::Motion::PULLIN);
    _sB.setPullInOutSpeed(Settings::Motion::PULLIN, Settings::Motion::PULLIN);
    _sC.setPullInOutSpeed(Settings::Motion::PULLIN, Settings::Motion::PULLIN);

    _sA.setInverseRotation(Settings::Stepper::DIR_A_POLARITY);
    _sB.setInverseRotation(Settings::Stepper::DIR_B_POLARITY);
    _sC.setInverseRotation(Settings::Stepper::DIR_C_POLARITY);

    _sA.setMaxSpeed(Settings::Motion::SPEED);
    _sB.setMaxSpeed(Settings::Motion::SPEED);
    _sC.setMaxSpeed(Settings::Motion::SPEED);

    _sA.setAcceleration(Settings::Motion::ACCEL);
    _sB.setAcceleration(Settings::Motion::ACCEL);
    _sC.setAcceleration(Settings::Motion::ACCEL);

    /* Initialise the sensor */
    /*while(!bno.begin()){
        Console::error("MotionPID") << "Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!" << Console::endl;
    }
    bno.setExtCrystalUse(true);
    */
}

void Motion::onUpdate(){
    if(enabled() && !_sleeping && !isPaused() && isPending()){
        if(hasFinished()){
            complete();
        }
        estimatePosition();
    }
}


void Motion::enable(){
    Service::enable();
    engage();
    sleep();
}

void Motion::disable(){
    Service::disable();
    disengage();
}

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
    if(isPending()) cancel();
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
    _sleeping = true;
}

Motion& Motion::go(float x, float y){
    setStepsVelocity(Settings::Motion::SPEED);
    go(Vec2(x, y));
    return *this;
}

Motion& Motion::goPolar(float heading, float dist){
    setStepsVelocity(Settings::Motion::SPEED);
    PolarVec poltarget = PolarVec(heading*DEG_TO_RAD, dist);
    if (_absolute){
        Vec2 target = _position + poltarget.toVec2();
        move({target.a, target.b, _position.c*RAD_TO_DEG});
    }
    else{
        Vec2 reltarget = poltarget.toVec2();
        move({reltarget.a, reltarget.b, 0});
    }

}

Motion& Motion::turn(float angle){
    setStepsVelocity(Settings::Motion::TURN_SPEED);
    if (_absolute) move({_position.a, _position.b, angle });
    else move({0, 0, angle});
    return *this;
}
  
Motion& Motion::go(Vec2 target){
    setStepsVelocity(Settings::Motion::SPEED);
    if (_absolute) move({target.a, target.b, _position.c*RAD_TO_DEG});
    else move({target.a, target.b, 0});
    return *this;
}

Motion&  Motion::align(RobotCompass rc, float orientation){
    turn((orientation - getCompassOrientation(rc)));
    return *this;
}

//Raw relative move request
Motion&  Motion::move(Vec3 target){ //target is in world frame of reference
    if(!enabled()) {
        Console::error("MotionPID") << "MotionPID not enabled" << Console::endl;
        return *this;
    }
    Job::reset();//Start a new job
    
    target.c *= DEG_TO_RAD; //Convert to rotation to radian
    if(isRelative()){
        if(target.magSq() == 0){ //Test if move is 0 length
            Console::error("MotionPID") << "Move is null" << Console::endl;
            Job::cancel();
            return *this;
        }
        target = toAbsoluteTarget(target); //convert to ABS target
    }else{
        if(target == _position){
            Console::error("MotionPID") << "Move is null" << Console::endl;
            Job::cancel();
            return *this;
        }
    }
    _target = target;Console::info("Motion") << "Target is " << _target << Console::endl;

    //to relative step target 
    Vec3 _relTarget = _target - _position; Console::info("Motion") << "Relative target is " << _relTarget << Console::endl;
    if(_optimizeRotation) _relTarget = optmizeRelTarget(_relTarget);
    _stepsTarget = targetToSteps(_relTarget); Console::info("Motion") << "Steps Target is " << _stepsTarget << Console::endl;
    
    Job::start(); //robot is moving from now
    wakeUp();
    resetSteps();

    _sA.setTargetAbs(_stepsTarget.a);
    _sB.setTargetAbs(_stepsTarget.b);
    _sC.setTargetAbs(_stepsTarget.c);
    if(m_async)_steppers.moveAsync(_sA, _sB, _sC);
    else{
        _steppers.move(_sA, _sB, _sC);
        complete();
    }
    return *this;
}

void Motion::setCalibration(CalibrationProfile c){
    _calibration = c.Cartesian;
}

void Motion::run(){
    onUpdate();
}

void Motion::pause(){
    Job::pause();
    _steppers.stop(); // set new speed

}

void Motion::resume(){
    Job::resume();
    _steppers.moveAsync(_sA, _sB, _sC); // set new speed
}

bool Motion::hasFinished() {
    THROW(_sA.getPosition());
    THROW(_sB.getPosition());
    THROW(_sC.getPosition());
    THROW(_stepsTarget);
    return (_sA.getPosition() == _stepsTarget.a && _sB.getPosition() == _stepsTarget.b && _sC.getPosition() == _stepsTarget.c);
}

void Motion::cancel() {
    Job::cancel();
    if(Job::m_state == JobState::CANCELLED){
        _steppers.stop();
    }
    estimatePosition();

    _startPosition = _position;
    _stepsTarget = Vec3(0,0,0);

    _sA.setPosition(0);
    _sB.setPosition(0);
    _sC.setPosition(0);
    _sA.setTargetAbs(0);
    _sB.setTargetAbs(0);
    _sC.setTargetAbs(0);
}

void Motion::complete() {
    Job::complete();
    _startPosition = _position = _target;
    _stepsTarget = Vec3(0,0,0);
    _sA.setPosition(0);
    _sB.setPosition(0);
    _sC.setPosition(0);
    _sA.setTargetAbs(0);
    _sB.setTargetAbs(0);
    _sC.setTargetAbs(0);
    Console::println("complete");
}

void Motion::forceCancel() {
    Job::cancel();
    _steppers.emergencyStop(); // set new speed
    estimatePosition();
    _startPosition = _position;
    _stepsTarget = Vec3(0,0,0);
    _sA.setPosition(0);
    _sB.setPosition(0);
    _sC.setPosition(0);
    _sA.setTargetAbs(0);
    _sB.setTargetAbs(0);
    _sC.setTargetAbs(0);
}


Vec3 Motion::toRelativeTarget(Vec3 absTarget){
    absTarget.sub(_position); 		 //Convert  target relativeto Absolute
    return absTarget;
}

Vec3 Motion::toAbsoluteTarget(Vec3 relTarget){
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

void  Motion::estimatePosition(){ //We are not using the estimated velocity to prevent error accumulation. We used last steps instead.
    Vec3 steps = getLastSteps(); //Read steps counter
    Vec3 delta = steps - _lastSteps;
    _lastSteps = steps;

    Vec3 angularDelta = (delta / Settings::Stepper::STEP_MODE) * (PI/100);
    Vec3 linearDelta = angularDelta * Settings::Geometry::WHEEL_RADIUS;

    //Calculate XYZ delta
    Vec3 del = fk(linearDelta);
    del.rotateZ((_position.c + del.c)); //Transform to workd space

    _position += del/_calibration;
}

Vec3 Motion::optmizeRelTarget(Vec3 relTarget){
    while(relTarget.c > PI) relTarget.c -= 2.0f*PI;
    while(relTarget.c <= -PI) relTarget.c += 2.0f*PI;
    return relTarget;
}

Vec3 Motion::targetToSteps(Vec3 relTarget){
    Vec3 angularTarget = ik(relTarget*_calibration) / Settings::Geometry::WHEEL_RADIUS;
    Vec3 f_res = (angularTarget / (PI/200.0) ) * Settings::Stepper::STEP_MODE;
    return Vec3(int(f_res.a), int(f_res.b), int(f_res.c));
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

Vec3 Motion::getLastSteps() const{
    Vec3 lastSteps = Vec3(  _sA.getPosition(), 
                            _sB.getPosition(), 
                            _sC.getPosition() );

    return lastSteps;
}

void Motion::resetSteps(){
    _sA.setPosition(0); 
    _sB.setPosition(0); 
    _sC.setPosition(0);
}

float Motion::getAbsoluteTargetDirection() const{
    return Vec2(_target - _position).heading() - _position.c;
}

bool  Motion::isAbsolute() const{
    return _absolute;
}

bool  Motion::isRelative() const{
    return !_absolute;
}


bool Motion::isRotating() const{
    return (Job::isPending() && (_target.c - _position.c) != 0);
}

bool Motion::isMoving() const{
    return (Job::isPending() &&_target != _position);
}



void  Motion::setAbsPosition(Vec3 newPos){
    _position = newPos; 
}

void Motion::setStepsVelocity(float v){
    _sA.setMaxSpeed(min(max(m_feedrate * v, Settings::Motion::PULLIN), Settings::Motion::SPEED));
    _sB.setMaxSpeed(min(max(m_feedrate * v, Settings::Motion::PULLIN), Settings::Motion::SPEED));
    _sC.setMaxSpeed(min(max(m_feedrate * v, Settings::Motion::PULLIN), Settings::Motion::SPEED));
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
    m_feedrate = min(max(feed, 1.0), 0.1);
}

float Motion::getFeedrate() const{
    return m_feedrate;
}
