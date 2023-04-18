#pragma once
#include <Arduino.h>

namespace TwinSystem{

    enum class JobState{
        IDLE,
        RUNNING,
        PAUSED,
        CANCELLED,
        FINISHED
    };


    class JobExecutor  {
    public:

        String ToString();
        virtual bool IsBusy();
        virtual bool IsRunning();
        virtual bool IsIdle();
        virtual bool IsPaused();
        virtual bool IsCancelled();

        virtual void Run() = 0;
        virtual void Pause();
        virtual void Resume();
        virtual void Cancel();
        virtual void Finish();

        virtual ~JobExecutor() {} // make the class polymorphic by providing a virtual destructor

    protected:
        JobState _state = JobState::IDLE;
    };
    
} // namespace TwinSystem
