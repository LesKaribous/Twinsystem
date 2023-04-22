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


void Robot::TestDetection(){

	motion.SetAbsPosition({10,10,0});
	motion.SetAbsolute();
	
	actuators.Open(RobotCompass::AB);
	actuators.Open(RobotCompass::BC);
	actuators.Open(RobotCompass::CA);

	Wait(1000);

	actuators.Grab(RobotCompass::AB);
	actuators.Grab(RobotCompass::BC);
	actuators.Grab(RobotCompass::CA);

	Wait(1000);

	actuators.Close(RobotCompass::AB);
	actuators.Close(RobotCompass::BC);
	actuators.Close(RobotCompass::CA);
	
	Wait(1000);
}