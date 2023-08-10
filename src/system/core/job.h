#pragma once
#include "system/core/lib.h"


enum class JobState{
    IDLE,
    PAUSED,
    PENDING,
    CANCELLED,
    COMPLETED
};


class Job  {
public:
    Job();
    ~Job();// make the class polymorphic by providing a virtual destructor

    String toString() const ;
    virtual bool isPending();
    virtual bool isIdle();
    virtual bool isPaused();
    virtual bool isCancelled();
    virtual bool isCompleted();

    virtual void run() = 0; 
    virtual void reset();   //Set to IDLE
    virtual void start();   //Set to PENDING
    virtual void pause();   //Set to PAUSED
    virtual void resume();  //Set to PENDING
    virtual void cancel();  //Set to CANCELLED
    virtual void complete();//Set to COMPLETED

protected:
    JobState m_state = JobState::IDLE;
};
    
