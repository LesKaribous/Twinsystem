#include "settings.h"
#include "motion.h"
#include "system/math/kinematics.h"
#include "os.h"


Motion::Motion() : Service(MOTION),         
    _sA(Pin::Stepper::stepA, Pin::Stepper::dirA),
    _sB(Pin::Stepper::stepB, Pin::Stepper::dirB),
    _sC(Pin::Stepper::stepC, Pin::Stepper::dirC)
{
    os.screen.addBootProgress(10);
	os.screen.drawBootProgress("Loading Motion...");
    _calibration = Settings::Calibration::Primary.Cartesian; //set to primary by default
    
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

    _stepPos = _stepTarget = Vec3(0);

    _sA.setPosition(0);
    _sB.setMaxSpeed(0);
    _sC.setMaxSpeed(0);

    _sA.setMaxSpeed(Settings::Motion::SPEED);
    _sB.setMaxSpeed(Settings::Motion::SPEED);
    _sC.setMaxSpeed(Settings::Motion::SPEED);

    _sA.setAcceleration(Settings::Motion::ACCEL);
    _sB.setAcceleration(Settings::Motion::ACCEL);
    _sC.setAcceleration(Settings::Motion::ACCEL);

    _sAController.rotateAsync(_sA);
    _sBController.rotateAsync(_sB);
    _sCController.rotateAsync(_sC);

    _sAController.overrideSpeed(0);
    _sBController.overrideSpeed(0);
    _sCController.overrideSpeed(0);

    os.screen.x.SetValue(_position.x);
    os.screen.y.SetValue(_position.y);
    os.screen.z.SetValue(_position.z);
}

Motion::~Motion(){}

void Motion::run(){
    update();
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

void Motion::engage(){
    _engaged = true;
    _sleeping = false;
    digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);

}

void Motion::disengage(){
    _engaged = false;
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
}

void Motion::sleep(){
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
    _sleeping = true;
}

void Motion::wakeUp(){
    if(_sleeping){
        digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
        _sleeping = false;
    }
}

Vec3 Motion::getCurrentStepsPosition(){
    return _stepPos;
}

void Motion::pid(){
    static unsigned lastTick = 0;
    _stepPos = Vec3(_sA.getPosition(), _sB.getPosition(), _sC.getPosition());
    if (millis() - lastTick > Settings::Motion::PID_PERIOD)
    {
        
        float dt = Settings::Motion::PID_PERIOD;
        float kP = Settings::Motion::kP;
        Vec3 cPos = _stepPos;

        Vec3 delta = (_stepTarget - cPos) * (kP / dt); // This implements a simple P regulator (can be extended to a PID if necessary)
        Vec3 factor = { std::max(-1.0f, std::min(1.0f, delta.a)),// limit to -1.0..1.0
                        std::max(-1.0f, std::min(1.0f, delta.b)),
                        std::max(-1.0f, std::min(1.0f, delta.c))};

        _sAController.overrideSpeed(factor.a); // set new speed
        _sBController.overrideSpeed(factor.b); // set new speed
        _sCController.overrideSpeed(factor.c); // set new speed

        lastTick = 0;
    }
}

void Motion::pause(){
    Job::pause();
    _sAController.overrideSpeed(0); // set new speed
    _sBController.overrideSpeed(0); // set new speed
    _sCController.overrideSpeed(0); // set new speed
}

void Motion::resume(){
    Job::resume();
    
    Vec3 newPos = estimatePosition();
}

void Motion::cancel() {
    Job::cancel();
    _sAController.overrideSpeed(0); // set new speed
    _sBController.overrideSpeed(0); // set new speed
    _sCController.overrideSpeed(0); // set new speed
    
    Vec3 newPos = estimatePosition();

    _stepTarget = 0;
    _stepPos = 0;

    os.console.info("Motion") << "Start position : " << _startPosition << os.console.endl;
    os.console.info("Motion") << "Estimate position : " << newPos << os.console.endl;
    os.console.info("Motion") << "Target was : " << _target << os.console.endl;
    _startPosition = _position = newPos;
}

