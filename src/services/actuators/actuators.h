#pragma once
#include "system/core/service.h"
#include "system/math/geometry.h"
#include "services/actuators/bistableServo.h"

struct GripperGroup{
    BistableServo rightGripper;
    BistableServo leftGripper;
    BistableServo cherryLocker;

    GripperGroup(BistableServoProps rightGripProps, BistableServoProps leftGripProps, BistableServoProps cherryLockProps) : rightGripper(rightGripProps), leftGripper(leftGripProps), cherryLocker(cherryLockProps){};
};


class Actuators : public Service{
private:
    GripperGroup gripperAB; //Right group when facing screen
    GripperGroup gripperBC; //Opposed to the screen
    GripperGroup gripperCA; //Left group when facing screen

    int _pinTurbine;
    public:
    BistableServo trap;
public:

    Actuators();
    ~Actuators();

    void update() override;
    void disable()override;
    void enable()override;

    void lock   (RobotCompass rc); //Lock Cherry locker
    void unlock (RobotCompass rc); //Unlock Cherry locker

    void close  (RobotCompass rc);
    void open   (RobotCompass rc);
    void grab   (RobotCompass rc);
    void ungrab (RobotCompass rc);
    void applause(RobotCompass rc);

    void setTurbine(int speed); //in %
    void stopTurbine();
    void suckBall();
    void dropBall();

    void openTrap();
    void closeTrap();

    void sleep();
};
