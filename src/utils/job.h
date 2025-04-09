#pragma once
#include <Arduino.h>
//#include "os/asyncExecutor.h"

enum class JobState{
    IDLE,
    PAUSED,
    RUNNNING,
    CANCELLED,
    COMPLETED
};


class Job  {
public:
    Job();
    ~Job();// make the class polymorphic by providing a virtual destructor

    String toString() const;
    //operator AsyncExecutor() { OS::instance().waitUntil(*this); };

    bool isIdle()const;
    bool isPaused()const;
    bool isRunning()const;
    bool isPending()const;
    bool isCompleted()const;
    bool isCancelled()const;

    inline explicit operator bool() const {
        return isCompleted();  // Return true if the job succeeded
    }

    long elaspedTime() const;
    long pauseDuration() const;

    virtual void run() = 0; 
    virtual void reset();   //Set to IDLE
    virtual void start();   //Set to PENDING
    virtual void pause();   //Set to PAUSED
    virtual void resume();  //Set to PENDING
    virtual void cancel();  //Set to CANCELLED
    virtual void complete();//Set to COMPLETED

protected:
    JobState m_state = JobState::IDLE;
    long startTime = 0;
    long pauseTime = 0;
};
    
