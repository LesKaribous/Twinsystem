#include "app.h"

std::unique_ptr<SystemApplication> app;

void setup(){
	Console::initialize(ConsoleLevel::_INFO);
	app = std::make_unique<SystemApplication>();
}

void loop(){
	app->waitLaunch();
	app->startMatch();
	app->endMatch();
	
	Console::println("Fin du programme");
    while(true);//End of the program
}