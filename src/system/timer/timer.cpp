#include "timer.h"
#include "os.h"


Timer::Timer(){
    _time = 0;
    _elapsed = 0;
    _timeLeft = 0;
    _startTime = 0;
    Job::reset();
}

void Timer::start(){
    if(Job::isIdle()){
        _startTime = millis();
        Job::start();
    }
}

void Timer::reset(){
    _elapsed = 0;
    _timeLeft = _time;
    _startTime = 0;
    Job::reset();
}

void Timer::run(){
    updateTime();
}

void Timer::updateTime(){
    if(isPending()){
        _elapsed = millis() - _startTime;
        _timeLeft = _time - _elapsed;
        if(_timeLeft <= 0) complete();
    }
}

void Timer::setDuration(long t){
    _time = t;
    reset();
}

long Timer::getTimeLeftSeconds(){
    updateTime();
    return _timeLeft/1000;
}

long Timer::getTimeLeft(){
    updateTime();
    return _timeLeft;
}

long Timer::getElapsedTime(){
    updateTime();
    return _elapsed;
}

