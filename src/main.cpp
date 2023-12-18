#include "os.h"



void control();


void setup(){
	os.setConsoleLevel(INFO);
	os.interrupt.begin(control, Settings::Motion::PID_INTERVAL);
}
 
void loop(){
	os.update();
}

void control(){
	os.control();
}