#pragma once

#include "system.h"
#include "settings.h"

#include "core/chrono.h"

#include "match/poi.h"
#include "match/score.h"
#include "math/geometry.h"

#include "modules/lidar/lidar.h"
#include "modules/inputs/inputs.h"
#include "modules/motion/motion.h"
#include "modules/screen/screen.h"
#include "modules/planner/planner.h"
#include "modules/neopixel/neopixel.h"
#include "modules/actuators/actuators.h"

enum class RobotState{
    IDLE,
    ARMED,
    STARTING,
    STARTED,
    FINISHING,
    FINISHED
};

class SystemApplication{
    System system;
    Chronometer chrono;

public:
    SystemApplication();
    ~SystemApplication();

	void initialize();
	void waitLaunch();
	void startMatch();

    void connectModules();

    //Movements
    void go(Vec2);
    void go(float x, float y);

    void turn(float);
    void align(RobotCompass, float orientation);

    void goPolar(float heading, float length);
    void goAlign(Vec2, RobotCompass, float orientation);

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

    //Probe state
    bool isProbed();
    bool isProbing();
    bool isXProbed();
    bool isYProbed();

    //Tests
    void testMotion();
    void testSteppers();
    void testDetection();
    void testOrientation();

private:
    int score();
	void addScore(int points, int multiplicateur = 1.0);

    //Standalone modules
    std::unique_ptr<Lidar> _lidarPtr = nullptr;
    std::unique_ptr<Screen> _screenPtr = nullptr;
    std::unique_ptr<Inputs> _inputsPtr = nullptr;
    std::unique_ptr<Motion> _motionPtr = nullptr;
    std::unique_ptr<Planner> _plannerPtr = nullptr;
    std::unique_ptr<NeoPixel> _neopixelPtr = nullptr;
    std::unique_ptr<Actuators> _actuatorsPtr = nullptr;

    RobotState _state;

    int _score;
    bool _probedX = false, _probedY = false;
    bool _probing = false;
};