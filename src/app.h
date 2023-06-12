#pragma once

#include "settings.h"

#include "system/core/system.h"

#include "match/poi.h"
#include "match/score.h"
#include "system/math/geometry.h"


#include "modules/lidar/lidar.h"
#include "modules/chrono/chrono.h"
#include "modules/inputs/inputs.h"
#include "modules/motion/motion.h"
#include "modules/screen/screen.h"
//#include "modules/planner/planner.h"
#include "modules/neopixel/neopixel.h"
#include "modules/terminal/terminal.h"
#include "modules/actuators/actuators.h"
#include "modules/localisation/localisation.h"

enum class RobotState{
    IDLE,
    ARMED,
    STARTING,
    STARTED,
    FINISHING,
    FINISHED
};

class SystemApplication{
    System system;

public:
    SystemApplication();
    ~SystemApplication();

	void update();
    void processCommand(Command c);

    //State
    void setState(RobotState);
    void printState(RobotState);

    //wait blocking function
    void wait(unsigned long temps);
    void waitUntil(Job& obj);


private:
    void loadModule(Module*);

    //Standalone modules

    Lidar lidar;
    Screen screen;
    Inputs inputs;
    Motion motion;
    //Planner planner;
    NeoPixel neopixel;
    Terminal terminal;
    Intercom intercom;
    Chronometer chrono;
    Actuators actuators;
    Localisation localisation;

    RobotState _state;

    unsigned long _lastDrift = 0;
};