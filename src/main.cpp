
/*
#include "os/console.h"
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
/**/


#include "services/motion/controllers/twinController.h"
#include "pin.h"


/*
TwinController robot;



void setup() {
    robot.setPIDGains(1.0, 0.1, 0.5);
	robot.enable();

	Vec3 targetVelocity(1000, 0, 0);
    robot.setTargetVelocity(targetVelocity);
}

long lastTime = 0;

void loop() {

	if(micros() - lastTime > 1000*1) {
		lastTime = micros();
    	robot.control();
	}

}

*/




Stepper sA(Pin::Stepper::stepA, Pin::Stepper::dirA, Settings::Stepper::DIR_A_POLARITY);

void setup() {
	pinMode(Pin::Stepper::enable, OUTPUT);
	digitalWrite(Pin::Stepper::enable, LOW);

	sA.setTargetVelocity(100000);
	sA.setAcceleration(50);
	sA.enable();
}

long long lastTime = 0;

void loop() {

	if(micros() - lastTime > Settings::Stepper::STEPPER_DELAY) {
		lastTime = micros();
    	sA.control();
	}

}