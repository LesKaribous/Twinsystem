#include "app.h"
#include "system/core/os.h"

void App::handleIdleState(){}

void App::handleRunningState(){}

void App::TakePlant(RobotCompass rc){
    actuators.moveElevator(rc, ElevatorPose::GRAB);
    wait(1000, false);
    SlowClosing(rc);
    for(int i=0;i<3;i++){
        SlowGrabing(rc);
        SlowClosing(rc);
    }
    SlowGrabing(rc);
    actuators.moveElevator(rc, ElevatorPose::UP);
    wait(1000, false);
}

void App::PlacePlant(RobotCompass rc){
    actuators.moveElevator(rc, ElevatorPose::GRAB);
    wait(1000, false);
    SlowOpening(rc);
    SlowElevatorUp(rc);
    wait(1000, false);
}

void App::SlowGrabing(RobotCompass rc){
    while(!actuators.runGrabbing(rc)) wait(Settings::Actuators::speed, false);
}

void App::SlowGrabing(RobotCompass rc, Side gs){
    while(!actuators.runGrabbing(rc,gs)) wait(Settings::Actuators::speed, false);
}

void App::SlowOpening(RobotCompass rc){
    while(!actuators.runOpening(rc)) wait(Settings::Actuators::speed, false);
}

void App::SlowClosing(RobotCompass rc){
    while(!actuators.runClosing(rc)) wait(Settings::Actuators::speed, false);
}

void App::SlowElevatorUp(RobotCompass rc){
    while(!actuators.runElevatorUp(rc)) wait(Settings::Actuators::speed, false);
}

void App::SlowElevatorDown(RobotCompass rc){
    while(!actuators.runElevatorDown(rc)) wait(Settings::Actuators::speed, false);
}

void App::SlowElevatorGrab(RobotCompass rc){
    while(!actuators.runElevatorGrab(rc)) wait(Settings::Actuators::speed, false);
}

