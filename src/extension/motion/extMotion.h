#pragma once
#include "math/geometry.h"
#include "modules/motion/motion.h"

class System; // forward declaration

class ExtMotion{
private:
    System& system;
    Motion& motion;

    bool _probedX = false, _probedY = false;
    bool _probing = false;

public:
    ExtMotion(System& sys);
    ~ExtMotion();

    //Movements
    void turn(float);
    void go(Vec2);
    void go(float x, float y);

    void goAlign(Vec2, RobotCompass, float orientation); //Go align

    void goPolar(float heading, float length);
    void align(RobotCompass, float orientation);

    bool isProbed();
    bool isProbing();
    bool isXProbed();
    bool isYProbed();

};