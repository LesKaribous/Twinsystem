#pragma once

#include "settings.h"
#include "system/core/system.h"
#include "system/core/console.h"
#include "services/lidar/lidar.h"
#include "services/chrono/chrono.h"
#include "services/inputs/inputs.h"
#include "services/motion/motion.h"
#include "services/screen/screen.h"
//#include "modules/planner/planner.h"
#include "services/neopixel/neopixel.h"
#include "services/terminal/terminal.h"
#include "services/actuators/actuators.h"
#include "services/localisation/localisation.h"

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
    //friend class CommandHandler;
    friend class Service;
    friend class Lidar;
    friend class Inputs;
    friend class Motion;
    friend class Chronometer;
    friend class NeoPixel;
    friend class Terminal;
    friend class Intercom;
    friend class Actuators;
    friend class Localisation;

    //Singleton
    static inline OperatingSystem& getInstance(){return m_instance;}

    void enable(ServiceID id);
    void disable(ServiceID id);
	void update();
	void control();
    
    //Terminal
    void setConsoleLevel(ConsoleLevel level);

protected :
    //State
    void setState(RobotState);
    void printState(RobotState) const;



    void loadService(Service*);
    //void registerCommands();

    RobotState _state;
    
    //Standalone services
    Screen screen;
    Lidar lidar;
    Inputs inputs;
    Motion motion;
    Chronometer match; //for match time
    NeoPixel neopixel;
    Terminal terminal;
    Intercom intercom;
    Actuators actuators;
    Localisation localisation;

    IntervalTimer interrupt;

private:
    //Singleton
    OperatingSystem();
    OperatingSystem(OperatingSystem &other) = delete; //Singletons should not be cloneable.

    static OperatingSystem m_instance;
};