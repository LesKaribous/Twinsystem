#include "os.h"


String script = R"(
#Test Script
disable(neopixel)
enable(motion)
setAbsPosition(0,0,0)
go(100,0)
turn(60)
grab(AB) #Grab object using actuator "AB"
#if(hasObject(AB))
#go(0,0)
ungrab(AB)
#else
#go(500,0)
#end
sleep
)";

void setup(){
	os.setConsoleLevel(INFO);
	os.update();
	os.update();
	os.update();
	os.update();


	delay(1000);
	os.execute(script);


}
 
void loop(){
	os.update();
}