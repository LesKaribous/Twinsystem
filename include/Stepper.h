#pragma once

#include "TeesyStep.h"
#include "Pin.h"

namespace Controller{
    StepControl controller;

    Stepper sLeft(Pin::StepLeft,Pin::DirLeft), 
            sRight(Pin::StepRight,Pin::stepDirRight);
    
    void init();

    void go     (long dist, bool async = true);
    void turn   (long angle, bool async = true);
    void stop   (bool async = true);

    void sleep();
    void run();
    void emergencyStop();

    void setSpeed();
    void setAccel();

    int getAccel();
    int getSpeed();

    bool isRunning();
    bool isSleeping();
}





