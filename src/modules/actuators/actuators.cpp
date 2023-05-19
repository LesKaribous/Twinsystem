#include "modules/actuators/actuators.h"
#include "settings.h"

#define _GS_ Settings::Actuators::Gripper
#define _CPS_ Settings::Actuators::cherryPicker

Actuators::Actuators() : Module(ACTUATORS), 
gripperAB(  {Pin::Servo::ServoA1, _GS_::AB::right_Open, _GS_::AB::right_Close, _GS_::AB::right_Grab}, 
            {Pin::Servo::ServoA3, _GS_::AB::left_Open,  _GS_::AB::left_Close, _GS_::AB::left_Grab}, 
            {Pin::Servo::ServoA2, _GS_::AB::cherry_Open, _GS_::AB::cherry_Close}),

gripperBC(  {Pin::Servo::ServoB1, _GS_::BC::right_Open, _GS_::BC::right_Close, _GS_::BC::right_Grab}, 
            {Pin::Servo::ServoB3, _GS_::BC::left_Open,  _GS_::BC::left_Close, _GS_::BC::left_Grab}, 
            {Pin::Servo::ServoB2, _GS_::BC::cherry_Open, _GS_::BC::cherry_Close} ),

gripperCA(  {Pin::Servo::ServoC1, _GS_::CA::right_Open, _GS_::CA::right_Close, _GS_::CA::right_Grab}, 
            {Pin::Servo::ServoC3, _GS_::CA::left_Open,  _GS_::CA::left_Close, _GS_::CA::left_Grab}, 
            {Pin::Servo::ServoC2, _GS_::CA::cherry_Open, _GS_::CA::cherry_Close}),

trap(Pin::CherryPicker::pinServoTrap, _CPS_::trap_Open, _CPS_::trap_Close, _CPS_::trap_Grab)

{
    _pinTurbine = Pin::CherryPicker::pinTurbine;
    pinMode(_pinTurbine, OUTPUT);
    stopTurbine();

    gripperAB.cherryLocker.enable();
    gripperAB.leftGripper.enable();
    gripperAB.rightGripper.enable();
    gripperBC.cherryLocker.enable();
    gripperBC.leftGripper.enable();
    gripperBC.rightGripper.enable();
    gripperCA.cherryLocker.enable();
    gripperCA.leftGripper.enable();
    gripperCA.rightGripper.enable();
    trap.enable();
    
    trap.close();
    close(RobotCompass::AB);
    close(RobotCompass::BC);
    close(RobotCompass::CA);

    delay(1000);

    sleep();
}

Actuators::~Actuators(){
}

void Actuators::update(){
}

void Actuators::lock(RobotCompass rc){
    switch (rc)
    {
    case RobotCompass::AB :
        gripperAB.cherryLocker.close();
        break;

    case RobotCompass::BC :
        gripperBC.cherryLocker.close();
        break;

    case RobotCompass::CA :
        gripperCA.cherryLocker.close();
        break;
    
    default:
        break;
    }
}

void Actuators::unlock(RobotCompass rc){
    switch (rc)
    {
    case RobotCompass::AB :
        gripperAB.cherryLocker.open();
        break;

    case RobotCompass::BC :
        gripperBC.cherryLocker.open();
        break;

    case RobotCompass::CA :
        gripperCA.cherryLocker.open();
        break;
    
    default:
        break;
    }
}

void Actuators::close(RobotCompass rc){
    switch (rc)
    {
    case RobotCompass::AB :
        gripperAB.rightGripper.close();
        gripperAB.leftGripper.close();
        break;

    case RobotCompass::BC :
        gripperBC.rightGripper.close();
        gripperBC.leftGripper.close();
        break;

    case RobotCompass::CA :
        gripperCA.rightGripper.close();
        gripperCA.leftGripper.close();
        break;
    
    
    default:
        break;
    }
}

void Actuators::open(RobotCompass rc){
    switch (rc)
    {
    case RobotCompass::AB :
        gripperAB.rightGripper.open();
        gripperAB.leftGripper.open();
        break;

    case RobotCompass::BC :
        gripperBC.rightGripper.open();
        gripperBC.leftGripper.open();
        break;

    case RobotCompass::CA :
        gripperCA.rightGripper.open();
        gripperCA.leftGripper.open();
        break;
    
    default:
        break;
    }
}

