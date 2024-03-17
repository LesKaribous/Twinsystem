#include "settings.h"
#include "motion_pid.h"
#include "kinematics.h"
#include "os/console.h"

MotionPID::MotionPID() : Service(ID_MOTION_PID),         
    _sA(Pin::Stepper::stepA, Pin::Stepper::dirA),
    _sB(Pin::Stepper::stepB, Pin::Stepper::dirB),
    _sC(Pin::Stepper::stepC, Pin::Stepper::dirC)
{

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

    _sA.setPosition(0);
    _sB.setMaxSpeed(0);
    _sC.setMaxSpeed(0);

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

    _sAController.rotateAsync(_sA);
    _sBController.rotateAsync(_sB);
    _sCController.rotateAsync(_sC);

    _sAController.overrideSpeed(0);
    _sBController.overrideSpeed(0);
    _sCController.overrideSpeed(0);

    /* Initialise the sensor */
    /*while(!bno.begin()){
        Console::error("MotionPID") << "Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!" << Console::endl;
    }
    bno.setExtCrystalUse(true);
    */
    Console::info("MotionPID") << "Calibration settings : " << accelCorr << "\n";
}

void MotionPID::enable(){
    Service::enable();
    engage();
    sleep();
}

void MotionPID::disable(){
    Service::disable();
    disengage();
}

void MotionPID::engage(){
    _engaged = true;
    _sleeping = false;
    digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);

}

void MotionPID::disengage(){
    _engaged = false;
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
}

void MotionPID::wakeUp(){
    if(_sleeping){
        digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
        _sleeping = false;
    }
    delay(10);
}

void MotionPID::sleep(){
    if(isPending()) cancel();
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
    _sleeping = true;
}

void  MotionPID::go(float x, float y){
    Vec2 target = {x, y};
    go(target);
}

void  MotionPID::go(Vec2 target){
    if (_absolute) move({target.a, target.b, _position.c*RAD_TO_DEG});
    else move({target.a, target.b, 0});
}

void  MotionPID::goTurn(float x, float y, float angle){
    go({x, y});
    turn(angle);
}

void  MotionPID::turn(float angle){
    if (_absolute) move({_position.a, _position.b, angle });
    else move({0, 0, angle});
}

void  MotionPID::align(RobotCompass rc, float orientation){
    turn((orientation - getCompassOrientation(rc)));
}

void MotionPID::goAlign(Vec2 target, RobotCompass rc, float orientation){
    float rotation = (orientation - getCompassOrientation(rc));

    if (_absolute) move({target.a, target.b, rotation});
    else move({target.a, target.b, rotation - _position.c*RAD_TO_DEG});
}

void MotionPID::setFeedrate(int p){

}

void MotionPID::setCalibration(CalibrationProfile c){
    _calibration = c.Cartesian;
}

void MotionPID::run(){
    onUpdate();
}

void MotionPID::pause(){
    Job::pause();
    _sAController.overrideSpeed(0); // set new speed
    _sBController.overrideSpeed(0); // set new speed
    _sCController.overrideSpeed(0); // set new speed
    _targetWheelVelocity = Vec3(0);
}

void MotionPID::resume(){
    Job::resume();
}

void MotionPID::onUpdate(){
    if(enabled() && !_sleeping && !isPaused()){

        if(Settings::Motion::USE_GYROSCOPE) _position.c = getOrientation();
        if(Vec2(_target - _position).mag() < 0.5 && std::fabs(_target.c - _position.c) < 0.01){
            complete();
            //sleep();
        }
    }
}

void MotionPID::cancel() {
    Job::cancel();
    _sAController.overrideSpeed(0); // set new speed
    _sBController.overrideSpeed(0); // set new speed
    _sCController.overrideSpeed(0); // set new speed

    _targetWheelVelocity = Vec3(0);

    if(_debug){
        Console::plot("tva",fabs(_targetWheelVelocity.a));
        Console::plot("tvb",fabs(_targetWheelVelocity.b));
        Console::plot("tvc",fabs(_targetWheelVelocity.c));
    }
        
    Console::info("MotionPID") << "Start position : " << _startPosition << Console::endl;
    Console::info("MotionPID") << "Position : " << _position << Console::endl;
    Console::info("MotionPID") << "Target was : " << _target << Console::endl;
    _startPosition = _position;
}

