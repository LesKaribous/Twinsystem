#include "app.h"

App myApp;

void setup(){
	myApp.update();
}

void loop(){
	myApp.update();
}

void control(){
	//OperatingSystem::getInstance().control(); // for pid constroller
}