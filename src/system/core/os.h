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

#include "system/interpreter/interpreter.h"
#include "system/interpreter/commandHandler.h"

#include <memory>

class OperatingSystem : public SystemBase{
public:
    friend class CommandHandler;
    friend class App;
    friend class Service;
    friend class Lidar;
    friend class Inputs;
    friend class Motion;
    friend class MotionPID;
    friend class Chronometer;
    friend class NeoPixel;
    friend class Terminal;
    friend class Intercom;
    friend class Actuators;
    friend class Localisation;

    //Singleton
    OperatingSystem();

    void enable(ServiceID id);
    void disable(ServiceID id);
	void control();
    
    //Terminal
    void setConsoleLevel(ConsoleLevel level);

protected :
    
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

    virtual void handleIdleState() override;
    virtual void handleRunningState() override;

private:

    void handleBootState() override;
    void handleStoppedState() override;

    void loadService(Service*);
    void registerCommands();
};