#pragma once

#include "core/JobExecutor.h"
#include "math/Geometry.h"
#include "event/Event.h"
#include "motion/StepperController.h"

namespace TwinSystem{

    class MotionControl : public JobExecutor{
    public:
        /**
         * @brief Constructor
         */
        MotionControl();
        ~MotionControl();

        void Initialize();

        void OnEvent(Event& e);
        void UpdatePosition();

        void TurnAwait(float);
        void TurnAsync(float);

        void GoAwait(Vec2);
        void GoAsync(Vec2);
        void GoAwait(float x, float y);
        void GoAsync(float x, float y);

        void AlignAwait(RobotCompass, float orientation);
        void AlignAsync(RobotCompass, float orientation);

        void OptmizeTarget();
        Vec3 TargetToSteps(Vec3 relativeTarget);
        Vec3 ToRelativeTarget(Vec3 absTarget);
        Vec3 ToAbsoluteTarget(Vec3 absTarget);

    private:
        void MoveAwait(Vec3 target);
        void MoveAsync(Vec3 target);

    public:
        //Setters
        void SetAbsTarget(Vec3);    //mm, mm, rad
        void SetAbsPosition(Vec3);  //mm, mm, rad

        void SetAbsolute();
        void SetRelative();
 
        //Getters
        Vec3 GetAbsPosition() const;
        Vec3 GetAbsTarget() const;

        bool IsAbsolute() const;
        bool IsRelative() const;

        //Inherited
        void Run() override;
        void Pause() override;
        void Resume() override;
        void Cancel() override;
        void Finish() override;

    public : 
        StepperController steppers;
         
    private:

        Vec3 _startPosition  = { 0, 0, 0}; //Absolute mm, mm, rad
        Vec3 _position       = {-1,-1, 0}; //Absolute mm, mm, rad
        Vec3 _target 	     = { 0, 0, 0}; //Absolute mm, mm, rad
        Vec3 _relTarget      = { 0, 0, 0}; //Relative mm, mm, rad

        Vec3 _calibration 	 = { 1, 1, 1};
        Vec2 _controlPoint   = { 0, 0};

        bool _absolute = true;

    };
}