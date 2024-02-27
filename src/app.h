#pragma once
#include "system/core/os.h"

class App : public OperatingSystem{
public:
    App(){}

    void handleIdleState() override;
    void handleRunningState() override;

    void TakePlant(RobotCompass rc);
    void PlacePlant(RobotCompass rc);
    void SlowGrabing(RobotCompass rc);
    void SlowGrabing(RobotCompass rc, Side gs);
    void SlowOpening(RobotCompass rc);
    void SlowClosing(RobotCompass rc);
    void SlowElevatorUp(RobotCompass rc);
    void SlowElevatorDown(RobotCompass rc);
    void SlowElevatorGrab(RobotCompass rc);
};