void Actuators::grab(RobotCompass rc){
    switch (rc)
    {
    case RobotCompass::AB :
        gripperAB.rightGripper.grab();
        gripperAB.leftGripper.grab();
        break;

    case RobotCompass::BC :
        gripperBC.rightGripper.grab();
        gripperBC.leftGripper.grab();
        break;

    case RobotCompass::CA :
        gripperCA.rightGripper.grab();
        gripperCA.leftGripper.grab();
        break;
    
    default:
        break;
    }
}



void Actuators::close(){

    gripperAB.rightGripper.close();
    gripperAB.leftGripper.close();

    gripperBC.rightGripper.close();
    gripperBC.leftGripper.close();

    gripperCA.rightGripper.close();
    gripperCA.leftGripper.close();
}

void Actuators::open(){

    gripperAB.rightGripper.open();
    gripperAB.leftGripper.open();

    gripperBC.rightGripper.open();
    gripperBC.leftGripper.open();

    gripperCA.rightGripper.open();
    gripperCA.leftGripper.open();
}

void Actuators::danseRight(){

    gripperAB.rightGripper.open();
    gripperAB.leftGripper.grab();

    gripperBC.rightGripper.open();
    gripperBC.leftGripper.grab();

    gripperCA.rightGripper.open();
    gripperCA.leftGripper.grab();
}

void Actuators::danseLeft(){

    gripperAB.rightGripper.grab();
    gripperAB.leftGripper.open();

    gripperBC.rightGripper.grab();
    gripperBC.leftGripper.open();

    gripperCA.rightGripper.grab();
    gripperCA.leftGripper.open();
}






void Actuators::setTurbine(int speed){
    analogWrite(_pinTurbine, map(speed, 0, 100, 0, 255));
}

void Actuators::stopTurbine(){
    analogWrite(_pinTurbine, 0);
}

void Actuators::suckBall(){
    trap.close();
    setTurbine(100);
}

void Actuators::dropBall(){
    stopTurbine();
    trap.open();
}

void Actuators::openTrap(){
    trap.open();
}


void Actuators::closeTrap(){
    trap.close();
}


void Actuators::ungrab(RobotCompass rc){
    open(rc);
}

void Actuators::applause(RobotCompass rc){
    open(rc); unlock(rc);
    delay(500);
    grab(rc); lock(rc);
    delay(500);
    open(rc); unlock(rc);
    delay(500);
    grab(rc); lock(rc);
    delay(500);
    open(rc); unlock(rc);
}

void Actuators::sleep(){
    gripperAB.cherryLocker.sleep();
    gripperAB.leftGripper.sleep();
    gripperAB.rightGripper.sleep();
    gripperBC.cherryLocker.sleep();
    gripperBC.leftGripper.sleep();
    gripperBC.rightGripper.sleep();
    gripperCA.cherryLocker.sleep();
    gripperCA.leftGripper.sleep();
    gripperCA.rightGripper.sleep();
}


void Actuators::engage(){
    gripperAB.cherryLocker.enable();
    gripperAB.leftGripper.enable();
    gripperAB.rightGripper.enable();
    gripperBC.cherryLocker.enable();
    gripperBC.leftGripper.enable();
    gripperBC.rightGripper.enable();
    gripperCA.cherryLocker.enable();
    gripperCA.leftGripper.enable();
    gripperCA.rightGripper.enable();
    digitalWrite(_pinTurbine, LOW);
}

void Actuators::disengage(){
    gripperAB.cherryLocker.disable();
    gripperAB.leftGripper.disable();
    gripperAB.rightGripper.disable();
    gripperBC.cherryLocker.disable();
    gripperBC.leftGripper.disable();
    gripperBC.rightGripper.disable();
    gripperCA.cherryLocker.disable();
    gripperCA.leftGripper.disable();
    gripperCA.rightGripper.disable();
    trap.disable();
    digitalWrite(_pinTurbine, LOW);
}

