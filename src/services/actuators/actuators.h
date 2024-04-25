#pragma once
#include "services/service.h"
#include "services/actuators/actuatorGroup.h"
#include "utils/geometry.h"
#include "utils/job.h"
#include "groups.h"

#define FORK_SERVO       0
#define GRIPPER_RIGHT    0
#define ELEVATOR         1
#define GRIPPER_LEFT     2




enum ElevatorPose{
    UP = 0,
    DOWN = 1, 
    GRAB = 2,
    BORDER = 3,
    PLANTER = 4
};

enum GripperPose{
    OPEN = 0,
    GRAB = 1,
    CLOSE = 2,
};

class Actuators : public Service, public Job{
//protected:
public:
    ActuatorGroup groupAB; // Facing BAU
    ActuatorGroup groupBC; // Facing Init button
    ActuatorGroup groupCA; // Facing Tirette
    
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
    
    void moveElevatorAngle(RobotCompass rc, int angle);
    void moveElevator(RobotCompass rc, ElevatorPose poseIndex);

    bool readSensor(RobotCompass rc, Side gs);
    void testSensors();

    int howManyPlant(RobotCompass rc);
    
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
    bool moveGripper(SmartServo& gripper, int target);
    ActuatorGroup& getActuatorGroup(RobotCompass rc);
    
    void createFingerGroup(RobotCompass, FingerGroupProperty);
    void createForkGroup(RobotCompass, ForkGroupProperty);

    SERVICE(Actuators)
};

EXTERN_DECLARATION(Actuators, actuators)