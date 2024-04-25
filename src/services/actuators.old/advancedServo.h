#pragma once
#include <Servo.h>
#include <unordered_map>

class AdvancedServo{
public:
    AdvancedServo(int pin, int defaultPos = 90, int minPos = 0, int maxPos = 180);

    void moveToDefault();
    void moveTo(int pos);
    void moveToPose(int index);
    int getPosition();

    void setPose(int index, int pose);
    int getPose(int i) const;

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

class GripperServo : public AdvancedServo{
public:
    GripperServo(int pin, int openPos, int closePos, int grabPos, int defaultPos = 90): 
        AdvancedServo(pin, defaultPos, 0, 180),
        m_openPos(openPos), 
        m_closePos(closePos), 
        m_grabPos(grabPos){}

    inline void open(){ moveTo(m_openPos);}
    inline void close(){ moveTo(m_closePos);}
    inline void grab(){ moveTo(m_grabPos);}

    inline int getOpenPosition() const { return m_openPos; }
    inline int getClosePosition() const { return m_closePos; }
    inline int getGrabPosition() const { return m_grabPos; }

    bool isClosed() const;

    private: 
    const int m_openPos, m_closePos, m_grabPos;
};
