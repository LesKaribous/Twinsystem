#include "services/motion/motion.h"
#include "utils/geometry.h"
#include "kinematics.h"
#include "settings.h"
#include "os/console.h"
#include "motion.h"
#include "utils/timer/timer.h"

#include <cmath>

INSTANTIATE_SERVICE(Motion, motion)

Motion::Motion() : Service(ID_MOTION)
    #ifdef TEENSY35
   ,_sA(Pin::Stepper::stepA, Pin::Stepper::dirA),
    _sB(Pin::Stepper::stepB, Pin::Stepper::dirB),
    _sC(Pin::Stepper::stepC, Pin::Stepper::dirC)
    #endif
{}

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

    _IMU = false;
    /* Initialise the sensor */
    for(int i = 0; i < 10; i++){
        if(myOtos.begin()){
            _IMU = true;           
            break;
        }
    }

    if(!_IMU) Console::error("Motion") << "Ooops, no OTOS detected ... It may be unplugged... Tanpis ?" << Console::endl;
    else{
        myOtos.setLinearUnit(kSfeOtosLinearUnitMeters);
        myOtos.setAngularUnit(kSfeOtosAngularUnitRadians);
        myOtos.resetTracking();
        Console::success("Motion") << "OTOS connected" << Console::endl;
    }
}

// Main loop
void Motion::onUpdate(){
    if(enabled() && !_sleeping && !isPaused() && isPending()){
        if(hasFinished()){
            complete();
        }
        estimatePosition();
    }
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
    if(isPending()) cancel();
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
    _sleeping = true;
}


// Movements routines
Motion& Motion::go(float x, float y){
    setStepsVelocity(Settings::Motion::SPEED);
    _isMoving = true;
    go(Vec2(x, y));
    return *this;
}

Motion& Motion::goPolar(float heading, float dist){
    setStepsVelocity(Settings::Motion::SPEED);
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
    //if(_useBNO){
    //    _position.c = getOrientation();
    //}
    setStepsVelocity(Settings::Motion::TURN_SPEED);
    _isMoving = true;
    _isRotating = true;
    if (_absolute) move(Vec3(_position.a, _position.b, angle));
    else move(Vec3(0, 0, angle));
    return *this;
}
  
Motion& Motion::go(Vec2 target){
    setStepsVelocity(Settings::Motion::SPEED);
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
    Vec3 _relTarget = toRelativeTarget(_target); Console::trace("Motion") << "Relative target is " << _relTarget << Console::endl;
    if(_optimizeRotation) _relTarget = optmizeRelTarget(_relTarget);
    _stepsTarget = targetToSteps(_relTarget); Console::trace("Motion") << "Steps Target is " << _stepsTarget << Console::endl;
    
    Job::start(); //robot is moving from now
    wakeUp();
    resetSteps();

    #ifdef TEENSY35
    _sA.setTargetAbs(_stepsTarget.a);
    _sB.setTargetAbs(_stepsTarget.b);
    _sC.setTargetAbs(_stepsTarget.c);
    #endif

    if(m_async){
        #ifdef TEENSY35
        _steppers.moveAsync(_sA, _sB, _sC);
        #endif
    }
    else{
        #ifdef TEENSY35
        _steppers.move(_sA, _sB, _sC);
        #endif
        complete();
    }
    return *this;
}

void Motion::setCalibration(CalibrationProfile c){
    _calibration = c.Cartesian;
}

void Motion::setAcceleration(int accel){
    #ifdef TEENSY35
    _sA.setAcceleration(accel*Settings::Stepper::STEP_MODE);
    _sB.setAcceleration(accel*Settings::Stepper::STEP_MODE);
    _sC.setAcceleration(accel*Settings::Stepper::STEP_MODE);
    #endif
}

void Motion::run(){
    onUpdate();
}

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
    _steppers.moveAsync(_sA, _sB, _sC); // set new speed
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

