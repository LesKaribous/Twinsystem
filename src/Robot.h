#pragma once
#include "core/System.h"
#include "Match.h"
using namespace TwinSystem;

class Robot : public System{
public :
    Robot();
    ~Robot(){};

    //Sytem routines
    void Update() override;
    void PollEvents() override;

    //Lidar
    void CheckLidar();
    bool ObstacleDetected();
    void EnableAvoidance();
    void DisableAvoidance();

    //Movements
    void Turn(float);
    void Go(Vec2);
    void Go(float x, float y);
    void GoPolar(float heading, float length);
    void Align(RobotCompass, float orientation);

	void WaitLaunch();
	void StartMatch();

    //UI
    bool ButtonPressed();
    bool ButtonReleased();
    bool HasStarter();
    bool StarterPulled();
    bool StarterPlaced();
    bool StarterCancelled();

    bool IsPrimary();
    bool IsSecondary();
    bool IsBlue();
    bool IsGreen();

    bool GetRobotType();
    bool GetStrategyState();

    //Strategy
    void RecalagePrimaryBlue();
    void RecalagePrimaryGreen();
    void RecalageSecondaryBlue();
    void RecalageSecondaryGreen();

    void MatchPrimaryBlue();
	void MatchPrimaryGreen();
    void MatchSecondaryBlue();
    void MatchSecondaryGreen();
	
    //Macro
    void ProbeBorder(TableCompass, RobotCompass);

    //Tests
    void TestSteppers();
    void TestDetection();
    void TestOrientation();

    //Util
    void FreezeSettings();
    void UnfreezeSettings();

    bool IsProbed();
    bool IsProbing();
    bool IsXProbed();
    bool IsYProbed();

protected:

    TwinSystem::Match match;

    bool _avoidance = true;
    //bool _obstacle = false;
    unsigned long _lastLidarCheck = 0;

    bool _probedX = false, _probedY = false;
    bool _probing = false;

    //Tracked values
    ValueTracker<Vec3> robotPositionTracker;
    ValueTracker<int>  scoreTracker;
    ValueTracker<int>  timeTracker;
    ValueTracker<bool> robotProbedTracker;
    ValueTracker<bool> robotProbingTracker;
    ValueTracker<bool> robotArmedTracker;
    ValueTracker<bool> robotStartedTracker;
    ValueTracker<bool> intercomConnectionTracker;
};