void MotionPID::complete() {
    Job::complete();

    _sAController.overrideSpeed(0); // set new speed
    _sBController.overrideSpeed(0); // set new speed
    _sCController.overrideSpeed(0); // set new speed
    _targetWheelVelocity = Vec3(0);

    if(_debug){
        Console::plot("tva",fabs(_targetWheelVelocity.a));
        Console::plot("tvb",fabs(_targetWheelVelocity.b));
        Console::plot("tvc",fabs(_targetWheelVelocity.c));
    }

    //Console::info("MotionPID") << "Start position : " << _startPosition << Console::endl;
    //Console::info("MotionPID") << "Position : " << _position << Console::endl;
    //Console::info("MotionPID") << "Target was : " << _target << Console::endl;

    _position = _target;
    _startPosition = _position;
}

void MotionPID::forceCancel() {
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

        _targetWheelVelocity = Vec3(0);

    if(_debug){
        Console::plot("tva",fabs(_targetWheelVelocity.a));
        Console::plot("tvb",fabs(_targetWheelVelocity.b));
        Console::plot("tvc",fabs(_targetWheelVelocity.c));
    }

    Console::info("MotionPID") << "Start position : " << _startPosition << Console::endl;
    Console::info("MotionPID") << "Position : " << _position << Console::endl;
    Console::info("MotionPID") << "Target was : " << _target << Console::endl;
    _startPosition = _position;
}

void MotionPID::control(){
    if(enabled() && !_sleeping && !isPaused()){
        if(isPending()){
            estimatePosition();
            positionControl();
        }
    }
}

