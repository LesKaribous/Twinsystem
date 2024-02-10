#pragma once
#include <Servo.h>

namespace TwinSystem{

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
        void goTo(int _pos);
        int getPosition();

        void enable();
        void sleep();
        void wakeUp();
        void disable();

        int getOpenPosition() const { return _openPos; }
        int getClosePosition() const { return _closePos; }
        int getGrabPosition() const { return _grabPos; }

    private:
        Servo _servo;
        int _pin, _openPos, _closePos, _grabPos;
        bool _enabled, _sleeping, _closed, _grabed;
    };

} // namespace TwinSystem
