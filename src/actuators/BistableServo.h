#pragma once
#include <Servo.h>

namespace TwinSystem{

    struct BistableServoProps{
        int pin, close, open;
    };

    class BistableServo{
    public:
        BistableServo(BistableServoProps props);
        BistableServo(int pin, int openPos, int closePos);

        void open();
        void close();
        void toggle();

        void enable();
        void sleep(bool state = true);
        void disable();

    private:
        Servo _servo;
        int _pin, _openPos, _closePos;
        bool _enabled, _sleeping, _closed;
    };

} // namespace TwinSystem
