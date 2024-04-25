#pragma once
#include <Servo.h>
#include <unordered_map>

#define MAX_POSES 10

class SmartServo{
public:
    SmartServo();
    SmartServo(int pin, int defaultPos = 90, int minPos = 0, int maxPos = 180);
    SmartServo(SmartServo& cpy);

    void moveToDefault();
    void moveTo(int pos);
    void moveToPose(int index);
    int getPosition();

    void setPose(int index, int pose);
    int getPose(int i) const;
    bool hasPose(int i) const;

    void enable();
    void sleep();
    void wakeUp();
    void disable();

    inline bool isSleeping() const {return m_sleeping;}
    inline bool isEnable() const {return m_enabled;}
//private:
public:
    Servo m_servo;
    int m_position;
    std::unordered_map<int, int> m_poses;
    
    bool m_enabled, m_sleeping;
    const int m_pin, m_minPos, m_maxPos, m_defaultPos;

};