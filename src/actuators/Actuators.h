#pragma once
#include "core/Core.h"
#include "core/JobExecutor.h"
#include "BistableServo.h"
#include "Pin.h"
#include "math/Geometry.h"

#define _GS_ ActuatorsPresets::Gripper

namespace TwinSystem{
    namespace ActuatorsPresets{
        namespace Gripper{
            namespace AB{
                const int 
                right_Close     = 120,
                right_Open      = 10,
                right_Grab      = 180,
                left_Close      = 120,
                left_Open       = 10,
                left_Grab       = 180,
                elevator_Down   = 15,
                elevator_Up     = 160,
                elevator_Grab   = 50;
                
            }
            
            namespace BC{
                const int 
                right_Close     = 100,
                right_Open      = 10,
                right_Grab      = 160,
                left_Close      = 100,
                left_Open       = 10,
                left_Grab       = 160,
                elevator_Down   = 0,
                elevator_Up     = 130,
                elevator_Grab   = 20;
            }

            namespace CA{
                const int 
                right_Close     = 120,
                right_Open      = 10,
                right_Grab      = 180,
                left_Close      = 120,
                left_Open       = 10,
                left_Grab       = 180,
                elevator_Down   = 0,
                elevator_Up     = 130,
                elevator_Grab   = 25;
            }
        }
    }
    

    struct GripperGroup{
        BistableServo rightGripper;
        BistableServo leftGripper;
        BistableServo elevator;

        int rightSensor;
        int leftSensor;

        GripperGroup(BistableServoProps rightGripProps, int rightSensorPin,
                     BistableServoProps leftGripProps, int leftSensorPin,
                     BistableServoProps elevatorProps)
            : rightGripper(rightGripProps), rightSensor(rightSensorPin),
              leftGripper(leftGripProps),   leftSensor(leftSensorPin),
              elevator(elevatorProps) {}
    };

    class Actuators : public JobExecutor {
    private:
        GripperGroup gripperAB; //Right group when facing screen
        GripperGroup gripperBC; //Opposed to the screen
        GripperGroup gripperCA; //Left group when facing screen

        GripperGroup& getGripperGroup(RobotCompass rc);

    public:
        Actuators();
        ~Actuators();

        void Initialize();

        void GoUp   (RobotCompass rc);
        void GoDown (RobotCompass rc);
        void GoGrab (RobotCompass rc);

        void Close  (RobotCompass rc);
        void Open   (RobotCompass rc);
        void Grab   (RobotCompass rc);
        void Ungrab (RobotCompass rc);

        void TestSensors();
        bool ReadSensor(RobotCompass rc, GripperSide gs);

        bool moveGripper(BistableServo& gripper, int target);
        bool runGrabbing(RobotCompass rc);
        bool runGrabbing(RobotCompass rc, GripperSide gs);
        bool runOpening(RobotCompass rc);
        bool runOpening(RobotCompass rc, GripperSide gs);
        bool runClosing(RobotCompass rc);
        bool runClosing(RobotCompass rc, GripperSide gs);
        bool runElevatorUp(RobotCompass rc);
        bool runElevatorDown(RobotCompass rc);
        bool runElevatorGrab(RobotCompass rc);

        void EnableTraco();
        void DisableTraco();
        void Sleep();
        void Engage();
        void Disengage();

        //Inherited
        void Run() override;
        void Pause() override;
        void Resume() override;
        void Cancel() override;
        void Finish() override;

    };

} // namespace TwinSystem

