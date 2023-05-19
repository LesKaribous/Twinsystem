#include "app.h"

std::unique_ptr<SystemApplication> app;

void setup(){
	Console::initialize(ConsoleLevel::_INFO);
	app = std::make_unique<SystemApplication>();
	THROW("Boot completed successfully")
}

void loop(){
	app->waitLaunch();

	app->danse();
	app->dessin();

	app->startMatch();
	
	app->endMatch();
	
    while(true);//Safe
}