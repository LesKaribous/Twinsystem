#include "navigation.h"
#include "os/console.h"
#include "kinematics.h"
#include "services/localisation/localisation.h"

INSTANTIATE_SERVICE(Navigation, nav);


//-------------------------
//     Movements
//-------------------------

// Service class methods
void Navigation::onAttach() {

}

void Navigation::onUpdate() {

}


void Navigation::enable() {

}


void Navigation::disable() {

}

// Job class methods
void Navigation::run() {

}

void Navigation::pause() {

}

void Navigation::resume() {

}

void Navigation::cancel() {

}

void Navigation::complete() {

}

void Navigation::forceCancel() {

}



Navigation& Navigation::begin(){
    m_chainning = true;
    return *this;
}

void Navigation::end(){
    m_chainning = false;
}

// Movements routines
Navigation& Navigation::go(float x, float y){
    go(Vec2(x, y));
    return *this;
}

Navigation& Navigation::goPolar(float heading, float dist){
    //setStepsVelocity(Settings::Motion::SPEED);
    //_isMoving = true;
    PolarVec poltarget = PolarVec(heading*DEG_TO_RAD, dist);
    if (m_absolute){
        Vec2 target = m_position + poltarget.toVec2();
        move(Vec3(target.a, target.b, m_position.c*RAD_TO_DEG));
    }
    else{
        Vec2 reltarget = poltarget.toVec2();
        move(Vec3(reltarget.a, reltarget.b, 0));
    }
    return *this;
}

Navigation& Navigation::turn(float angle){

    if (m_absolute) move(Vec3(m_position.a, m_position.b, angle));
    else move(Vec3(0, 0, angle));
    return *this;
}
  
Navigation& Navigation::go(Vec2 dest){
    if(!m_chainning) m_moves.clear();

    Vec3 target;
    if (m_absolute) target = Vec3(dest.a, dest.b, m_position.c*RAD_TO_DEG);
    else target = Vec3(dest.a, dest.b, 0);


    return *this;
}

Navigation&  Navigation::align(RobotCompass rc, float orientation){
    turn((orientation - getCompassOrientation(rc)));
    return *this;
}

//Raw relative move request
Navigation&  Navigation::move(Vec3 target){ //target is in world frame of reference
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
        if(target == m_position){
            Console::error("Motion") << "Move is null" << Console::endl;
            complete();
            return *this;
        }
    }
    //m_target = target; 
    //Console::trace("Motion") << "Target is " << m_target << Console::endl;

    //todo
    
    return *this;
}



//-------------------------
//     Utils
//-------------------------


Vec3 Navigation::targetToSteps(Vec3 relTarget){
    Vec3 angularTarget = ik(relTarget*m_calibration) / Settings::Geometry::WHEEL_RADIUS;
    Vec3 f_res = (angularTarget / (PI/200.0) ) * Settings::Stepper::STEP_MODE;
    return Vec3(int(f_res.a), int(f_res.b), int(f_res.c));
}

Vec3 Navigation::optmizeRelTarget(Vec3 relTarget){
    while(relTarget.c > PI) relTarget.c -= 2.0f*PI;
    while(relTarget.c <= -PI) relTarget.c += 2.0f*PI;
    return relTarget;
}

Vec3 Navigation::toRelativeTarget(Vec3 absTarget){
    absTarget.sub(m_position); 		 //Convert  target relativeto Absolute
    absTarget.rotateZ(m_position.c); //to robot FoR
    return absTarget;
}

Vec3 Navigation::toAbsoluteTarget(Vec3 relTarget){
    relTarget.rotateZ(-m_position.c); //to world FoR
    relTarget.add(m_position); 		 //Convert  target relativeto Absolute
    return relTarget;
}

void Navigation::setCalibration(CalibrationProfile c){
    m_calibration = c.Cartesian;
}
/*
void  Navigation::estimatePosition(){ //We are not using the estimated velocity to prevent error accumulation. We used last steps instead.

    if(localisation.useIMU()){
        _position = localisation.getPosition();
    }else{
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
}
*/



//-------------------------
//     Settters
//-------------------------


bool  Navigation::isAbsolute() const{
    return m_absolute;
}

bool  Navigation::isRelative() const{
    return !m_absolute;
}

Vec3 Navigation::getPosition() const{
    return m_position;
}


void  Navigation::setAbsolute(){
    m_absolute = true;
}

void  Navigation::setRelative(){
    m_absolute = false;
}

void Navigation::setAsync(){
    m_async = true;
}
void Navigation::setSync(){
    m_async = false;
}

void  Navigation::setPosition(Vec3 newPos){
    m_position = newPos; 
    if(localisation.useIMU())
        localisation.setPosition(newPos);
}

void Navigation::enableOptimization(){// Use rotation optimization (see optmizeRelTarget)
    m_optimizeRotation = true;
} 

void Navigation::disableOptimization(){
    // disable rotation optimization (see optmizeRelTarget)
    m_optimizeRotation = false;
}