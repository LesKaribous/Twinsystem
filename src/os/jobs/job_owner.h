#pragma once
#include "os/jobs/job.h"

class JobOwner{
protected:
    Job* m_job = nullptr;

public:
    JobOwner();
    inline bool hasJob()const{return m_job != nullptr;} 
    inline JobState state(){return m_job->state();}

    virtual bool isIdle()const;
    virtual bool isPausing()const;
    virtual bool isPaused()const;
    virtual bool isRunning()const;
    virtual bool isPending()const;
    virtual bool isBusy()const;
    virtual bool isCompleted()const;
    virtual bool isCanceling()const;
    virtual bool isCanceled()const;

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

};

