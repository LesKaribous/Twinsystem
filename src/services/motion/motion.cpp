#include "settings.h"
#include "motion.h"
#include "kinematics.h"
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

    /* Initialise the sensor */
    while(!bno.begin()){
        /* There was a problem detecting the BNO055 ... check your connections */
        os.console.error("Motion") << "Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!" << os.console.endl;
    }
    bno.setExtCrystalUse(true);

    
    os.console.info("Motion") << "Calibration settings : " << accelCorr << "\n";
}

Motion::~Motion(){}


void Motion::run(){
    update();
}


void Motion::update(){

    if(m_enabled && !_sleeping && !isPaused()){

        if(isPending()){
            if(millis() - lastPIDTick  < Settings::Motion::PID_MIN_PERIOD) return;
            if(millis() - lastPIDTick > Settings::Motion::PID_MAX_PERIOD){
                if(lastPIDTick != 0) os.console.info(m_ID) << "PID update is too slow (" << int(millis() - lastPIDTick) << "ms | " << int(Settings::Motion::PID_MAX_PERIOD) << "ms)" << os.console.endl;
                lastPIDTick = millis() - Settings::Motion::PID_MAX_PERIOD;
            }
            
            float dt = float(millis() - lastPIDTick)/1000.0;

            //Speed estimation based on last steps
            estimateVelocity(dt);
            //Position estimation and rotation measure
            _position.c = getOrientation();
            _position = estimatePosition(_position, _velocity, dt); //in world frame of reference

            positionControl(dt);
            speedControl(dt);
        }

        if(Vec2(_target - _position).mag() < 0.5 && abs(_target.c - _position.c) < 0.025 && _velocity.mag() < 50){
            complete();
            sleep();
        }
        
        os.screen.x.SetValue(_lastError.x);
        os.screen.y.SetValue(_lastError.y);
        os.screen.z.SetValue(_lastError.z);
    }
}

void Motion::speedControl(float dt){
    Vec3 kP = Vec3(0.001,0.001,0.001); //Settings::Motion::kP;
    Vec3 kI = Settings::Motion::kI;
    Vec3 kD = Settings::Motion::kD;
    
    Vec3 error = Vec3(_targetWheelVelocity) - Vec3(_wheelVelocity);
    _velIntegral  += error * dt; //steps / s
    Vec3 corr = (error * kP) + (_velIntegral * kI) + (((error - _velLastError)/dt) * kD); // This implements a simple P regulator (can be extended to a PID if necessary)
    _velLastError = Vec3(error); //steps / s

    //os.console.println("velocity : " + String(_velocity));

    _sAController.overrideSpeed(corr.a); // set new speed
    _sBController.overrideSpeed(corr.b); // set new speed
    _sCController.overrideSpeed(corr.c); // set new speed

    lastPIDTick = millis();
    

   /*
    // Apply speeds to motors
    _sAController.overrideSpeed(speedA/wheelRadius);
    _sBController.overrideSpeed(speedB/wheelRadius);
    _sCController.overrideSpeed(speedC/wheelRadius);
    */
}

//https://link.springer.com/article/10.1007/s40313-019-00439-0
void Motion::positionControl(float dt){
    Vec3 kP = Vec3(0.1,0.1,0.05); //Settings::Motion::kP;
    Vec3 kI = Vec3(0.0,0.0,0.0);//Settings::Motion::kI;
    Vec3 kD = Vec3(0.008,0.008,0.01);//Settings::Motion::kD;

    //correction
    Vec3 error = _target - _position;

    _integral  += error * dt; //Absolute mm, mm, rad
    Vec3 corr = (error * kP) + (_integral * kI) + (((error - _lastError)/dt) * kD); // This implements a simple P regulator (can be extended to a PID if necessary)
    _lastError = Vec3(error); //Absolute mm, mm, rad

    float beta = atan2f(error.y, error.x) - _position.c;

    _targetWheelVelocity = Vec3(1000,0,0);//corr * Vec3(cos(beta), sin(beta),1.0);
    _targetWheelVelocity = computeStaturedSpeed(_targetWheelVelocity);

    _targetWheelVelocity = ik(_targetWheelVelocity);

   if(true){
        os.console.plot("px",_position.x);
        os.console.plot("tx",_target.x);
        os.console.plot("py",_position.y);
        os.console.plot("ty",_target.y);
        os.console.plot("pa",_position.c);
        os.console.plot("ta",_target.c);
    }
}


Vec3 Motion::computeStaturedSpeed(Vec3 targetV){
    // Maximum speed constraints (in mm/s or similar units)
    float maxSpeed = Settings::Motion::SPEED;
    float scaleFactor = 1000.0; // This scales the rotational error to an angular speed

    // Compute target speeds while respecting the maximum speed constraints
    Vec3 targetSpeed = targetV * scaleFactor;

    //staturation
    
    float M = std::max(std::max(targetSpeed.x, targetSpeed.y), targetSpeed.z);
    if(M > maxSpeed){
        targetSpeed *= maxSpeed/M;
    }

    //limit
    /*
    targetSpeed.x = std::min(std::max(target.x * scaleFactor, -maxSpeed), maxSpeed);
    targetSpeed.y = std::min(std::max(target.y * scaleFactor, -maxSpeed), maxSpeed);
    targetSpeed.z = std::min(std::max(target.z * scaleFactor, -maxSpeed), maxSpeed);
    */
    return targetSpeed;
}


