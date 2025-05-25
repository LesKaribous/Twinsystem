#include "os/console.h"

#include "routines.h"
#include "robot.h"

void setup(){
	Console::init();
	Console::setLevel(ConsoleLevel::VERBOSE);

	os.setRountine(OS::BOOT, onRobotBoot);   			          // - Execute once  - - - - - - - - - - (setup)
	os.setRountine(OS::MANUAL, onRobotManual); 			         // - Execute during preparation phase 	(loop)
	os.setRountine(OS::AUTO, onRobotAuto);    			        // - Execute during match  - - - - - - (loop)
	os.setRountine(OS::STOPPED, onRobotStop);			       // - Execute after match - - - - - - - (loop)
	os.setRountine(OS::AUTO_PROGRAM, robotProgramAuto);       // - Execute after match - - - - - - - (loop)
	os.setRountine(OS::MANUAL_PROGRAM, robotProgramManual);  // - Execute after match - - - - - - - (loop)

	threads.setSliceMicros(10);
	int id = threads.addThread(control, 0, 8192);
	threads.setTimeSlice(id, 100);
	
}

void loop(){
	os.run();	//update the services and run the current routine
}