void Motion::forceCancel() {
    Job::cancel();
    _sAController.emergencyStop(); // set new speed
    _sBController.emergencyStop(); // set new speed
    _sCController.emergencyStop(); // set new speed

    _sAController.overrideSpeed(0); // set new speed
    _sBController.overrideSpeed(0); // set new speed
    _sCController.overrideSpeed(0); // set new speed

    _sAController.rotateAsync(_sA); //recover from emmergency stop
    _sBController.rotateAsync(_sB);
    _sCController.rotateAsync(_sC);

    Vec3 newPos = estimatePosition();

    _stepTarget = 0;
    _stepPos = 0;

    os.console.info("Motion") << "Start position : " << _startPosition << os.console.endl;
    os.console.info("Motion") << "Estimate position : " << newPos << os.console.endl;
    os.console.info("Motion") << "Target was : " << _target << os.console.endl;
    _startPosition = _position = newPos;
}

void Motion::complete() {
    Job::complete();
    Vec3 newPos = estimatePosition();

    _sAController.overrideSpeed(0); // set new speed
    _sBController.overrideSpeed(0); // set new speed
    _sCController.overrideSpeed(0); // set new speed

    _stepTarget = 0;
    _stepPos = 0;

    os.console.info("Motion") << "Start position : " << _startPosition << os.console.endl;
    os.console.info("Motion") << "Estimate position : " << newPos << os.console.endl;
    os.console.info("Motion") << "Final position : " << _position << os.console.endl;
    os.console.info("Motion") << "Target was : " << _target << os.console.endl;
    //Need to take actual rotation when adding relative target
    //_position = _startPosition.add(_target.rotateZ(-_position.c));
    _position = _target;
    _startPosition = _position;
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

void Motion::setCalibration(CalibrationProfile c){
    _calibration = c.Cartesian;
}


void Motion::setFeedrate(int p){

}

void Motion::update(){
    if(m_enabled && !_sleeping && !isPaused()){
        if(isMoving()) pid();
        if(isMoving() && (_stepPos - _stepTarget).mag() < 5){
            _stepPos = _stepTarget;
            _position = _target;
            complete();
        }
        
        os.screen.x.SetValue(_position.x);
        os.screen.y.SetValue(_position.y);
        os.screen.z.SetValue(_position.z);
    
    }
}

void  Motion::go(float x, float y){
    Vec2 target = {x, y};
    go(target);
}

void  Motion::turn(float angle){
    if (_absolute) move({_position.a, _position.b, _position.c*RAD_TO_DEG + angle });
    else move({0, 0, angle});
}

void  Motion::goTurn(float x, float y, float angle){
    go({x, y});
    turn(angle);
}
  
void  Motion::go(Vec2 target){
    if (_absolute) move({target.a, target.b, _position.c*RAD_TO_DEG});
    else move({target.a, target.b, 0});
}

void Motion::goAlign(Vec2 target, RobotCompass rc, float orientation){
    float rotation = (orientation - getCompassOrientation(rc));

    if (_absolute) move({target.a, target.b, rotation});
    else move({target.a, target.b, rotation - _position.c*RAD_TO_DEG});
}

void  Motion::align(RobotCompass rc, float orientation){
    turn((orientation - getCompassOrientation(rc)));
}

//Raw relative move request
void  Motion::move(Vec3 target){
    if(!m_enabled) {
        os.console.error("Motion") << "Motion not enabled" << os.console.endl;
        return;
    }
    Job::reset();
    //Set new target
    target.c *= DEG_TO_RAD; //Convert to radian
    if(isRelative()){
        if(target.magSq() == 0){ //Test if move is 0 length
            os.console.error("Motion") << "Move is null" << os.console.endl;
            Job::cancel();
            return;
        }
        _target = toAbsoluteTarget(target); //convert to ABS target
    }else{
        if(target == _position){
            os.console.error("Motion") << "Move is null" << os.console.endl;
            Job::cancel();
            return;
        }
        _target = target;
    }

    //Optimize target rotation while taking actual pos into acount
    _relTarget  = toRelativeTarget(_target);
    if(_optimizeRotation )_relTarget = optmizeRelTarget(_relTarget); //Optimize rotation (% 2*k*PI)

    os.console.info("Motion") << "Current position : " << getAbsPosition() << os.console.endl;
    os.console.info("Motion") << "Current target   : " << getAbsTarget() << os.console.endl;
    os.console.info("Motion") << "Current relative target : " << _relTarget << os.console.endl;
    
    Job::start(); //robot is moving from now

    wakeUp();
    _sA.setPosition(0);
    _sB.setPosition(0);
    _sC.setPosition(0);
    _stepPos = Vec3(0);
    _stepTarget = targetToSteps(_relTarget); //Set new target
    os.console.info("Step target") << _stepTarget << os.console.endl;
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
    Vec3 relativePosition = fk(_stepPos);
    
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

