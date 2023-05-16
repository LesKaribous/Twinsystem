#pragma once
#include "core/lib.h"


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
    bool isPending() const ;
    bool isPaused() const ;
    bool isCancelled() const ;
    bool isCompleted() const ;

    
    void update(); 

    void reset();   //Set to IDLE
    void start();   //Set to PENDING
    void pause();   //Set to PAUSED
    void resume();  //Set to PENDING
    void cancel();  //Set to CANCELLED
    void complete();//Set to COMPLETED

protected:
    JobState m_state = JobState::IDLE;
};
    
