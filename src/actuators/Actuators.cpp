#include "Actuators.h"

namespace TwinSystem{

    Actuators::Actuators() : gripperAB( {Pin::Servo::ServoB3, _GS_::AB::right_Open, _GS_::AB::right_Close, _GS_::AB::right_Grab}, 
                                        {Pin::Servo::ServoB2, _GS_::AB::left_Open,  _GS_::AB::left_Close, _GS_::AB::left_Grab}, 
                                        {Pin::Servo::ServoB1, _GS_::AB::elevator_Down, _GS_::AB::elevator_Up, _GS_::AB::elevator_Grab}),

                             gripperBC( {Pin::Servo::ServoA3, _GS_::BC::right_Open, _GS_::BC::right_Close, _GS_::BC::right_Grab}, 
                                        {Pin::Servo::ServoA2, _GS_::BC::left_Open,  _GS_::BC::left_Close, _GS_::BC::left_Grab}, 
                                        {Pin::Servo::ServoA1, _GS_::BC::elevator_Down, _GS_::BC::elevator_Up, _GS_::BC::elevator_Grab}),

                             gripperCA( {Pin::Servo::ServoC3, _GS_::CA::right_Open, _GS_::CA::right_Close, _GS_::CA::right_Grab}, 
                                        {Pin::Servo::ServoC2, _GS_::CA::left_Open,  _GS_::CA::left_Close, _GS_::CA::left_Grab}, 
                                        {Pin::Servo::ServoC1, _GS_::CA::elevator_Down, _GS_::CA::elevator_Up, _GS_::CA::elevator_Grab})
        {
            EnableTraco();
            gripperAB.rightGripper.open();
            gripperAB.leftGripper.open();
            gripperAB.elevator.open();

            gripperBC.rightGripper.open();
            gripperBC.leftGripper.open();
            gripperBC.elevator.open();

            gripperCA.rightGripper.open();
            gripperCA.leftGripper.open();
            gripperCA.elevator.open();

            delay(2000);
        }

    Actuators::~Actuators(){
    }

    void Actuators::Initialize(){
        pinMode(Pin::Outputs::enTraco,OUTPUT); //Enable Traco to enable Servos
        EnableTraco();
        gripperAB.elevator.enable();
        gripperAB.leftGripper.enable();
        gripperAB.rightGripper.enable();
        gripperBC.elevator.enable();
        gripperBC.leftGripper.enable();
        gripperBC.rightGripper.enable();
        gripperCA.elevator.enable();
        gripperCA.leftGripper.enable();
        gripperCA.rightGripper.enable();
        
        Open(RobotCompass::AB);
        Open(RobotCompass::BC);
        Open(RobotCompass::CA);
        
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

    void Actuators::GoUp(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            gripperAB.elevator.close();
            break;

        case RobotCompass::BC :
            gripperBC.elevator.close();
            break;

        case RobotCompass::CA :
            gripperCA.elevator.close();
            break;
        
        default:
            break;
        }
    }

    void Actuators::GoDown(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            gripperAB.elevator.open();
            break;

        case RobotCompass::BC :
            gripperBC.elevator.open();
            break;

        case RobotCompass::CA :
            gripperCA.elevator.open();
            break;
        
        default:
            break;
        }
    }

    void Actuators::GoGrab(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            gripperAB.elevator.grab();
            break;

        case RobotCompass::BC :
            gripperBC.elevator.grab();
            break;

        case RobotCompass::CA :
            gripperCA.elevator.grab();
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

    void Actuators::EnableTraco(){
        digitalWrite(Pin::Outputs::enTraco,HIGH);
    }

    void Actuators::DisableTraco(){
        digitalWrite(Pin::Outputs::enTraco,LOW);
    }
    
    void Actuators::Sleep(){
        gripperAB.elevator.sleep();
        gripperAB.leftGripper.sleep();
        gripperAB.rightGripper.sleep();
        gripperBC.elevator.sleep();
        gripperBC.leftGripper.sleep();
        gripperBC.rightGripper.sleep();
        gripperCA.elevator.sleep();
        gripperCA.leftGripper.sleep();
        gripperCA.rightGripper.sleep();
    }

    void Actuators::Engage(){
        EnableTraco();
        gripperAB.elevator.enable();
        gripperAB.leftGripper.enable();
        gripperAB.rightGripper.enable();
        gripperBC.elevator.enable();
        gripperBC.leftGripper.enable();
        gripperBC.rightGripper.enable();
        gripperCA.elevator.enable();
        gripperCA.leftGripper.enable();
        gripperCA.rightGripper.enable();
    }

    void Actuators::Disengage(){
        DisableTraco();
        gripperAB.elevator.disable();
        gripperAB.leftGripper.disable();
        gripperAB.rightGripper.disable();
        gripperBC.elevator.disable();
        gripperBC.leftGripper.disable();
        gripperBC.rightGripper.disable();
        gripperCA.elevator.disable();
        gripperCA.leftGripper.disable();
        gripperCA.rightGripper.disable();
    }

}