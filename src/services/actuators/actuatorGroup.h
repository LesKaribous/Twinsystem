#pragma once
#include "smartServo.h"
#include "sensor.h"
#include <vector>

#define MAX_SERVOS 12

class ActuatorGroup{
private:
    std::unordered_map<int, SmartServo> m_servos;

public:
    ActuatorGroup();  

    void enable();
    void disable();
    void sleep();

    void createServo(int id, int pin, int defaultPos, int minPos = 0, int maxPos = 180);
    SmartServo& getServo(int id);
    bool hasServo(int id);
    void moveServoToPose(int servo, int pose, int speed);
};
