#pragma once

#include <TeensyStep.h>
#include <stdint.h>

#include "Pin.h"
#include "Geometry.h"

#define SLOW 30
#define FAST 100

namespace Controller{
    extern bool engaged;

    void init();

    void move(Vec3 target, bool async = false);

    void stop   (bool async = false);

    void reset();

    void emergencyStop();

    void setSpeed(int speed);
    void setAccel(int accel);
    void setCalibration(bool state);
    void setFeedrate(float feed);

    u_int32_t getAccel();
    u_int32_t getSpeed();

    bool isRunning();
    bool isSleeping();
    void update();

    // Engaging motors make them ready to move.
    // Note : Motors may be engaged but sleeping !
    void engage(bool state = true);

    // Disengaging motors turn them off. They cannot move at all, not powered at all.
    void disengage();

    //Sleep mode is used to save battery or let the robot move freely. It disable both motors.
    //Note : A move request will exit sleep mode and turn them on.
    void sleep(bool state = true);
}





