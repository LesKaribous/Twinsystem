#include "settings.h"
#include "motion.h"
#include "system/math/kinematics.h"
#include "os.h"




/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)





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
  while(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    os.console.error("Motion") << "Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!" << os.console.endl;
  }
  bno.setExtCrystalUse(true);
}

Motion::~Motion(){}


void Motion::run(){
    update();
}

void Motion::update(){

    if(m_enabled && !_sleeping && !isPaused()){
        if(isMoving()) pid();
        if(isMoving() && Vec2(_target - _position).mag() < 1 && fabs(_target.c - _position.c) < 0.05 && false){
            //os.console.info("update") << "_position : "<< _position << os.console.endl;
            //os.console.info("update") << "_target : "<< _target << os.console.endl;
            _position = _target;
            complete();
        }
        
        os.screen.x.SetValue(_position.x);
        os.screen.y.SetValue(_position.y);
        os.screen.z.SetValue(_position.z);
    
    }
}

void Motion::pid(){
    static unsigned lastTick = 0;
    if (millis() - lastTick > Settings::Motion::PID_PERIOD){

        //os.console.info("Motion") << "PID period : "<< int(millis() - lastTick) << "ms" << os.console.endl;

        float dt = float(Settings::Motion::PID_PERIOD)/1000.0; //seconds
        float kP = Settings::Motion::kP;
        float kI = Settings::Motion::kI;
        float kD = Settings::Motion::kD;

        Vec3 lastSteps = Vec3(_sA.getPosition(), _sB.getPosition(), _sC.getPosition());
        _sA.setPosition(0); _sB.setPosition(0); _sC.setPosition(0);//reset steps history

        _position = estimatePosition(_position, lastSteps); //in world frame of reference
        //os.console.print(">p:"); os.console.println(_position.mag());
        //os.console.print(">t:"); os.console.println(_target.mag());
        
        sensors_event_t event;
        bno.getEvent(&event);

        /* Display the floating point data */

        _position.c = -DEG_TO_RAD * event.orientation.x;
        os.console.info("Motion") << "BNO: " << event.orientation.x  << os.console.endl;
        while(_position.c > PI) _position.c -= 2.0f*PI;
        while(_position.c <= -PI) _position.c += 2.0f*PI;
        os.console.info("Motion") << "TTheta: " << _position.c * RAD_TO_DEG << os.console.endl;


        Vec3 error = _target - (_position); //relative target
       
        _integral  += error * dt; //Absolute mm, mm, rad
        Vec3 corr = (error * kP) + (_integral * kI) + (((error - _lastError)/dt) * kD); // This implements a simple P regulator (can be extended to a PID if necessary)
        _lastError = error; //Absolute mm, mm, rad


        corr.rotateZ(_position.c); //to robot frame of reference
        corr = targetToSteps(corr);

        Vec3 factor = { std::max(-1.0f, std::min(1.0f, corr.a)),// limit to -1.0..1.0
                        std::max(-1.0f, std::min(1.0f, corr.b)),
                        std::max(-1.0f, std::min(1.0f, corr.c))};

        _sAController.overrideSpeed(factor.a); // set new speed
        _sBController.overrideSpeed(factor.b); // set new speed
        _sCController.overrideSpeed(factor.c); // set new speed

        lastTick = millis();
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

Vec3 Motion::optmizeRelTarget(Vec3 relTarget){
    while(relTarget.c > PI) relTarget.c -= 2.0f*PI;
    while(relTarget.c <= -PI) relTarget.c += 2.0f*PI;
    return relTarget;
}

Vec3 Motion::targetToSteps(Vec3 relTarget){
    relTarget *= _calibration; 					                //Apply cartesian calibration
    relTarget = ik(relTarget);				  					//Apply inverse kinematics result in steps
    relTarget.mult(Settings::Stepper::STEP_MODE * RAD_TO_DEG); 	//Apply stepping multiplier
    return relTarget;
}

Vec3 Motion::toRelativeTarget(Vec3 absTarget){
    absTarget.sub(_position); 		 //Convert  target relativeto Absolute
    return absTarget;
}

Vec3 Motion::toAbsoluteTarget(Vec3 relTarget){
    relTarget.add(_position); 		 //Convert  target relativeto Absolute
    return relTarget;
}

Vec3  Motion::estimatePosition(Vec3 start, Vec3 steps) const{
    Vec3 relativePosition = fk(steps);
    
    relativePosition.a /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;
    relativePosition.b /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;
    relativePosition.c /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;

    relativePosition.a /= _calibration.a;
    relativePosition.b /= _calibration.b;
    relativePosition.c /= _calibration.c;

    //os.console.info("Motion") << relativePosition << os.console.endl;
    return start.add(relativePosition.rotateZ(-relativePosition.c)); //To world frame of reference
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
