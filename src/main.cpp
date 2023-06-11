#include "app.h"

SystemApplication app;

void setup(){
	Console::setLevel(INFO);
}

void loop(){
	app.update();
}