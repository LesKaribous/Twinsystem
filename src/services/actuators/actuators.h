#pragma once
#include "services/service.h"
#include "advancedServo.h"

#include "utils/job.h"
#include "utils/geometry.h"
#include "pin.h"

struct ActuatorsProperty{
const int 
rightServoPin,
leftServoPin,
elevatorServoPin,
rightSensorPin,
leftSensorPin;

//position servo
const int 
    right_Close,
    right_Open,
    right_Grab,
    left_Close,
    left_Open,
    left_Grab,
    elevator_Down,
    elevator_Up,
    elevator_Grab,
    elevator_Border;
};

namespace ActuatorsPreset{
    const ActuatorsProperty AB = {
        //pins
        Pin::Servo::ServoB3, //rightServoPin
        Pin::Servo::ServoB2, //leftServoPin
        Pin::Servo::ServoB1, //elevatorServoPin
        Pin::Sensor::SensorRight_AB, 
        Pin::Sensor::SensorLeft_AB,
        //position servo
        100, //right_Close
        10,  //right_Open   
        180, //right_Grab   
        100, //left_Close   
        10,  //left_Open    
        180, //left_Grab    
        15,  //elevator_Down
        160, //elevator_Up  
        35,   //elevator_Grab
        90   //elevator_Border
    };
        
    const ActuatorsProperty BC = {
        //pins
        Pin::Servo::ServoA3, //rightServoPin
        Pin::Servo::ServoA2, //leftServoPin
        Pin::Servo::ServoA1, //elevatorServoPin
        Pin::Sensor::SensorRight_BC, 
        Pin::Sensor::SensorLeft_BC,
        //position servo
        80, //right_Close
        10,  //right_Open   
        160, //right_Grab   
        80, //left_Close   
        10,  //left_Open    
        160, //left_Grab    
        0,   //elevator_Down
        130, //elevator_Up  
        10,   //elevator_Grab
        60   //elevator_Border
    };

    const ActuatorsProperty CA = {
        //pins
        Pin::Servo::ServoC3, //rightServoPin
        Pin::Servo::ServoC2, //leftServoPin
        Pin::Servo::ServoC1, //elevatorServoPin
        Pin::Sensor::SensorRight_CA, 
        Pin::Sensor::SensorLeft_CA,
        //position servo
        100, //right_Close
        10,  //right_Open   
        180, //right_Grab   
        100, //left_Close   
        10,  //left_Open    
        180, //left_Grab    
        0,   //elevator_Down
        130, //elevator_Up  
        10,   //elevator_Grab
        60   //elevator_Border
    };
}

enum class ElevatorPose{
    UP = 0,
    DOWN = 1, 
    GRAB = 2,
    BORDER = 3
};

struct ActuatorGroup{
    GripperServo rightGripper; 
    const int rightSensor;

    GripperServo leftGripper;
    const int leftSensor;

    AdvancedServo elevator;

    ActuatorGroup(ActuatorsProperty presets) : 
        rightGripper(presets.rightServoPin, presets.right_Open, presets.right_Close, presets.right_Grab), 
        rightSensor(presets.rightSensorPin),
        leftGripper(presets.leftServoPin, presets.left_Open, presets.left_Close, presets.left_Grab),  
        leftSensor(presets.leftSensorPin),
        elevator(presets.elevatorServoPin) {
            elevator.setPose(static_cast<int>(ElevatorPose::DOWN), presets.elevator_Down);
            elevator.setPose(static_cast<int>(ElevatorPose::UP), presets.elevator_Up);
            elevator.setPose(static_cast<int>(ElevatorPose::GRAB), presets.elevator_Grab);
            elevator.setPose(static_cast<int>(ElevatorPose::BORDER), presets.elevator_Border);
        }
};

class Actuators : public Service, public Job{
protected:
    ActuatorGroup gripperAB; //Right group when facing screen
    ActuatorGroup gripperBC; //Opposed to the screen
    ActuatorGroup gripperCA; //Left group when facing screen
    
public:
    friend class App;

    Actuators();

    void run(){};
    void sleep();

    void onAttach()override;
    void onUpdate()override;
    void disable()override;
    void enable()override;

    void close  (RobotCompass rc);
    void open   (RobotCompass rc);
    void grab   (RobotCompass rc);
    void applause(RobotCompass rc);

    void moveElevator(RobotCompass rc, ElevatorPose poseIndex);
    void testSensors();
    bool readSensor(RobotCompass rc, Side gs);

    bool runGrabbing(RobotCompass rc, Side gs = Side::BOTH);
    bool runOpening(RobotCompass rc, Side gs = Side::BOTH);
    bool runClosing(RobotCompass rc, Side gs = Side::BOTH);
    bool runElevatorUp(RobotCompass rc);
    bool runElevatorDown(RobotCompass rc);
    bool runElevatorGrab(RobotCompass rc);
    bool runElevatorBorder(RobotCompass rc);

    void enableTraco();
    void disableTraco();

private : 
    bool moveGripper(AdvancedServo& gripper, int target);
    ActuatorGroup& getActuatorGroup(RobotCompass rc);

    SERVICE(Actuators)
};

EXTERN_DECLARATION(Actuators, actuators)