//https://link.springer.com/article/10.1007/s40313-019-00439-0
void MotionPID::positionControl(){
    Vec3 kP = Vec3(4.0, 4.0, 5.5); //Settings::MotionPID::kP;
    Vec3 kI = Vec3( 0.0, 0.0, 0.0);//Settings::MotionPID::kI;
    Vec3 kD = Vec3( 40.0, 40.0, 4.5);//Settings::MotionPID::kD;

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

    
    _sAController.overrideSpeed(_targetWheelVelocity.a / float(Settings::Motion::SPEED)); // set new speed %
    _sBController.overrideSpeed(_targetWheelVelocity.b / float(Settings::Motion::SPEED)); // set new speed %
    _sCController.overrideSpeed(_targetWheelVelocity.c / float(Settings::Motion::SPEED)); // set new speed %

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


Vec3 MotionPID::computeStaturedSpeed(Vec3 targetSpeed){
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


void  MotionPID::estimatePosition(){ //We are not using the estimated velocity to prevent error accumulation. We used last steps instead.
    Vec3 steps = getLastSteps(); //Read steps counter
    resetSteps(); //Reset counter

    Vec3 angularDelta = (steps / Settings::Stepper::STEP_MODE) * (PI/100);
    Vec3 linearDelta = angularDelta * Settings::Geometry::WHEEL_RADIUS;

    //Calculate XYZ delta
    Vec3 del = fk(linearDelta);
    del.rotateZ((_position.c + del.c)); //Transform to workd space

    Vec3 lastPosition = _position;
    _position += del*_calibration;
    _velocity = (lastPosition - _position)/Settings::Motion::PID_INTERVAL; //calculate velocity
}


Vec3 MotionPID::optmizeRelTarget(Vec3 relTarget){
    while(relTarget.c > PI) relTarget.c -= 2.0f*PI;
    while(relTarget.c <= -PI) relTarget.c += 2.0f*PI;
    return relTarget;
}

Vec3 MotionPID::targetToSteps(Vec3 relTarget){
    Vec3 angularSpeed = relTarget / Settings::Geometry::WHEEL_RADIUS;
    Vec3 stepsSpeed = (angularSpeed / (PI/100.0) ) * float(Settings::Stepper::STEP_MODE);
    
    return stepsSpeed;
}


//Raw relative move request
void  MotionPID::move(Vec3 target){ //target is in world frame of reference
    if(!enabled()) {
        Console::error("MotionPID") << "MotionPID not enabled" << Console::endl;
        return;
    }
    Job::reset();//Start a new job
    
    target.c *= DEG_TO_RAD; //Convert to rotation to radian
    if(isRelative()){
        if(target.magSq() == 0){ //Test if move is 0 length
            Console::error("MotionPID") << "Move is null" << Console::endl;
            Job::cancel();
            return;
        }
        target = toAbsoluteTarget(target); //convert to ABS target
    }else{
        if(target == _position){
            Console::error("MotionPID") << "Move is null" << Console::endl;
            Job::cancel();
            return;
        }
        //target = target;
    }

    //Console::info("MotionPID") << "Current position : " << getAbsPosition() << Console::endl;
    //Console::info("MotionPID") << "Current target   : " << getAbsTarget() << Console::endl;
     

    _lastError = _integral = Vec3(0); //Absolute mm, mm, rad

    Job::start(); //robot is moving from now
    wakeUp();
    _sA.setPosition(0);
    _sB.setPosition(0);
    _sC.setPosition(0);
    _target = target; //Start regulation
}


Vec3 MotionPID::toRelativeTarget(Vec3 absTarget){
    absTarget.sub(_position); 		 //Convert  target relativeto Absolute
    return absTarget;
}

Vec3 MotionPID::toAbsoluteTarget(Vec3 relTarget){
    relTarget.add(_position); 		 //Convert  target relativeto Absolute
    return relTarget;
}



bool MotionPID::isSleeping() const{
    return _sleeping;
}

void MotionPID::enableOptimization(){// Use rotation optimization (see optmizeRelTarget)
    _optimizeRotation = true;
} 
void MotionPID::disableOptimization(){
    // disable rotation optimization (see optmizeRelTarget)
    _optimizeRotation = false;
}

float MotionPID::getOrientation(){
    sensors_event_t eventAngular;
    bno.getEvent(&eventAngular, Adafruit_BNO055::VECTOR_EULER);
    float orientation = compassOffset - DEG_TO_RAD * eventAngular.orientation.x;

    while(orientation > PI) orientation-= 2.0f*PI;
    while(orientation <= -PI) orientation += 2.0f*PI;
    return  orientation;
}

void MotionPID::resetCompass(){
    sensors_event_t eventAngular;
    bno.getEvent(&eventAngular, Adafruit_BNO055::VECTOR_EULER);
    compassOffset = -DEG_TO_RAD * eventAngular.orientation.x;
}








// -------------------------------
// ----- Setters and Getters -----
// -------------------------------

Vec3 MotionPID::getAbsPosition() const{
    return _position;
}

Vec3  MotionPID::getAbsTarget() const{
    return _target;
}

Vec3 MotionPID::getLastSteps() const{
    Vec3 lastSteps = Vec3(  _sA.getPosition(), 
                            _sB.getPosition(), 
                            _sC.getPosition() );

    return lastSteps;
}

void MotionPID::resetSteps(){
    _sA.setPosition(0); 
    _sB.setPosition(0); 
    _sC.setPosition(0);//reset steps history
}

float MotionPID::getAbsoluteTargetDirection() const{
    return Vec2(_target - _position).heading() - _position.c;
}

bool  MotionPID::isAbsolute() const{
    return _absolute;
}

bool  MotionPID::isRelative() const{
    return !_absolute;
}


bool MotionPID::isRotating() const{
    return (Job::isPending() && (_target.c - _position.c) != 0);
}

bool MotionPID::isMoving() const{
    return (Job::isPending() &&_target != _position);
}



void  MotionPID::setAbsPosition(Vec3 newPos){
    _position = newPos; 
}

void  MotionPID::setAbsTarget(Vec3 newTarget){
    _target = newTarget;
}


void  MotionPID::setAbsolute(){
    _absolute = true;
}

void  MotionPID::setRelative(){
    _absolute = false;
}

