#pragma once
#include "system/core/job.h"

class Timer : public Job{
public:

    Timer();

    bool isPending() override;
    bool isCompleted() override;

    
    void run()override;
    void reset()override;   
    void start()override;   

    void setDuration(long);
    long getTimeLeftSeconds();
    long getTimeLeft();
    long getElapsedTime();

private:
    void updateTime();

    long _elapsed;
    long _startTime;
    long _timeLeft;
    long _time;
};

