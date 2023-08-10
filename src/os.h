#pragma once

#include "settings.h"
#include "system/core/system.h"

#include "services/lidar/lidar.h"
#include "services/timer/timer.h"
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

#include "system/interpreter/interpreter.h"

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
    
    void execute(String& script);

    //wait blocking function
    bool isBusy() const;
    void wait(unsigned long time);
    void waitUntil(Job& obj);
    
    //Terminal
    void setConsoleLevel(ConsoleLevel level);


protected :
    //State
    void setState(RobotState);
    void printState(RobotState) const;

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

    //Utils
    Timer timer;
    

private:
    void loadService(Service*);

    bool _busy = false;
    Job* _currentJob = nullptr;
    Interpreter interpreter;
    RobotState _state;
    unsigned long _lastDrift = 0;

    //Singleton
    OperatingSystem();
    OperatingSystem(OperatingSystem &other); //Singletons should not be cloneable.

    Program currentProgram;
    static OperatingSystem instance;
};