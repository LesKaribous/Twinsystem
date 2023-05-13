#pragma once
#include "system.h"
#include "Match.h"
using namespace TwinSystem;

class Robot : public System{
public :
    Robot();
    ~Robot(){};


    //Sytem routines
    void Initialize() override;
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

    void GoAlign(Vec2, RobotCompass, float orientation); //Go align

    void GoPolar(float heading, float length);
    void Align(RobotCompass, float orientation);

	void WaitLaunch();
	void StartMatch();

    void FinishedMatch();
    void NearlyFinishedMatch();

    //Deguisement

    float GetMaxLidarDist(Vec2 pos, float angle);

    void EnableDisguisement();
    void DisableDisguisement();

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
    bool IsCake();
    bool IsCherry();

    bool GetRobotType();
    bool GetStrategyState();

    //Strategy
    void RecalagePrimaryBlue();
    void RecalagePrimaryGreen();
    void RecalageSecondaryBlue();
    void RecalageSecondaryGreen();
    void RecalageSecondaryCakeBlue();
    void RecalageSecondaryCakeGreen();

    void MatchPrimaryBlue();
	void MatchPrimaryGreen();
    void MatchSecondaryBlue();
    void MatchSecondaryGreen();
    void MatchSecondaryCakeBlue();
    void MatchSecondaryCakeGreen();

    void NearlyFinishPrimaryBlue();
    void NearlyFinishPrimaryGreen();
    void NearlyFinishSecondaryBlue();
    void NearlyFinishSecondaryGreen();

    void FinishPrimaryBlue();
    void FinishPrimaryGreen();
    void FinishSecondaryBlue();
    void FinishSecondaryGreen();
	
    //Macro
    void ProbeBorder(TableCompass, RobotCompass);
    void ProbeObstacle(Vec2 obstaclePosition,TableCompass, RobotCompass);

    //Tests
    void TestSteppers();
    void TestMotion();
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
