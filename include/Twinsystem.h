#pragma once

#include "Actuators.h"
#include "IHM.h"
#include "Motion.h"
#include "Match.h"
#include "Controller.h"
#include "Strategy.h"
#include "Debugger.h"
#include "Settings.h"
#include "Pin.h"
#include "Test.h"
#include "Intercom.h"

namespace System{
    void reboot();
    void init();
    void update();
	void wait(int temps);
} // namespace System

