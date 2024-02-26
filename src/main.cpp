#include "system/core/os.h"
OperatingSystem& os = OperatingSystem::getInstance();

void setup(){}
 
void loop(){
	os.update();
}