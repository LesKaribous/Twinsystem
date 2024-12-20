#include "os/console.h"
#include "routines.h"
#include "robot.h"

void setup(){
	Console::init();
	Console::setLevel(ConsoleLevel::INFO);

	os.setRountine(OS::BOOT, onRobotBoot);		//Execute once						(setup)
	os.setRountine(OS::IDLE, onRobotIdle);		//Execute during preparation phase 	(loop)
	os.setRountine(OS::RUNNING, onRobotRun);	//Execute during match				(loop)
	os.setRountine(OS::STOPPED, onRobotStop);	//Execute after match				(loop)
	os.setRountine(OS::PROGRAM, robotProgram);	//Execute after match				(loop)
	os.setRountine(OS::IDLE_PROGRAM, robotIdleProgram);	//Execute after match				(loop)
}

void loop(){
	os.run();	//update the services and run the current routine
}