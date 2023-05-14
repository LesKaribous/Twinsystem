#include "app.h"

SystemApplication app;

void setup(){
	Console::initialize();
}

void loop(){
	app.initialize();
	app.waitLaunch();
	app.startMatch();
	
	Console::println("Fin du programme");
    while(true);//End of the program
}