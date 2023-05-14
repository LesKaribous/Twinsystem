#pragma once
#include "core/module.h"

class Chronometer{
public:

    enum class State{
        IDLE,
        STARTED,
        FINISHED
    };

    Chronometer();
    void start();
    bool isFinished();
    bool isNearlyFinished();


    int getTimeLeftSeconds();
    unsigned long getTimeLeft();
    unsigned long getElapsedTime();

private:
    void updateTime();

    unsigned long _elapsed;
    unsigned long _startTime;
    unsigned long _timeLeft;
    int _score;

    State _state;
};

