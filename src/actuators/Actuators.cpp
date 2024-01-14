#include "Actuators.h"

namespace TwinSystem{

    Actuators::Actuators() : //gripperAB( {Pin::Servo::ServoA1, _GS_::AB::right_Open, _GS_::AB::right_Close, _GS_::AB::right_Grab}, 
                                        //{Pin::Servo::ServoA3, _GS_::AB::left_Open,  _GS_::AB::left_Close, _GS_::AB::left_Grab}, 
                                        //{Pin::Servo::ServoA2, _GS_::AB::cherry_Open, _GS_::AB::cherry_Close}),

                             gripperBC( {Pin::Servo::ServoB1, _GS_::BC::right_Open, _GS_::BC::right_Close, _GS_::BC::right_Grab}, 
                                        {Pin::Servo::ServoB3, _GS_::BC::left_Open,  _GS_::BC::left_Close, _GS_::BC::left_Grab}, 
                                        {Pin::Servo::ServoB2, _GS_::BC::cherry_Open, _GS_::BC::cherry_Close} ),

                             gripperCA( {Pin::Servo::ServoC1, _GS_::CA::right_Open, _GS_::CA::right_Close, _GS_::CA::right_Grab}, 
                                        {Pin::Servo::ServoC3, _GS_::CA::left_Open,  _GS_::CA::left_Close, _GS_::CA::left_Grab}, 
                                        {Pin::Servo::ServoC2, _GS_::CA::cherry_Open, _GS_::CA::cherry_Close}),

                            trap(Pin::CherryPicker::pinServoTrap, ActuatorsPresets::cherryPicker::trap_Open, ActuatorsPresets::cherryPicker::trap_Close, ActuatorsPresets::cherryPicker::trap_Grab)
        {
            EnableTraco();
            //gripperAB.rightGripper.close();
            //gripperAB.leftGripper.close();
            //gripperAB.cherryLocker.close();

            gripperBC.rightGripper.close();
            gripperBC.leftGripper.close();
            gripperBC.cherryLocker.close();

            gripperCA.rightGripper.close();
            gripperCA.leftGripper.close();
            gripperCA.cherryLocker.close();
            
            _pinTurbine = Pin::CherryPicker::pinTurbine;

            trap.close();

            elevator.attach(Pin::Servo::ServoA1);
            arm.attach(Pin::Servo::ServoA2);
            tool.attach(Pin::Servo::ServoA3);
            elevator.write(ActuatorsPresets::arm::elevator_down);
            arm.write(ActuatorsPresets::arm::arm_up);
            tool.write(ActuatorsPresets::arm::tool_up);

            delay(2000);
        }

    Actuators::~Actuators(){
    }

    void Actuators::Initialize(){
        pinMode(Pin::Outputs::enTraco,OUTPUT); //Enable Traco to enable Servo
        EnableTraco();
        //gripperAB.cherryLocker.enable();
        //gripperAB.leftGripper.enable();
        //gripperAB.rightGripper.enable();
        gripperBC.cherryLocker.enable();
        gripperBC.leftGripper.enable();
        gripperBC.rightGripper.enable();
        gripperCA.cherryLocker.enable();
        gripperCA.leftGripper.enable();
        gripperCA.rightGripper.enable();
        trap.enable();
        pinMode(_pinTurbine, OUTPUT);
        

        //Close(RobotCompass::AB);
        Close(RobotCompass::BC);
        Close(RobotCompass::CA);

        moveArm(0, 0, 0);
        
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

    void Actuators::moveArm(int elevator_pos, int arm_pos, int tool_pos){

        int elevator_pos_c = constrain(elevator_pos,0,100);
        int arm_pos_c= constrain(arm_pos,0,100);
        int tool_pos_c = constrain(tool_pos,0,100);

        int elevator_angle = map(elevator_pos_c, 0, 100, ActuatorsPresets::arm::elevator_down, ActuatorsPresets::arm::elevator_up);
        int arm_angle = map(arm_pos_c, 0, 100, ActuatorsPresets::arm::arm_up, ActuatorsPresets::arm::arm_down);
        int tool_angle = map(tool_pos_c, 0, 100, ActuatorsPresets::arm::tool_up, ActuatorsPresets::arm::tool_down);

        elevator.write(elevator_angle);
        arm.write(arm_angle);
        tool.write(tool_angle);

    }

    void Actuators::Lock(RobotCompass rc){
        switch (rc)
        {
        case RobotCompass::AB :
            //gripperAB.cherryLocker.close();
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
            //gripperAB.cherryLocker.open();
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
            //gripperAB.rightGripper.close();
            //gripperAB.leftGripper.close();
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
            //gripperAB.rightGripper.open();
            //gripperAB.leftGripper.open();
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
            //gripperAB.rightGripper.grab();
            //gripperAB.leftGripper.grab();
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

    void Actuators::SetTurbine(int speed){
        analogWrite(_pinTurbine, map(speed, 0, 100, 0, 255));
    }

    void Actuators::StopTurbine(){
        analogWrite(_pinTurbine, 0);
    }

    void Actuators::SuckBall(){
        trap.close();
        SetTurbine(100);
    }

    void Actuators::DropBall(){
        StopTurbine();
        trap.open();
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

    void Actuators::EnableTraco(){
        digitalWrite(Pin::Outputs::enTraco,HIGH);
    }

    void Actuators::DisableTraco(){
        digitalWrite(Pin::Outputs::enTraco,LOW);
    }
    
    void Actuators::Sleep(){
        //gripperAB.cherryLocker.sleep();
        //gripperAB.leftGripper.sleep();
        //gripperAB.rightGripper.sleep();
        gripperBC.cherryLocker.sleep();
        gripperBC.leftGripper.sleep();
        gripperBC.rightGripper.sleep();
        gripperCA.cherryLocker.sleep();
        gripperCA.leftGripper.sleep();
        gripperCA.rightGripper.sleep();
    }


    void Actuators::Engage(){
        EnableTraco();
        //gripperAB.cherryLocker.enable();
        //gripperAB.leftGripper.enable();
        //gripperAB.rightGripper.enable();
        gripperBC.cherryLocker.enable();
        gripperBC.leftGripper.enable();
        gripperBC.rightGripper.enable();
        gripperCA.cherryLocker.enable();
        gripperCA.leftGripper.enable();
        gripperCA.rightGripper.enable();
        digitalWrite(_pinTurbine, LOW);
    }

    void Actuators::Disengage(){
        DisableTraco();
        //gripperAB.cherryLocker.disable();
        //gripperAB.leftGripper.disable();
        //gripperAB.rightGripper.disable();
        gripperBC.cherryLocker.disable();
        gripperBC.leftGripper.disable();
        gripperBC.rightGripper.disable();
        gripperCA.cherryLocker.disable();
        gripperCA.leftGripper.disable();
        gripperCA.rightGripper.disable();
        trap.disable();
        digitalWrite(_pinTurbine, LOW);
    }

}