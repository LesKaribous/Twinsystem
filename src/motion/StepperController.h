#pragma once

#include "core/Core.h"
#include <TeensyStep.h>
#include <stdint.h>

#include "core/JobExecutor.h"
#include "Pin.h"
#include "event/Event.h"
#include "math/Geometry.h"

#define SLOW 30
#define FAST 100

namespace TwinSystem{
    class StepperController : public JobExecutor{
    private:
        bool _engaged, _sleeping;
        StepControl _controller;

        Stepper _sA;
        Stepper _sB;
        Stepper _sC;

        int32_t _sA_target,
                _sB_target,
                _sC_target,
                _speed, 
                _accel;

        Vec3 _calibration;

        float _feedrate;
        bool _async = true;

    public:
        StepperController();
        ~StepperController(){};

        void Initialize();
        void OnEvent(Event& e);

        void EnableAsync();
        void DisableAsync();
        
        void Move(Vec3 target);

        bool HasFinished();

        void Reset();
        void EmergencyStop();

        void SetSpeed(int32_t speed);
        void SetAccel(int32_t accel);
        void SetCalibration(bool state);
        void SetFeedrate(float feed);

        u_int32_t GetAccel();
        u_int32_t GetSpeed();

        Vec3 GetPosition();

        bool IsSleeping();
        bool IsAsync();

        // Engaging motors make them ready to move. Motors may be engaged but sleeping !
        void Engage();
        // Disengaging motors turn them off. They cannot move at all.
        void Disengage();

        //Sleep mode is used to save battery or let the robot move freely. It disable both motors.
        //Note : A move request will exit sleep mode and turn them on.
        void WakeUp();
        void Sleep();

        //Inherited
        void Run() override;
        void Pause() override;
        void Resume() override;
        void Cancel() override;
        void Finish() override;

    };


}