void  Motion::estimatePosition(){ //We are not using the estimated velocity to prevent error accumulation. We used last steps instead.
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





// IMU and PID
void Motion::control(){
    estimatePosition();

    Vec3 kP = Vec3(4.0, 4.0, 5.5); //Settings::Motion::kP;
    Vec3 kI = Vec3( 0.0, 0.0, 0.0);//Settings::Motion::kI;
    Vec3 kD = Vec3( 40.0, 40.0, 4.5);//Settings::Motion::kD;

    //correction
    Vec3 error =  _target - _position;

    _integral += error; //mm, mm, rad
    Vec3 corr = (error * kP) + (_integral * kI) + (((error - _lastError)) * kD); // This implements a simple P regulator (can be extended to a PID if necessary)
    _lastError = error; //mm, mm, rad

    _targetWheelVelocity = corr.rotateZ(-_position.c);
    _targetWheelVelocity = ik(_targetWheelVelocity);
    _targetWheelVelocity = targetToSteps(_targetWheelVelocity);
    _targetWheelVelocity = computeStaturedSpeed(_targetWheelVelocity);
    //_targetWheelVelocity = computeStaturedSpeed(Vec3(500,0,3).rotateZ(-_position.c));

    #ifdef TEENSY35
    _sAController.overrideSpeed(_targetWheelVelocity.a / float(Settings::Motion::SPEED)); // set new speed %
    _sBController.overrideSpeed(_targetWheelVelocity.b / float(Settings::Motion::SPEED)); // set new speed %
    _sCController.overrideSpeed(_targetWheelVelocity.c / float(Settings::Motion::SPEED)); // set new speed %
    #endif
    
if(true){
        Console::plot("px",_position.x);
        Console::plot("tx",_target.x);
        Console::plot("py",_position.y);
        Console::plot("ty",_target.y);
        //Console::plot("pa",_position.c);
        //Console::plot("ta",_target.c);

        /*
        Console::plot("tva",fabs(_targetWheelVelocity.a));
        Console::plot("tvb",fabs(_targetWheelVelocity.b));
        Console::plot("tvc",fabs(_targetWheelVelocity.c));
        
        Console::plot("va",_sAController.getCurrentSpeed());
        Console::plot("vb",_sBController.getCurrentSpeed());
        Console::plot("vc",_sCController.getCurrentSpeed());*/
    }
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
    _closeLoop = false;
}
void Motion::closeLoop(){
    _closeLoop = _IMU; //activate only if IMU available
}

void Motion::autoCalibration(){
    Serial.println("Calibrating IMU scales...");
    Serial.println("Warning robot will move during this sequence...");
    delay(2000);
    Serial.println("Not implemented...");

}

void Motion::readIMU(){
    sfe_otos_pose2d_t myPosition;
    myOtos.getPosition(myPosition);

    _unsafePosition.x = myPosition.x * 1000.0; //to millimeters
    _unsafePosition.y = myPosition.y * 1000.0; //to millimeters

    float orientation = myPosition.h;
    while(orientation > PI) orientation-= 2.0f*PI;
    while(orientation <= -PI) orientation += 2.0f*PI;
    _unsafePosition.z = orientation;
}

void Motion::calibrateIMU(){
    Serial.println("Ensure the OTOS is flat and stationary, then enter any key to calibrate the IMU");
    Serial.println("Calibrating IMU...");

    // Calibrate the IMU, which removes the accelerometer and gyroscope offsets
    myOtos.calibrateImu();
    Serial.println("Calibrated IMU.");
    _IMU_calibrated = true;
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

float Motion::getAbsoluteTargetDirection() const{
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
    //if(_useBNO) setOrientation(newPos.c);
}

void Motion::setStepsVelocity(float v){
    #ifdef TEENSY35
    _sA.setMaxSpeed(min(max(m_feedrate * v*Settings::Stepper::STEP_MODE, Settings::Motion::PULLIN*Settings::Stepper::STEP_MODE), Settings::Motion::SPEED*Settings::Stepper::STEP_MODE));
    _sB.setMaxSpeed(min(max(m_feedrate * v*Settings::Stepper::STEP_MODE, Settings::Motion::PULLIN*Settings::Stepper::STEP_MODE), Settings::Motion::SPEED*Settings::Stepper::STEP_MODE));
    _sC.setMaxSpeed(min(max(m_feedrate * v*Settings::Stepper::STEP_MODE, Settings::Motion::PULLIN*Settings::Stepper::STEP_MODE), Settings::Motion::SPEED*Settings::Stepper::STEP_MODE));
    #endif
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
