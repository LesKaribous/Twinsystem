#include "Robot.h"
#include "Settings.h"
#include "Poi.h"
#include "Points.h"

#include "debug/Console.h"

using namespace POI;
using namespace Score;


void Robot::RecalagePrimaryBlue(){

    //motion.steppers.Engage();
    actuators.GoGrab(RobotCompass::AB);
    actuators.GoGrab(RobotCompass::BC);
    actuators.GoGrab(RobotCompass::CA);
    // !!!!!!! TEST WIP !!!!!!!!!
    // ----------------------
    while(1){
        actuators.TestSensors();
        Wait(1000);
    }
    TakePlant(RobotCompass::BC);
    PlacePlant(RobotCompass::BC);
    // ----------------------

    motion.SetAbsPosition(Vec3(-1,-1,0));

    motion.steppers.Sleep();
}

void Robot::RecalagePrimaryGreen(){
	
    motion.steppers.Engage();

    actuators.Close(RobotCompass::AB);
    actuators.Close(RobotCompass::BC);
    actuators.Close(RobotCompass::CA);

    motion.SetAbsPosition(Vec3(-1,-1,0));

    motion.steppers.Sleep();
}

void Robot::MatchPrimaryBlue(){

    motion.steppers.Engage();
	motion.SetAbsolute();

    // Fin de match
    actuators.Disengage();
	motion.steppers.Disengage();
}

void Robot::MatchPrimaryGreen(){
	motion.steppers.Engage();
	motion.SetAbsolute();

    // Fin de match
    actuators.Disengage();
	motion.steppers.Disengage();
}

void Robot::NearlyFinishPrimaryBlue(){
    // Go to End Position
    Go(blueEndPrimary);
}

void Robot::NearlyFinishPrimaryGreen(){
    // Go to End Position
    Go(greenEndPrimary);
}

void Robot::FinishPrimaryBlue(){
    // Fin de match
    actuators.Disengage();
    motion.steppers.Disengage();
}

void Robot::FinishPrimaryGreen(){
    // Fin de match
    actuators.Disengage();
    motion.steppers.Disengage();
}

void Robot::TakePlant(RobotCompass rc){
    actuators.GoGrab(rc);
    Wait(1000);
    SlowClosing(rc);
    for(int i=0;i<3;i++){
        SlowGrabing(rc);
        SlowClosing(rc);
    }
    SlowGrabing(rc);
    actuators.GoUp(rc);
    Wait(1000);
}

void Robot::PlacePlant(RobotCompass rc){
    actuators.GoGrab(rc);
    Wait(1000);
    SlowOpening(rc);
    SlowElevatorUp(rc);
    Wait(1000);
}

void Robot::SlowGrabing(RobotCompass rc){
    while(!actuators.runGrabbing(rc)) Wait(Settings::Actuators::speed);
}

void Robot::SlowGrabing(RobotCompass rc, GripperSide gs){
    while(!actuators.runGrabbing(rc,gs)) Wait(Settings::Actuators::speed);
}

void Robot::SlowOpening(RobotCompass rc){
    while(!actuators.runOpening(rc)) Wait(Settings::Actuators::speed);
}

void Robot::SlowClosing(RobotCompass rc){
    while(!actuators.runClosing(rc)) Wait(Settings::Actuators::speed);
}

void Robot::SlowElevatorUp(RobotCompass rc){
    while(!actuators.runElevatorUp(rc)) Wait(Settings::Actuators::speed);
}

void Robot::SlowElevatorDown(RobotCompass rc){
    while(!actuators.runElevatorDown(rc)) Wait(Settings::Actuators::speed);
}

void Robot::SlowElevatorGrab(RobotCompass rc){
    while(!actuators.runElevatorGrab(rc)) Wait(Settings::Actuators::speed);
}
