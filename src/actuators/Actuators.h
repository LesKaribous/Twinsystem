#pragma once
#include "core/Core.h"
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
                cherry_Open = 100;
            }
            
            namespace BC{
                const int 
                right_Close = 140,
                left_Close = 50,
                cherry_Close = 50,
                right_Open = 30,
                left_Open = 160,
                cherry_Open = 100;
            }

            namespace CA{
                const int 
                right_Close = 140,
                left_Close = 50,
                cherry_Close = 50,
                right_Open = 30,
                left_Open = 160,
                cherry_Open = 100;
            }
        }
    } // namespace name
    

    struct GripperGroup{
        BistableServo rightGripper;
        BistableServo leftGripper;
        BistableServo cherryLocker;

        GripperGroup(BistableServoProps rightGripProps, BistableServoProps leftGripProps, BistableServoProps cherryLockProps) : rightGripper(rightGripProps), leftGripper(leftGripProps), cherryLocker(cherryLockProps){

        };
    };

    struct VacuumGroup{
        BistableServo cherryLocker;

        VacuumGroup(BistableServoProps Props);
        //Turbine t;
        //Pump p;
    };
    
    class Actuators{
    private:
        GripperGroup GripperAB; //Right group when facing screen
        GripperGroup GripperBC; //Opposed to the screen
        GripperGroup GripperCA; //Left group when facing screen
        //VacuumGroup Vacuum;
    public:
        Actuators();
        ~Actuators();

        void lock   (RobotCompass rc); //Lock Cherry locker
        void unlock (RobotCompass rc); //Unlock Cherry locker

        void close  (RobotCompass rc);
        void open   (RobotCompass rc);
        void grab   (RobotCompass rc);
        void ungrab (RobotCompass rc);
        void applause(RobotCompass rc);


    };


} // namespace TwinSystem

