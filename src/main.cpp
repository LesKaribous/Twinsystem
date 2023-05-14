#include "system.h"

System sys;

void setup(){
	Console::initialize();
}

void loop(){
	sys.update(); //Update the whole system
}