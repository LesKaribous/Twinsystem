#pragma once
#include "os/jobs/job.h"

#define RUN_EVERY(X, D){static long lastRun = 0;if(millis()-lastRun > D){lastRun = millis();X;}}

class Timer : public Job{
public:

    Timer();
    
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

