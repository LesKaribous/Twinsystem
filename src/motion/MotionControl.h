#pragma once

#include "math/Geometry.h"
#include "event/Event.h"
#include "motion/StepperController.h"

namespace TwinSystem{

    class MotionControl{
    public:
        enum class State{
            IDLE,
            RUNNING,
            PAUSED,
            CANCELLED,
            ARRIVED,
        };

        /**
         * @brief Constructor
         */
        MotionControl(Shared<StepperController> stps);
        ~MotionControl();

        void Run();
        void Pause();
        void Resume();

        void OnEvent(Event& e);
        
        void DebugState();

        /**
         * @brief Execute this routine while moving.
         */
        void ComputeSync();

        /**
         * @brief updatePosition by calculating foward kinematics. (In realtime folk's B-D )
         */
        void UpdatePosition();

        /**
         * @brief Turn robot arround its center by desired angle
         * @param angle angle in degrees
         */
        void Turn(float);

        /**
         * @brief Move the robot using polar coordinates
         * @param heading move direction
         * @param length distance to go
         */
        void GoPolar(float heading, float length);
        
        /**
         * @brief Move the robot using cartesian coordinates
         * @param x X coordinate
         * @param y Y coordinate
         */
        void Go(float x, float y);
        void Go(Vec2);

        /**
         * @brief Move the robot using cartesian coordinates & turn by desired angle
         * @param x X coordinate
         * @param y Y coordinate
         * @param w Y coordinate
         */
        void GoTurn(float x, float y, float w);
        void GoTurn(Vec3);

        /**
         * @brief Set the robot position using position of the bumped border
         * @param border position vector of the border relative to the robot current position
         */
        void ProbeBorder(TableCompass, RobotCompass);

        /**
         * @brief Align (turn) the robot toward the desired vector
         * @param target the target vector to align with
         * @param orientation offset angle to align specific face of the robot
         */
        void Align(RobotCompass, float orientation);

        /**
         * @brief Move the robot to the desired target (last params of the Vec3 is used for arc curvature)
         * @param target the target vector to align with
         */
        void Move(Vec3 target);

        /**
         * @brief Return true if a move is running
         */
        bool Running();


        //Setters
        Vec3 SetTarget(Vec3);
        void SetPosition(Vec2);
        void SetPosition(Vec3);
        void SetAbsolute(bool = true);
        void SetRelative(bool = true);
        void SetAvoidance(bool = true);


        //Getters
        Vec3 GetPosition();
        Vec3 GetTarget();
        Vec3 GetAbsTarget();
        bool isAbsolute();
        bool isRelative();
        bool isProbed();
        bool isXProbed();
        bool isYProbed();
        //Return true while probing
        bool isProbing();


    private:
        State cState = State::IDLE;

        Vec3 cPosition 		= {-1,-1,0};
        Vec3 cStartPosition = {0,0,0};
        Vec3 cTarget 		= {0,0,0};
        Vec3 calibration 	= {1,1,1};

        Vec2 controlPoint   = {0,0};

        bool probedX = false, probedY = false;
        bool absolute = true;
        bool probing = false;

        bool avoidance = false;

        Shared<StepperController> controller;

    };
}