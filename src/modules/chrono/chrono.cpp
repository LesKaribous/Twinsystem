#include "modules/chrono/chrono.h"
#include "settings.h"


Chronometer::Chronometer() : Module(CHRONO){
    _elapsed = 0;
    _timeLeft = Settings::Match::DURATION;
    _startTime = 0;
    _score = 0;
    _state = State::IDLE;
}

void Chronometer::update(){
    updateTime();
}

void Chronometer::start(){
    if(_state == State::IDLE){
        _startTime = millis();
        _state = State::STARTED;
    }
}

bool Chronometer::isFinished(){
    updateTime();
    return _state == State::FINISHED;
}

bool Chronometer::isNearlyFinished(){
    updateTime();
    return _timeLeft <= Settings::Match::NEARLY_FINISH; // <= 5s
}

void Chronometer::updateTime(){
    if(_state == State::STARTED){
        _elapsed = millis() - _startTime;
        _timeLeft = Settings::Match::DURATION - _elapsed;
        if(_timeLeft <= Settings::Match::ENDMATCH) _state = State::FINISHED;
    }
}

int Chronometer::getTimeLeftSeconds(){
    updateTime();
    return _timeLeft/1000;
}

unsigned long Chronometer::getTimeLeft(){
    updateTime();
    return _timeLeft;
}

unsigned long Chronometer::getElapsedTime(){
    updateTime();
    return _elapsed;
}

