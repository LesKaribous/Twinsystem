#pragma once
#include "modules/lidar/lidar.h"
#include "modules/motion/motion.h"
#include "modules/actuators/actuators.h"
class System;

class ExtTest{
private:
    System& system;
    Lidar& lidar;
    Motion& motion;
    Actuators& actuators;

public:
    ExtTest(System& sys);
    ~ExtTest();

    void testMotion();
    void testSteppers();
    void testDetection();
    void testOrientation();
};