#pragma once
#include <Arduino.h>
//#include "os/asyncExecutor.h"

enum class JobState{
    IDLE,
    PAUSING,
    PAUSED,
    RUNNNING,
    CANCELING,
    CANCELED,
    COMPLETED
};


class Job  {
public:
    Job();
    ~Job();// make the class polymorphic by providing a virtual destructor

    String toString() const;
    //operator AsyncExecutor() { OS::instance().waitUntil(*this); };

    bool isIdle()const;
    bool isPausing()const;
    bool isPaused()const;
    bool isRunning()const;
    bool isPending()const;
    bool isBusy()const;
    bool isCompleted()const;
    bool isCanceling()const;
    bool isCanceled()const;

    inline explicit operator bool() const {
        return isCompleted();  // Return true if the job succeeded
    }

    long elaspedTime() const;
    long pauseDuration() const;

    virtual void run(); 
    virtual void reset();   //Set to IDLE
    virtual void start();   //Set to PENDING
    virtual void pause();   //Set to PAUSING
    virtual void resume();  //Set to PENDING
    virtual void cancel();  //Set to CANCELING
    virtual void forceCancel();  //Set to CANCELED
    virtual void complete();//Set to COMPLETED
    
    virtual void onPausing();   //Called every run if in Pausing state
    virtual void onCanceling(); //Called every run if in  exiting Pausing state

    virtual void onPaused();   //Called once when exiting Pausing state
    virtual void onCanceled(); //Called once when exiting Canceling state


protected:
    JobState m_state = JobState::IDLE;
    long startTime = 0;
    long pauseTime = 0;
};
    
