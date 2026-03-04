
#include "os/console.h"
#include "program/routines.h"
#include "config/env.h"

void setup(){
	Console::init();
	Console::setLevel(ConsoleLevel::VERBOSE);

	os.setRountine(OS::BOOT, onRobotBoot);   			          // - Execute once  - - - - - - - - - - (setup)
	os.setRountine(OS::MANUAL, onRobotManual); 			         // - Execute during preparation phase 	(loop)
	os.setRountine(OS::AUTO, onRobotAuto);    			        // - Execute during match  - - - - - - (loop)
	os.setRountine(OS::STOPPED, onRobotStop);			       // - Execute after match - - - - - - - (loop)
	os.setRountine(OS::AUTO_PROGRAM, programAuto);       // - Execute after match - - - - - - - (loop)
	os.setRountine(OS::MANUAL_PROGRAM, programManual);  // - Execute after match - - - - - - - (loop)
	
	cycle_manager.registerCycle(CycleFrequency::T_10US, step);
	cycle_manager.registerCycle(CycleFrequency::T_1MS, control);
	cycle_manager.start();
}

void loop(){
	os.run();	//update the services and run the current routine
}