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
forkElevatorPin,
forkServoPin,
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
    elevator_Border,
    elevator_Planter,
    elevatorFork_Up,
    elevatorFork_Down,
    elevatorFork_Border,
    fork_Close,
    fork_Open,
    fork_Grab;
};

namespace ActuatorsPreset{
    const ActuatorsProperty AB = {
        //pins
        Pin::Servo::ServoB3, //rightServoPin
        Pin::Servo::ServoB2, //leftServoPin
        Pin::Servo::ServoB1, //elevatorServoPin
        Pin::ServoFork::ServoAB_Elevator,
        Pin::ServoFork::ServoAB_Fork,
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
        90,   //elevator_Border
        60,   //elevator_Planter
        100,     //FORK - elevatorFork_Up
        180,     //FORK - elevatorFork_Down
        10,     //FORK - elevatorFork_Border
        120,     //FORK - fork_Close
        12,     //FORK - fork_Open
        60      //FORK - fork_Grab
    };
        
    const ActuatorsProperty BC = {
        //pins
        Pin::Servo::ServoA3, //rightServoPin
        Pin::Servo::ServoA2, //leftServoPin
        Pin::Servo::ServoA1, //elevatorServoPin
        Pin::ServoFork::ServoBC_Elevator,
        Pin::ServoFork::ServoBC_Fork,
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
        60,   //elevator_Border
        30,   //elevator_Planter
        100,     //FORK - elevatorFork_Up
        180,     //FORK - elevatorFork_Down
        10,     //FORK - elevatorFork_Border
        120,     //FORK - fork_Close
        12,     //FORK - fork_Open
        60      //FORK - fork_Grab
    };

    const ActuatorsProperty CA = {
        //pins
        Pin::Servo::ServoC3, //rightServoPin
        Pin::Servo::ServoC2, //leftServoPin
        Pin::Servo::ServoC1, //elevatorServoPin
        Pin::ServoFork::ServoCA_Elevator,
        Pin::ServoFork::ServoCA_Fork,
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
        60,   //elevator_Border
        30,   //elevator_Planter
        100,     //FORK - elevatorFork_Up
        180,     //FORK - elevatorFork_Down
        10,     //FORK - elevatorFork_Border
        120,     //FORK - fork_Close
        12,     //FORK - fork_Open
        60      //FORK - fork_Grab
    };
}

enum class ElevatorPose{
    UP = 0,
    DOWN = 1, 
    GRAB = 2,
    BORDER = 3,
    PLANTER = 4
};

struct ActuatorGroup{
    GripperServo rightGripper; 
    const int rightSensor;

    GripperServo leftGripper;
    const int leftSensor;

    GripperServo forkGripper;

    AdvancedServo elevator;

    AdvancedServo forkElevator;

    ActuatorGroup(ActuatorsProperty presets) :
        rightGripper(presets.rightServoPin, presets.right_Open, presets.right_Close, presets.right_Grab), 
        rightSensor(presets.rightSensorPin),
        leftGripper(presets.leftServoPin, presets.left_Open, presets.left_Close, presets.left_Grab),  
        leftSensor(presets.leftSensorPin),
        forkGripper(presets.forkServoPin, presets.fork_Open, presets.fork_Close, presets.fork_Grab),
        elevator(presets.elevatorServoPin),
        forkElevator(presets.forkElevatorPin) 
        {
            elevator.setPose(static_cast<int>(ElevatorPose::DOWN), presets.elevator_Down);
            elevator.setPose(static_cast<int>(ElevatorPose::UP), presets.elevator_Up);
            elevator.setPose(static_cast<int>(ElevatorPose::GRAB), presets.elevator_Grab);
            elevator.setPose(static_cast<int>(ElevatorPose::BORDER), presets.elevator_Border);
            elevator.setPose(static_cast<int>(ElevatorPose::PLANTER), presets.elevator_Planter);

            forkElevator.setPose(static_cast<int>(ElevatorPose::UP), presets.elevatorFork_Up);
            forkElevator.setPose(static_cast<int>(ElevatorPose::DOWN), presets.elevatorFork_Down);
            forkElevator.setPose(static_cast<int>(ElevatorPose::BORDER), presets.elevatorFork_Border);
        }
};

class Actuators : public Service, public Job{
protected:
    ActuatorGroup gripperAB; // Facing BAU
    ActuatorGroup gripperBC; // Facing Init button
    ActuatorGroup gripperCA; // Facing Tirette
    
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

    void forkUp   (RobotCompass rc);
    void forkDown (RobotCompass rc);
    void forkGrab (RobotCompass rc);
    
    void moveElevator(RobotCompass rc, ElevatorPose poseIndex);
    void moveForkElevator(RobotCompass rc, ElevatorPose poseIndex);
    bool readSensor(RobotCompass rc, Side gs);
    void testSensors();
    
    bool runGrabbing(RobotCompass rc, Side gs = Side::BOTH);
    bool runOpening(RobotCompass rc, Side gs = Side::BOTH);
    bool runClosing(RobotCompass rc, Side gs = Side::BOTH);
    bool runElevatorUp(RobotCompass rc);
    bool runElevatorDown(RobotCompass rc);
    bool runElevatorGrab(RobotCompass rc);
    bool runElevatorBorder(RobotCompass rc);
    bool runElevatorPlanter(RobotCompass rc);

    void enableTraco();
    void disableTraco();

private : 
    bool moveGripper(AdvancedServo& gripper, int target);
    ActuatorGroup& getActuatorGroup(RobotCompass rc);

    SERVICE(Actuators)
};

EXTERN_DECLARATION(Actuators, actuators)