#pragma once
#include "core/System.h"
using namespace TwinSystem;

class Robot : public System{
public :
    Robot();

    void Go(Vec2);
    void Go(float x, float y);
    void Turn(float a);
    void Align(RobotCompass, float orientation);

    void GoPolar(float heading, float length);

	void WaitLaunch();
    void RecalageGreen();
    void RecalageBlue();
	void MatchGreen();
	void MatchBlue();

    void FreezeSettings();
    void UnfreezeSettings();
    void TestSteppers();
    void TestOrientation();

    void ProbeBorder(TableCompass, RobotCompass);
    bool IsProbed();
    bool IsXProbed();
    bool IsYProbed();
    //Return true while probing
    bool IsProbing();

    void PollEvents() override;

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
