#pragma once
#include "core/System.h"
using namespace TwinSystem;

class Robot : public System{
public :
    Robot();
    ~Robot(){};

    //Sytem routines
    void Update() override;
    void PollEvents() override;

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
    void RecalagePrimaryGreen();
    void RecalageSecondaryGreen();
    void RecalagePrimaryBlue();
    void RecalageSecondaryBlue();

	void MatchPrimaryGreen();
	void MatchPrimaryBlue();
    void MatchSecondaryGreen();
	void MatchSecondaryBlue();

    //Macro
    void ProbeBorder(TableCompass, RobotCompass);

    //Tests
    void TestSteppers();
    void TestOrientation();

    //Util
    void FreezeSettings();
    void UnfreezeSettings();

    bool IsProbed();
    bool IsProbing();
    bool IsXProbed();
    bool IsYProbed();

protected:
    bool _team;
    bool _avoidance;
    bool _strategy;

    bool _probedX = false, _probedY = false;
    bool _probing = false;

    //Tracked values
    ValueTracker<Vec3> robotPositionTracker;
    ValueTracker<bool> robotProbedTracker;
    ValueTracker<bool> robotProbingTracker;
    ValueTracker<bool> robotArmedTracker;
    ValueTracker<bool> robotStartedTracker;
    ValueTracker<bool> intercomConnectionTracker;
};