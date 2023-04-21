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
                right_Close = 140,
                left_Close = 50,
                cherry_Close = 50,
                right_Open = 30,
                left_Open = 160,
                cherry_Open = 100,
                right_Grab = 100,
                left_Grab = 90;
            }
            
            namespace BC{
                const int 
                right_Close = 140,
                left_Close = 50,
                cherry_Close = 50,
                right_Open = 30,
                left_Open = 160,
                cherry_Open = 100,
                right_Grab = 100,
                left_Grab = 90;
            }

            namespace CA{
                const int 
                right_Close = 140,
                left_Close = 50,
                cherry_Close = 50,
                right_Open = 30,
                left_Open = 160,
                cherry_Open = 100,
                right_Grab = 100,
                left_Grab = 90;
            }
        }
        namespace cherryPicker{
            const int
            trap_Close = 50,
            trap_Open = 140,
            trap_Grab = 50;
        }
    } // namespace name
    

    struct GripperGroup{
        BistableServo rightGripper;
        BistableServo leftGripper;
        BistableServo cherryLocker;

        GripperGroup(BistableServoProps rightGripProps, BistableServoProps leftGripProps, BistableServoProps cherryLockProps) : rightGripper(rightGripProps), leftGripper(leftGripProps), cherryLocker(cherryLockProps){};
    };


    class Actuators : public JobExecutor {
    private:
        GripperGroup gripperAB; //Right group when facing screen
        GripperGroup gripperBC; //Opposed to the screen
        GripperGroup gripperCA; //Left group when facing screen

        int _pinTurbine;

    public:
        BistableServo trap;

    public:
        Actuators();
        ~Actuators();

        void Initialize();

        void Lock   (RobotCompass rc); //Lock Cherry locker
        void Unlock (RobotCompass rc); //Unlock Cherry locker

        void Close  (RobotCompass rc);
        void Open   (RobotCompass rc);
        void Grab   (RobotCompass rc);
        void Ungrab (RobotCompass rc);
        void Applause(RobotCompass rc);

        void SetTurbine(int speed); //in %
        void StopTurbine();
        void SuckBall();
        void DropBall();

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

