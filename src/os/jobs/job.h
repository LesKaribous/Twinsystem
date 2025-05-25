#pragma once
#include <Arduino.h>
//#include <memory>
//#include "os/asyncExecutor.h"

enum class JobState{
    IDLE,
    PAUSING,
    PAUSED,
    RUNNING,
    CANCELING,
    CANCELED,
    COMPLETED
};
//class JobOwner;

class Job  {
    //JobOwner* m_owner = nullptr;
    //bool m_blocking = false;
public:
    //Job(JobOwner* = nullptr);
    Job();
    ~Job();// make the class polymorphic by providing a virtual destructor

    String toString() const;
    //operator AsyncExecutor() { OS::instance().waitUntil(*this); };

    inline JobState state(){return m_state;}

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

    virtual void exec(); 
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

    //bool hasOwner() const { return m_owner != nullptr; }
    //bool isBlocking() const { return m_blocking; }
    //void setBlocking(bool blocking) { m_blocking = blocking; }

    //inline static std::unique_ptr<Job> create() {
    //    return std::make_unique<Job>();
    //}
    //void track(Job* job); //to update the job pointer for the owner

protected:
    JobState m_state = JobState::IDLE;
    long startTime = 0;
    long pauseTime = 0;
};
    
//typedef std::unique_ptr<Job> JobHandle;