#pragma once


#include "settings.h"

#include "core/system.h"
#include "core/chrono.h"

#include "match/poi.h"
#include "match/score.h"
#include "math/geometry.h"

#include "modules/cli/cli.h"
#include "modules/lidar/lidar.h"
#include "modules/inputs/inputs.h"
#include "modules/motion/motion.h"
#include "modules/screen/screen.h"
//#include "modules/planner/planner.h"
#include "modules/neopixel/neopixel.h"
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

    void finishPrimaryBlue();
    void finishPrimaryGreen();
    void finishSecondaryBlue();
    void finishSecondaryGreen();

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

private:
    int score();
	void addScore(int points, int multiplicateur = 1.0);

    //Standalone modules
    std::unique_ptr<Lidar> _lidarPtr = nullptr;
    std::unique_ptr<Screen> _screenPtr = nullptr;
    std::unique_ptr<Inputs> _inputsPtr = nullptr;
    std::unique_ptr<Motion> _motionPtr = nullptr;
    //std::unique_ptr<CommandLine> _cliPtr = nullptr;
    //std::unique_ptr<Planner> _plannerPtr = nullptr;
    std::unique_ptr<NeoPixel> _neopixelPtr = nullptr;
    std::unique_ptr<Actuators> _actuatorsPtr = nullptr;
    //std::unique_ptr<Localisation> _localisationPtr = nullptr;
    

    RobotState _state;

    int _score;
    bool _probed;
    bool _probing;


    unsigned long _lastDrift = 0;
};