#pragma once
#include "core/System.h"
using namespace TwinSystem;

class Robot : public System{
public :
    Robot();

	bool WaitLaunch();
    void Recalage();
	void Match();

    void FreezeSettings();
    void UnfreezeSettings();

    void TestSteppers();

protected:
    bool _team;
    bool _avoidance;
    bool _strategy;
};
