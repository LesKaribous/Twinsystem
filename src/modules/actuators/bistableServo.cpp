#include "bistableServo.h"

BistableServo::BistableServo(BistableServoProps props) :
    _pin(props.pin), _openPos(props.open), _closePos(props.close), _grabPos(props.grab) , _enabled(false), _sleeping(false), _closed(false), _grabed(false)
{}

BistableServo::BistableServo(int pin, int openPos, int closePos, int grabPos) :
    _pin(pin), _openPos(openPos), _closePos(closePos), _grabPos(grabPos), _enabled(false), _sleeping(false), _closed(false), _grabed(false)
{}

void BistableServo::sleep(){
    _servo.detach();
    _sleeping = true;
}

void BistableServo::wakeUp(){
    if(_enabled){
        _servo.attach(_pin);
    }
    _sleeping = false;
}

void BistableServo::open(){
    if(_enabled){
        if(_sleeping) wakeUp(); //Wake
        _servo.write(_openPos);
        _closed = false;
    }
}

void BistableServo::close(){
    if(_enabled){
        if(_sleeping) wakeUp(); //Wake
        _servo.write(_closePos);
        _closed = true;
    }
}

void BistableServo::grab(){
    if(_enabled){
        if(_sleeping) wakeUp(); //Wake
        _servo.write(_grabPos);
        _grabed = true;
    }
}

void BistableServo::toggle(){
    if(_enabled){
        if(_sleeping) wakeUp(); //Wake
        if(_closed) open();
        else close();
    }
}

void BistableServo::enable(){
    _servo.attach(_pin);
    _enabled = true;
    _sleeping = false;
}

void BistableServo::disable(){
    _servo.detach();
    _enabled = false;
}
