#pragma once
#include "system/core/service.h"

class Chronometer : public Service{
public:

    enum class State{
        IDLE,
        STARTED,
        FINISHED
    };

    Chronometer();

    void update() override;
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

