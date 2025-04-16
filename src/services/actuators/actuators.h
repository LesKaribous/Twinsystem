#pragma once
#include "services/service.h"
#include "services/actuators/actuatorGroup.h"
#include "utils/geometry.h"
#include "utils/job.h"
#include "services/actuators/groups.h"


enum class ServoIDs : int{
    MAGNET_RIGHT = 0,
    ELEVATOR = 1,
    MAGNET_LEFT = 2,
    PLANKS = 3
};

enum class ElevatorPose{
    DROP = 0,
    UP = 1,
    DOWN = 2
    //BORDER = 3,
};

enum class ManipulatorPose{
    DROP = 0,
    GRAB = 1
};

enum class PlankManipulatorPose{
    DROP = 0,
    GRAB = 1,
    STORE = 2,
};


class Actuators : public Service, public Job{
//protected:
public:
    // When BAU is front of robot, 
    ActuatorGroup groupAB; // Left Manipulator
    ActuatorGroup groupBC; // Banner elevator
    ActuatorGroup groupCA; // Right Manipulator

//common methods  
public:
    Actuators();

    void run(){};
    void sleep();

    void onAttach()override;
    void onUpdate()override;
    void disable()override;
    void enable()override;

    void enableTraco();
    void disableTraco();
private:
    ActuatorGroup& getActuatorGroup(RobotCompass rc);
    
//primary methods
public:
    void drop   (RobotCompass rc, int speed = 100);
    void grab   (RobotCompass rc, int speed = 100);

    void dropPlank   (RobotCompass rc, int speed = 100);
    void grabPlank   (RobotCompass rc, int speed = 100);
    void storePlank   (RobotCompass rc, int speed = 100);
    
    void moveElevatorAngle(RobotCompass rc, int angle, int speed = 100);
    void moveElevator(RobotCompass rc, ElevatorPose poseIndex, int speed = 100);
    //void moveElevator(RobotCompass rc, PlankManipulatorPose poseIndex, int speed = 100);
    void moveElevatorOffset(RobotCompass rc, ElevatorPose poseIndex, int offset, int speed = 100);
    void registerPoses(); //Register all servos poses

private : 
    bool moveMagnetDual(SmartServo& a,SmartServo& b, ManipulatorPose pose, int speed = 100);
    bool moveMagnet(SmartServo& servo, ManipulatorPose pose, int speed = 100);
    bool moveElevator(SmartServo& servo, ElevatorPose pose, int speed = 100);
    void createManipulator(RobotCompass, ManipulatorProperties);
    void createBannerManipulator(RobotCompass, BannerManipulatorProperties);

    SERVICE(Actuators)
};

EXTERN_DECLARATION(Actuators, actuators)