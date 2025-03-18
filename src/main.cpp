
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

	threads.setSliceMicros(10); // Setting is needed!
	int id = threads.addThread(control, 1);
	threads.setTimeSlice(id, 20);
}

void loop(){
	os.run();	//update the services and run the current routine
}

