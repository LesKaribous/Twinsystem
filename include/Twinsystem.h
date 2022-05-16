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

using namespace Debugger;

namespace System{
    inline void reboot(){
        log("Rebooting robot in 3s...");
        delay(1000);
        log("Rebooting robot in 2s...");
        delay(1000);
        log("Rebooting robot in 1s...");
        delay(1000);
        _reboot_Teensyduino_();
    }


    inline void init(){
        //--- INIT ---
        Settings::init();
        Actuators::init();
        Controller::init();
        Motion::init();
        Debugger::init();
        IHM::init();
        Intercom::init();
        //--- WAIT LAUNCH---
        Strategy::waitLaunch();
    }

} // namespace System

