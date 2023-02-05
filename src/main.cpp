#include "TwinSystem.h"

using namespace TwinSystem;

class Robot : public System{
public :
    Robot() : System(){}
};

void setup(){}

void loop(){
    Scope<Robot> robot;
    robot->Run();
}