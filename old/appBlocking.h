#pragma once

#include "settings.h"

#include "system/core/system.h"
#include "system/core/chrono.h"

#include "system/match/poi.h"
#include "system/match/score.h"
#include "system/math/geometry.h"

#include "modules/lidar/lidar.h"
#include "modules/inputs/inputs.h"
#include "modules/motion/motion.h"
#include "modules/screen/screen.h"
//#include "modules/planner/planner.h"
#include "modules/neopixel/neopixel.h"
#include "modules/terminal/terminal.h"
#include "modules/actuators/actuators.h"
#include "modules/localisation/localisation.h"

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

	void update();
	void waitLaunch();
	void startMatch();
    void endMatch();

    void connectModules();

    void processCommand(Command c);
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

    void matchSecondaryCakeBlueOld();
    void matchSecondaryCakeGreenOld();
    
    void nearlyFinishPrimaryBlue();
    void nearlyFinishPrimaryGreen();
    void nearlyFinishSecondaryBlue();
    void nearlyFinishSecondaryGreen();

    void handleRecalage();
    void handleFinishedMatch();
    void handleNearlyFinishedMatch();

    //Macro
    void probeBorder(TableCompass, RobotCompass);
    void probeObstacle(Vec2 obstaclePosition,TableCompass, RobotCompass);
    
    //Tests
    void testMotion();
    void testSteppers();
    void testDetection();
    void testOrientation();

    //State
    void setState(RobotState);
    void printState(RobotState);

    //wait blocking function
    void wait(unsigned long temps);
    void waitUntil(Job& obj);

    void optimizeScore(float k);

private:
    int score();
	void addScore(int points, int multiplicateur = 1.0);

    template<typename T>
    constexpr std::shared_ptr<T> loadModule();
    //Standalone modules
    std::shared_ptr<Lidar> _lidarPtr = nullptr;
    std::shared_ptr<Screen> _screenPtr = nullptr;
    std::shared_ptr<Inputs> _inputsPtr = nullptr;
    std::shared_ptr<Motion> _motionPtr = nullptr;
    //std::shared_ptr<Planner> _plannerPtr = nullptr;
    std::shared_ptr<NeoPixel> _neopixelPtr = nullptr;
    std::shared_ptr<Terminal> _terminalPtr = nullptr;
    std::shared_ptr<Actuators> _actuatorsPtr = nullptr;

    //std::shared_ptr<Localisation> _localisationPtr = nullptr;

    RobotState _state;

    int _score;
    bool _probed;
    bool _probing;


    unsigned long _lastDrift = 0;
};