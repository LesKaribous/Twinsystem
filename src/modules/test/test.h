#pragma once
#include "core/module.h"

#include "modules/lidar/lidar.h"
#include "modules/inputs/inputs.h"
#include "modules/motion/motion.h"
#include "modules/screen/screen.h"
#include "modules/planner/planner.h"
#include "modules/neopixel/neopixel.h"
#include "modules/actuators/actuators.h"

class Test : public Module{
private:
    Lidar* lidar;
    Screen* screen;
    Inputs* inputs;
    Motion* motion;
    Planner* planner;
    NeoPixel* neopixel;
    Actuators* actuators;

public:
    Test(   Lidar* m_lidar,
            Screen* m_screen,
            Inputs* m_inputs,
            Motion* m_motion,
            Planner* m_planner,
            NeoPixel* m_neopixel,
            Actuators* m_actuators        
    );
    ~Test();

    void update() override;

    bool testMotion();
    bool testSteppers();
    bool testDetection();
    bool testOrientation();
};