#include "Robot.h"
#include "Settings.h"

#include "debug/Console.h"

void Robot::TestSteppers(){

	motion.SetAbsPosition(Vec3(0,0,0));
	motion.SetAbsolute();
	Go(1000,1000);
}

void Robot::TestOrientation(){
	motion.SetAbsPosition({0,0,0});
	motion.SetAbsolute();
	Align(RobotCompass::A, GetCompassOrientation(TableCompass::EAST));
	Align(RobotCompass::AB, GetCompassOrientation(TableCompass::EAST));
}


void Robot::TestDetection(){

	motion.SetAbsPosition({10,10,0});
	motion.SetAbsolute();

	//DisableAvoidance();

	Console::print("Current pos : {");
	Console::print(motion.GetAbsPosition().a);
	Console::print(",");
	Console::print(motion.GetAbsPosition().b);
	Console::println("}");

	GetMaxLidarDist(Vec2(motion.GetAbsPosition().a, motion.GetAbsPosition().b), 0);

	GetMaxLidarDist(Vec2(motion.GetAbsPosition().a, motion.GetAbsPosition().b), 90*DEG_TO_RAD);
	
	GetMaxLidarDist(Vec2(motion.GetAbsPosition().a, motion.GetAbsPosition().b), 270*DEG_TO_RAD);
	
	GetMaxLidarDist(Vec2(motion.GetAbsPosition().a, motion.GetAbsPosition().b), -90*DEG_TO_RAD);
	
}

void Robot::CalibAngle(int turnNumber){

	motion.steppers.Engage();
	motion.SetAbsolute();
    actuators.Close(RobotCompass::AB);
    actuators.Close(RobotCompass::BC);
    actuators.Close(RobotCompass::CA);

    DisableAvoidance();

	motion.SetAbsPosition(Vec3(0,0,0));

	Turn(turnNumber*360.0);

	motion.steppers.Sleep();
}

void Robot::CaliDistance(int distance,RobotCompass){

}