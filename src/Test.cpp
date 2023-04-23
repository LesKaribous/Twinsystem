#include "Robot.h"
#include "Settings.h"

#include "debug/Console.h"

void Robot::TestSteppers(){

	motion.SetAbsPosition(Vec3(0,0,0));
	motion.SetAbsolute();
	Go(100,0);
	Go(0,0);

	Turn(90);
	Turn(0);

	motion.SetRelative();
	Go(100,0);
	Go(-100,0);

	Turn(90);
	Turn(-90);
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

	DisableAvoidance();
	Turn(0);
	GetMaxLidarDist(Vec2(motion.GetAbsPosition().a, motion.GetAbsPosition().b), motion.GetAbsoluteTargetDirection());

	Turn(90);
	GetMaxLidarDist(Vec2(motion.GetAbsPosition().a, motion.GetAbsPosition().b), motion.GetAbsoluteTargetDirection());
	
	Turn(180);
	GetMaxLidarDist(Vec2(motion.GetAbsPosition().a, motion.GetAbsPosition().b), motion.GetAbsoluteTargetDirection());
	
	Turn(270);
	GetMaxLidarDist(Vec2(motion.GetAbsPosition().a, motion.GetAbsPosition().b), motion.GetAbsoluteTargetDirection());
	

}