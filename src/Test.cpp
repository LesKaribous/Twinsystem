#include "Robot.h"
#include "Settings.h"

#include "debug/Console.h"

void Robot::TestSteppers(){
	/*
	motion.steppers.DisableAsync();
	motion.steppers.Move(Vec3(1000,0,0));
	motion.steppers.Move(Vec3(0,1000,0));
	motion.steppers.Move(Vec3(0,0,1000));
	motion.steppers.Sleep();
	

	motion.steppers.EnableAsync();
	motion.steppers.Move(Vec3(1000,0,0));
	WaitUntil(motion.steppers);
	motion.steppers.Move(Vec3(-1000,0,0));
	WaitUntil(motion.steppers);

	motion.steppers.Move(Vec3(0,1000,0));
	WaitUntil(motion.steppers);
	motion.steppers.Move(Vec3(0,-1000,0));
	WaitUntil(motion.steppers);

	motion.steppers.Move(Vec3(0,0,1000));
	WaitUntil(motion.steppers);
	motion.steppers.Move(Vec3(0,0,-1000));
	WaitUntil(motion.steppers);


	motion.steppers.Move(Vec3(1000,1000,1000));
	WaitUntil(motion.steppers);
	motion.steppers.Move(Vec3(-1000,-1000,-1000));
	WaitUntil(motion.steppers);

	motion.steppers.Sleep();
	*/
	
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