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



#define HERE " [" + String(__FILE__) + " at line " + String(__LINE__) + "]"
#define THROW(x) os.console.println( "Throw in " + String(__FILE__) + " at line " + String(__LINE__) + " : " + x);
#define os OperatingSystem::getInstance()

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
    friend class CommandHandler;
    Console console;
    Screen screen;

    //Singleton
    static OperatingSystem& getInstance();

    void enable(ServiceID id);
    void disable(ServiceID id);
	void update();
    
    //Terminal
    void setConsoleLevel(ConsoleLevel level);

protected :
    //State
    void setState(RobotState);
    void printState(RobotState) const;

public:
    //Standalone services
    Lidar lidar;
    
    Inputs inputs;
    Motion motion;
    Chronometer match; //for match time
    //Planner planner;
    NeoPixel neopixel;
    Terminal terminal;
    Intercom intercom;
    Actuators actuators;
    Localisation localisation;

private:
    void loadService(Service*);
    void registerCommands();


    RobotState _state;

    //Singleton
    OperatingSystem();
    OperatingSystem(OperatingSystem &other); //Singletons should not be cloneable.

    static OperatingSystem instance;
};