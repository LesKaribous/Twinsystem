#include "app.h"

std::unique_ptr<SystemApplication> app;

void setup(){
	Console::initialize(ConsoleLevel::_INFO);
	app = std::make_unique<SystemApplication>();
	THROW("Boot completed successfully")
	delay(800);
}

void loop(){
	app->waitLaunch();
	app->startMatch();
	app->endMatch();
	
    while(true);//Safe
}