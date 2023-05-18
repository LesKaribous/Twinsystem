#pragma once
#include "core/lib.h"

struct BistableServoProps{
    int pin, open, close, grab;
};

class BistableServo{
public:
    BistableServo(BistableServoProps props);
    BistableServo(int pin, int openPos, int closePos, int grabPos);

    void open();
    void close();
    void toggle();
    void grab();

    void enable();
    void sleep();
    void wakeUp();
    void disable();

    inline bool isClosed() const {return _closed;}
    inline bool isSleeping() const {return _sleeping;}
    inline bool isEnable() const {return _enabled;}
    inline bool idGrabed() const {return _grabed;}

private:
    Servo _servo;
    int _pin, _openPos, _closePos, _grabPos;
    bool _enabled, _sleeping, _closed, _grabed;
};
