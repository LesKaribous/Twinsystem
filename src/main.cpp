#include "TwinSystem.h"
#include "Robot.h"

void setup(){
	Console::Initialize();
    Console::SetLevel(ConsoleLevel::_INFO);
}

void loop(){
	Robot robot; //Start the robot
	
	robot.Initialize();
	robot.WaitLaunch();
	robot.StartMatch();
    Console::println("Fin du programme");
    while(true);//End of the program
}