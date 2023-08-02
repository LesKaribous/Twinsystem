#pragma once

#include "settings.h"
#include "system/core/system.h"

#include "services/lidar/lidar.h"
#include "services/chrono/chrono.h"
#include "services/inputs/inputs.h"
#include "services/motion/motion.h"
#include "services/screen/screen.h"
#include "services/terminal/console.h"
//#include "modules/planner/planner.h"
#include "services/neopixel/neopixel.h"
#include "services/terminal/terminal.h"
#include "services/actuators/actuators.h"
#include "services/localisation/localisation.h"


#define THROW(x) os.console.println( "Throw in " + String(__FILE__) + " at line " + String(__LINE__) + " : " + x);

enum class RobotState{
    IDLE,
    ARMED,
    STARTING,
    STARTED,
    FINISHING,
    FINISHED
};

class OperatingSystem : public SystemBase{

public:

    Console console;
    Screen screen;

    OperatingSystem();
    ~OperatingSystem();

	void update();
    
    //wait blocking function
    void wait(unsigned long temps);
    void waitUntil(Job& obj);
    
    //Terminal
    void setConsoleLevel(ConsoleLevel level);

protected :
    void processCommand(Command c);

    //State
    void setState(RobotState);
    void printState(RobotState) const;

    //Standalone services
    Lidar lidar;
    Inputs inputs;
    Motion motion;
    //Planner planner;
    NeoPixel neopixel;
    Terminal terminal;
    Intercom intercom;
    Chronometer chrono;
    Actuators actuators;
    Localisation localisation;

private:
    void loadService(Service*);
    
    RobotState _state;
    unsigned long _lastDrift = 0;
};


extern OperatingSystem os; //OS instance