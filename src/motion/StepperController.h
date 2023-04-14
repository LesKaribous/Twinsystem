#pragma once

#include "core/Core.h"
#include <TeensyStep.h>
#include <stdint.h>

#include "Pin.h"
#include "event/Event.h"
#include "math/Geometry.h"

#define SLOW 30
#define FAST 100

namespace TwinSystem{
    class StepperController{
    private:
        bool engaged, sleeping;
        StepControl controller;

        Stepper sA;
        Stepper sB;
        Stepper sC;

        int32_t sA_target,
                sB_target,
                sC_target;

        Vec3 calibration;
        u_int32_t speed, accel;

        float feedrate;
        bool wasLastAsync;
        bool _initialized = false;

    public:
        StepperController();
        ~StepperController(){};

        void Initialize();

        void OnEvent(Event& e);

        void Move(Vec3 target, bool async = false);

        void Stop   (bool async = true);
        void Resume ();

        void Update();
        void Reset();
        void ResetPosition();

        void EmergencyStop();

        void SetSpeed(int speed);
        void SetAccel(int accel);
        void SetCalibration(bool state);
        void SetFeedrate(float feed);

        u_int32_t GetAccel();
        u_int32_t GetSpeed();

        Vec3 GetPosition();

        bool isArrived();
        bool isRunning();
        bool isSleeping();

        // Engaging motors make them ready to move.
        // Note : Motors may be engaged but sleeping !
        void Engage(bool state = true);
        // Disengaging motors turn them off. They cannot move at all, not powered at all.
        void Disengage();

        //Sleep mode is used to save battery or let the robot move freely. It disable both motors.
        //Note : A move request will exit sleep mode and turn them on.
        void Sleep(bool state = true);
    };


}