Vec2  Motion::estimatePosition(Vec3 start, Vec3 steps, float dt) const{
    // Calculate wheel speeds
    // Assuming SpeedA, SpeedB, SpeedC are the speeds of wheels A, B, C

    Vec3 relativeVelocity = fk(steps);
    return start + relativeVelocity*dt; //To world frame of reference
}

void  Motion::estimateVelocity(float dt){
    _lastStepsHistory.push(getLastSteps());
    resetSteps();

    if(_lastStepsHistory.size() > Settings::Motion::VELOCITY_SAMPLES){
        _lastStepsSum -= _lastStepsHistory.front();
        _lastStepsHistory.pop();
    }
    _lastStepsSum += _lastStepsHistory.back();
    _wheelVelocity = _lastStepsSum/(dt*_lastStepsHistory.size());
    _velocity = fk(_wheelVelocity);
}


Vec3 Motion::optmizeRelTarget(Vec3 relTarget){
    while(relTarget.c > PI) relTarget.c -= 2.0f*PI;
    while(relTarget.c <= -PI) relTarget.c += 2.0f*PI;
    return relTarget;
}

Vec3 Motion::targetToSteps(Vec3 relTarget){
    relTarget.rotateZ(_position.c);
    relTarget *= _calibration; 					                //Apply cartesian calibration
    relTarget = ik(relTarget);				  					//Apply inverse kinematics result in steps
    relTarget.mult(Settings::Stepper::STEP_MODE * RAD_TO_DEG); 	//Apply stepping multiplier
    return relTarget;
}



void Motion::pause(){
    Job::pause();
    _sAController.overrideSpeed(0); // set new speed
    _sBController.overrideSpeed(0); // set new speed
    _sCController.overrideSpeed(0); // set new speed
}

void Motion::resume(){
    Job::resume();
}

void Motion::cancel() {
    Job::cancel();
    _sAController.overrideSpeed(0); // set new speed
    _sBController.overrideSpeed(0); // set new speed
    _sCController.overrideSpeed(0); // set new speed
        
    os.console.info("Motion") << "Start position : " << _startPosition << os.console.endl;
    os.console.info("Motion") << "Position : " << _position << os.console.endl;
    os.console.info("Motion") << "Target was : " << _target << os.console.endl;
    _startPosition = _position;
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

    os.console.info("Motion") << "Start position : " << _startPosition << os.console.endl;
    os.console.info("Motion") << "Position : " << _position << os.console.endl;
    os.console.info("Motion") << "Target was : " << _target << os.console.endl;
    _startPosition = _position;
}

void Motion::complete() {
    Job::complete();

    _sAController.overrideSpeed(0); // set new speed
    _sBController.overrideSpeed(0); // set new speed
    _sCController.overrideSpeed(0); // set new speed

    //os.console.info("Motion") << "Start position : " << _startPosition << os.console.endl;
    //os.console.info("Motion") << "Position : " << _position << os.console.endl;
    //os.console.info("Motion") << "Target was : " << _target << os.console.endl;

    _position = _target;
    _startPosition = _position;
}



void  Motion::go(float x, float y){
    Vec2 target = {x, y};
    go(target);
}

void  Motion::turn(float angle){
    if (_absolute) move({_position.a, _position.b, angle });
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
void  Motion::move(Vec3 target){ //target is in world frame of reference
    if(!m_enabled) {
        os.console.error("Motion") << "Motion not enabled" << os.console.endl;
        return;
    }
    Job::reset();//Start a new job
    
    target.c *= DEG_TO_RAD; //Convert to rotation to radian
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

    //os.console.info("Motion") << "Current position : " << getAbsPosition() << os.console.endl;
    //os.console.info("Motion") << "Current target   : " << getAbsTarget() << os.console.endl;
     

    _lastError = _integral = Vec3(0); //Absolute mm, mm, rad

    Job::start(); //robot is moving from now
    wakeUp();
    _sA.setPosition(0);
    _sB.setPosition(0);
    _sC.setPosition(0);
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

float Motion::getOrientation(){
    sensors_event_t eventAngular;
    bno.getEvent(&eventAngular, Adafruit_BNO055::VECTOR_EULER);
    float orientation = compassOffset - DEG_TO_RAD * eventAngular.orientation.x;

    while(orientation > PI) orientation-= 2.0f*PI;
    while(orientation <= -PI) orientation += 2.0f*PI;
    return  orientation;
}

void Motion::resetCompass(){
    sensors_event_t eventAngular;
    bno.getEvent(&eventAngular, Adafruit_BNO055::VECTOR_EULER);
    compassOffset = -DEG_TO_RAD * eventAngular.orientation.x;
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
    if(isPending()) cancel();
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
    _sleeping = true;
    
}

void Motion::wakeUp(){
    if(_sleeping){
        digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
        _sleeping = false;
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

void Motion::setCalibration(CalibrationProfile c){
    _calibration = c.Cartesian;
}


void Motion::setFeedrate(int p){

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
    _sC.setPosition(0);//reset steps history
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

void  Motion::setAbsTarget(Vec3 newTarget){
    _target = newTarget;
}


void  Motion::setAbsolute(){
    _absolute = true;
}

void  Motion::setRelative(){
    _absolute = false;
}

