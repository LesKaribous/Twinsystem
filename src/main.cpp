
#include "os/console.h"

/*
#include "routines.h"
#include "robot.h"

void setup(){
	Console::init();
	Console::setLevel(ConsoleLevel::INFO);

	os.setRountine(OS::BOOT, onRobotBoot);   			          // - Execute once  - - - - - - - - - - (setup)
	os.setRountine(OS::MANUAL, onRobotManual); 			         // - Execute during preparation phase 	(loop)
	os.setRountine(OS::AUTO, onRobotAuto);    			        // - Execute during match  - - - - - - (loop)
	os.setRountine(OS::STOPPED, onRobotStop);			       // - Execute after match - - - - - - - (loop)
	os.setRountine(OS::AUTO_PROGRAM, robotProgramAuto);       // - Execute after match - - - - - - - (loop)
	os.setRountine(OS::MANUAL_PROGRAM, robotProgramManual);  // - Execute after match - - - - - - - (loop)

	//tw_Thread::createThread(control, 1024, "control");
}

void loop(){
	os.run();	//update the services and run the current routine
}

*/


/**/
#include "services/motion/positionController.h"
#include "pin.h"
#include "os/console.h"

PositionController robot;

void setup() {
	Console::init();
	Console::setLevel(ConsoleLevel::INFO);

	robot.setTarget(Vec3(0,0,120.0*DEG_TO_RAD));
}

long lastTime = 0;

void loop() {
	if(micros() - lastTime > Settings::Stepper::STEPPER_DELAY) {
		lastTime = micros();
    	robot.control();
	}
}


/**/