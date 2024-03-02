#include "os.h"
#include "routines.h"
OS& os = OS::instance();

void setup(){
	os.setRountine(OS::BOOT, onRobotBoot);		//Execute once
	os.setRountine(OS::IDLE, onRobotIdle);		//Execute during preparation phase 	(loop)
	os.setRountine(OS::RUNNING, onRobotRun);	//Execute during match				(loop)
	os.setRountine(OS::STOPPED, onRobotStop);	//Execute after match				(loop)
}

void loop(){
	os.loop();	//update the services and run the current routine
}