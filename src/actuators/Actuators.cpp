#include "Actuators.h"

namespace TwinSystem{

    Actuators::Actuators() : gripperAB( {Pin::Servo::ServoA1, _GS_::AB::right_Open, _GS_::AB::right_Close, _GS_::AB::right_Grab}, 
                                        {Pin::Servo::ServoA3, _GS_::AB::left_Open,  _GS_::AB::left_Close, _GS_::AB::left_Grab}, 
                                        {Pin::Servo::ServoA2, _GS_::AB::cherry_Open, _GS_::AB::cherry_Close}),

                             gripperBC( {Pin::Servo::ServoB1, _GS_::BC::right_Open, _GS_::BC::right_Close, _GS_::BC::right_Grab}, 
                                        {Pin::Servo::ServoB3, _GS_::BC::left_Open,  _GS_::BC::left_Close, _GS_::BC::left_Grab}, 
                                        {Pin::Servo::ServoB2, _GS_::BC::cherry_Open, _GS_::BC::cherry_Close} ),

                             gripperCA( {Pin::Servo::ServoC1, _GS_::CA::right_Open, _GS_::CA::right_Close, _GS_::CA::right_Grab}, 
                                        {Pin::Servo::ServoC3, _GS_::CA::left_Open,  _GS_::CA::left_Close, _GS_::CA::left_Grab}, 
                                        {Pin::Servo::ServoC2, _GS_::CA::cherry_Open, _GS_::CA::cherry_Close}),

                            trap(Pin::CherryPicker::pinServoTrap, ActuatorsPresets::cherryPicker::trap_Open, ActuatorsPresets::cherryPicker::trap_Close, ActuatorsPresets::cherryPicker::trap_Grab)
        {
            gripperAB.rightGripper.close();
            gripperAB.leftGripper.close();
            gripperAB.cherryLocker.close();

            gripperBC.rightGripper.close();
            gripperBC.leftGripper.close();
            gripperBC.cherryLocker.close();

            gripperCA.rightGripper.close();
            gripperCA.leftGripper.close();
            gripperCA.cherryLocker.close();

            trap.close();
        }

    Actuators::~Actuators(){
    }

    void Actuators::Initialize(){
        gripperAB.cherryLocker.enable();
        gripperAB.leftGripper.enable();
        gripperAB.rightGripper.enable();
        gripperBC.cherryLocker.enable();
        gripperBC.leftGripper.enable();
        gripperBC.rightGripper.enable();
        gripperCA.cherryLocker.enable();
        gripperCA.leftGripper.enable();
        gripperCA.rightGripper.enable();

        Close(RobotCompass::AB);
        Close(RobotCompass::BC);
        Close(RobotCompass::CA);

        delay(2000);

        Sleep();

    }

    //Inherited
    void Actuators::Run(){
        //Do something...
    }

    void Actuators::Pause(){
        JobExecutor::Pause();
    }

    void Actuators::Resume(){
        JobExecutor::Resume();
    }

    void Actuators::Cancel(){
        JobExecutor::Cancel();
    }

    void Actuators::Finish(){
        JobExecutor::Finish();
    }

    void Actuators::Lock(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            gripperAB.cherryLocker.close();
            break;

        case RobotCompass::BC :
            gripperBC.cherryLocker.close();
            break;

        case RobotCompass::CA :
            gripperCA.cherryLocker.close();
            break;
        
        default:
            break;
        }
    }

    void Actuators::Unlock(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            gripperAB.cherryLocker.open();
            break;

        case RobotCompass::BC :
            gripperBC.cherryLocker.open();
            break;

        case RobotCompass::CA :
            gripperCA.cherryLocker.open();
            break;
        
        default:
            break;
        }
    }

    void Actuators::Close(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            gripperAB.rightGripper.close();
            gripperAB.leftGripper.close();
            break;

        case RobotCompass::BC :
            gripperBC.rightGripper.close();
            gripperBC.leftGripper.close();
            break;

        case RobotCompass::CA :
            gripperCA.rightGripper.close();
            gripperCA.leftGripper.close();
            break;
        
        
        default:
            break;
        }
    }

    void Actuators::Open(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            gripperAB.rightGripper.open();
            gripperAB.leftGripper.open();
            break;

        case RobotCompass::BC :
            gripperBC.rightGripper.open();
            gripperBC.leftGripper.open();
            break;

        case RobotCompass::CA :
            gripperCA.rightGripper.open();
            gripperCA.leftGripper.open();
            break;
        
        default:
            break;
        }
    }

    void Actuators::Grab(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            gripperAB.rightGripper.grab();
            gripperAB.leftGripper.grab();
            break;

        case RobotCompass::BC :
            gripperBC.rightGripper.grab();
            gripperBC.leftGripper.grab();
            break;

        case RobotCompass::CA :
            gripperCA.rightGripper.grab();
            gripperCA.leftGripper.grab();
            break;
        
        default:
            break;
        }
    }

    void Actuators::Ungrab(RobotCompass rc){
        Open(rc);
    }

    void Actuators::Applause(RobotCompass rc){
        Open(rc); Unlock(rc);
        delay(500);
        Grab(rc); Lock(rc);
        delay(500);
        Open(rc); Unlock(rc);
        delay(500);
        Grab(rc); Lock(rc);
        delay(500);
        Open(rc); Unlock(rc);
    }
    
    void Actuators::Sleep(){
        gripperAB.cherryLocker.sleep();
        gripperAB.leftGripper.sleep();
        gripperAB.rightGripper.sleep();
        gripperBC.cherryLocker.sleep();
        gripperBC.leftGripper.sleep();
        gripperBC.rightGripper.sleep();
        gripperCA.cherryLocker.sleep();
        gripperCA.leftGripper.sleep();
        gripperCA.rightGripper.sleep();
    }
}