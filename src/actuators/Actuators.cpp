#include "Actuators.h"

namespace TwinSystem{

    Actuators::Actuators() : GripperAB( {Pin::Servo::ServoA1, _GS_::AB::right_Open, _GS_::AB::right_Close, _GS_::AB::right_Grab}, 
                                        {Pin::Servo::ServoA3, _GS_::AB::left_Open,  _GS_::AB::left_Close, _GS_::AB::left_Grab}, 
                                        {Pin::Servo::ServoA2, _GS_::AB::cherry_Open, _GS_::AB::cherry_Close}),

                             GripperBC( {Pin::Servo::ServoB1, _GS_::BC::right_Open, _GS_::BC::right_Close, _GS_::BC::right_Grab}, 
                                        {Pin::Servo::ServoB3, _GS_::BC::left_Open,  _GS_::BC::left_Close, _GS_::BC::left_Grab}, 
                                        {Pin::Servo::ServoB2, _GS_::BC::cherry_Open, _GS_::BC::cherry_Close} ),

                             GripperCA( {Pin::Servo::ServoC1, _GS_::CA::right_Open, _GS_::CA::right_Close, _GS_::CA::right_Grab}, 
                                        {Pin::Servo::ServoC3, _GS_::CA::left_Open,  _GS_::CA::left_Close, _GS_::CA::left_Grab}, 
                                        {Pin::Servo::ServoC2, _GS_::CA::cherry_Open, _GS_::CA::cherry_Close} )
                                        
                              //Vacuum(   {Pin::Servo::, _GS_::, _GS_::})
        {
            GripperAB.rightGripper.close();
            GripperAB.leftGripper.close();
            GripperAB.cherryLocker.close();

            GripperBC.rightGripper.close();
            GripperBC.leftGripper.close();
            GripperBC.cherryLocker.close();

            GripperCA.rightGripper.close();
            GripperCA.leftGripper.close();
            GripperCA.cherryLocker.close();
        
        }

    Actuators::~Actuators(){
    }


    void Actuators::lock(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            GripperAB.cherryLocker.close();
            break;

        case RobotCompass::BC :
            GripperBC.cherryLocker.close();
            break;

        case RobotCompass::CA :
            GripperCA.cherryLocker.close();
            break;
        
        default:
            break;
        }
    }

    void Actuators::unlock(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            GripperAB.cherryLocker.open();
            break;

        case RobotCompass::BC :
            GripperBC.cherryLocker.open();
            break;

        case RobotCompass::CA :
            GripperCA.cherryLocker.open();
            break;
        
        default:
            break;
        }
    }

    void Actuators::close(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            GripperAB.rightGripper.close();
            GripperAB.leftGripper.close();
            break;

        case RobotCompass::BC :
            GripperBC.rightGripper.close();
            GripperBC.leftGripper.close();
            break;

        case RobotCompass::CA :
            GripperCA.rightGripper.close();
            GripperCA.leftGripper.close();
            break;
        
        
        default:
            break;
        }
    }

    void Actuators::open(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            GripperAB.rightGripper.open();
            GripperAB.leftGripper.open();
            break;

        case RobotCompass::BC :
            GripperBC.rightGripper.open();
            GripperBC.leftGripper.open();
            break;

        case RobotCompass::CA :
            GripperCA.rightGripper.open();
            GripperCA.leftGripper.open();
            break;
        
        default:
            break;
        }
    }

    void Actuators::grab(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            GripperAB.rightGripper.grab();
            GripperAB.leftGripper.grab();
            break;

        case RobotCompass::BC :
            GripperBC.rightGripper.grab();
            GripperBC.leftGripper.grab();
            break;

        case RobotCompass::CA :
            GripperCA.rightGripper.grab();
            GripperCA.leftGripper.grab();
            break;
        
        default:
            break;
        }
    }

    void Actuators::ungrab(RobotCompass rc){
        open(rc);
    }

    void Actuators::applause(RobotCompass rc){
        open(rc); unlock(rc);
        delay(500);
        grab(rc);lock(rc);
        delay(500);
        open(rc); unlock(rc);
        delay(500);
        grab(rc);lock(rc);
        delay(500);
        open(rc); unlock(rc);
    }
}