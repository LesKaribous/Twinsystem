#pragma once
#include <Servo.h>

namespace TwinSystem{

    class BistableServo{
    public:
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


    BistableServo::BistableServo(int pin, int openPos, int closePos) :
     _pin(pin), _openPos(openPos), _closePos(closePos), _enabled(false), _sleeping(false), _closed(false)
    {
        enable();
        sleep();
    }

    void BistableServo::sleep(bool state){
        if(_enabled && _sleeping != state){
            if(!state) _servo.attach(_pin);
            else _servo.detach();
            _sleeping = state;
        }
    }

    void BistableServo::open(){
        if(_enabled){
            if(_sleeping) sleep(false); //Wake
            _servo.write(_openPos);
            _closed = false;
        }
    }

    void BistableServo::close(){
        if(_enabled){
            if(_sleeping) sleep(false); //Wake
            _servo.write(_closePos);
            _closed = true;
        }
    }

    void BistableServo::toggle(){
        if(_enabled){
            if(_sleeping) sleep(false); //Wake
            if(_closed) open();
            else close();
        }
    }

    void BistableServo::enable(){
        _servo.attach(_pin);
        _enabled = true;
    }

    void BistableServo::disable(){
        _servo.detach();
        _enabled = false;
    }


} // namespace TwinSystem
