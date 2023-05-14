#pragma once
#include "modules/motion/motion.h"
#include "modules/actuators/actuators.h"

class System; // forward declaration

class ExtStrategy{
private:
    System& system;
    Motion& motion;
    Actuators& actuators;

public:
    ExtStrategy(System& sys);
    ~ExtStrategy();

    //Strategy
    void recalagePrimaryBlue();
    void recalagePrimaryGreen();
    void recalageSecondaryBlue();
    void recalageSecondaryGreen();
    void recalageSecondaryCakeBlue();
    void recalageSecondaryCakeGreen();

    void matchPrimaryBlue();
	void matchPrimaryGreen();
    void matchSecondaryBlue();
    void matchSecondaryGreen();
    void matchSecondaryCakeBlue();
    void matchSecondaryCakeGreen();

    void nearlyFinishPrimaryBlue();
    void nearlyFinishPrimaryGreen();
    void nearlyFinishSecondaryBlue();
    void nearlyFinishSecondaryGreen();

    void finishPrimaryBlue();
    void finishPrimaryGreen();
    void finishSecondaryBlue();
    void finishSecondaryGreen();

    void handleFinishedMatch();
    void handleNearlyFinishedMatch();

    //Macro
    void probeBorder(TableCompass, RobotCompass);
    void probeObstacle(Vec2 obstaclePosition,TableCompass, RobotCompass);

};

