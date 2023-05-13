#pragma once
#include "core/lib.h"


enum class JobState{
    IDLE,
    PAUSED,
    PENDING,
    CANCELLED,
    FINISHED
};


class Job  {
public:

    String toString();
    virtual bool isPending();
    virtual bool isPaused();
    virtual bool isCancelled();

    virtual void update() = 0;
    virtual void pause();
    virtual void resume();
    virtual void cancel();
    virtual void finish();

    virtual ~Job() {} // make the class polymorphic by providing a virtual destructor

protected:
    JobState m_state = JobState::IDLE;
};
    
