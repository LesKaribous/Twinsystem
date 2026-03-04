#pragma once
#include "services/service.h"
#include "services/actuators/actuatorGroup.h"
#include "utils/geometry.h"
#include "os/jobs/job.h"
#include "services/actuators/groups.h"

enum class ServoIDs : int{
    GRABBER_RIGHT = 0,
    ELEVATOR = 1,
    GRABBER_LEFT = 2,
    HUGGER_ELEVATOR = 3,
    HUGGER_GRAB = 4,
};

enum class ElevatorPose{
    STORE = 0,
    UP = 1,
    DOWN = 2,
    //BORDER = 3,
};

enum class ManipulatorPose{
    DROP = 0,
    GRAB = 1,
    STORE = 2,
};


class Actuators : public Service{
//protected:
public:
    // When BAU is front of robot, 
    ActuatorGroup groupAB; // Left Manipulator
    ActuatorGroup groupBC; // Banner elevator
    ActuatorGroup groupCA; // Right Manipulator

//common methods  
public:
    Actuators();

    void sleep();

    void attach() override;
    void run()override;

    void disable()override;
    void enable()override;

    void enableTraco();
    void disableTraco();

     ActuatorGroup& getActuatorGroup(RobotCompass rc);

    
//primary methods
public:
    void drop   (RobotCompass rc, int speed = 100);
    void grab   (RobotCompass rc, int speed = 100);
    void store   (RobotCompass rc, int speed = 100);
    
    void moveElevatorAngle(RobotCompass rc, int angle, int speed = 100);
    void moveElevator(RobotCompass rc, ElevatorPose poseIndex, int speed = 100);
    //void moveElevator(RobotCompass rc, PlankManipulatorPose poseIndex, int speed = 100);
    void moveElevatorOffset(RobotCompass rc, ElevatorPose poseIndex, int offset, int speed = 100);
    void registerPoses(); //Register all servos poses

private : 
    bool moveManipulator(SmartServo& servo, ManipulatorPose pose, int speed = 100);
    bool moveElevator(SmartServo& servo, ElevatorPose pose, int speed = 100);

    void createHugger(RobotCompass, HuggerProperties);
    void createManipulator(RobotCompass, ManipulatorProperties);

    SINGLETON(Actuators)
};

SINGLETON_EXTERN(Actuators, actuators)