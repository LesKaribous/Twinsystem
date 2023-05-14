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
    void start();
    void pause();
    void resume();
    void cancel();
    void complete();

protected:
    JobState m_state = JobState::IDLE;
};
    
