#include "Robot.h"
#include "Settings.h"
#include "Poi.h"
#include "Points.h"

#include "debug/Console.h"

using namespace POI;
using namespace Score;


void Robot::RecalagePrimaryBlue(){

    motion.steppers.Engage();

    actuators.Open(RobotCompass::BC);
    Wait(500);
    actuators.GoDown(RobotCompass::BC);
    Wait(2000);
    actuators.GoGrab(RobotCompass::BC);
    Wait(1000);
    actuators.Grab(RobotCompass::BC);
    Wait(2000);
    actuators.GoUp(RobotCompass::BC);
    Wait(2000);
    actuators.GoGrab(RobotCompass::BC);
    Wait(2000);
    actuators.Close(RobotCompass::BC);
    Wait(500);
    actuators.Open(RobotCompass::BC);
    Wait(500);
    actuators.GoUp(RobotCompass::BC);
    Wait(1000);
    actuators.GoDown(RobotCompass::BC);

    /*
    actuators.GoUp(RobotCompass::AB);
    actuators.GoUp(RobotCompass::BC);
    actuators.GoUp(RobotCompass::CA);

    delay(2000);

    actuators.GoDown(RobotCompass::AB);
    actuators.GoDown(RobotCompass::BC);
    actuators.GoDown(RobotCompass::CA);

    delay(2000);

    actuators.GoGrab(RobotCompass::AB);
    actuators.GoGrab(RobotCompass::BC);
    actuators.GoGrab(RobotCompass::CA);

    delay(2000);
    
    actuators.Open(RobotCompass::AB);
    actuators.Open(RobotCompass::BC);
    actuators.Open(RobotCompass::CA);

    delay(2000);

    actuators.Close(RobotCompass::AB);
    actuators.Close(RobotCompass::BC);
    actuators.Close(RobotCompass::CA);

    delay(2000);

    actuators.Grab(RobotCompass::AB);
    actuators.Grab(RobotCompass::BC);
    actuators.Grab(RobotCompass::CA);

    delay(2000);

    actuators.Open(RobotCompass::AB);
    actuators.Open(RobotCompass::BC);
    actuators.Open(RobotCompass::CA);

    delay(2000);
    */

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