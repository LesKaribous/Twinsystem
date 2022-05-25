#include "Twinsystem.h"


namespace System{
    void reboot(){
        Debugger::log("Rebooting robot in 3s...");
        delay(1000);
        Debugger::log( "Rebooting robot in 2s...");
        delay(1000);
        Debugger::log( "Rebooting robot in 1s...");
        delay(1000);
        _reboot_Teensyduino_();
    }


    void init(){
        //--- INIT ---
        Debugger::init();
        IHM::init();
        Settings::init();
        Actuators::init();
        Controller::init();
        Motion::init();
       
        Intercom::init();
        IHM::ready();

        Strategy::waitLaunch();
    }

    void update(){
        Debugger::checkSerial();
        Intercom::checkSerial();
        IHM::update();
        Match::update();
        Controller::update();
    }


	void wait(int temps){
		unsigned long initTemps = millis();
		while ((millis() - initTemps) <= temps){
			System::update();
		}
	}


}
