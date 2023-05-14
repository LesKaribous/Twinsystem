#include "system.h"
#include "Settings.h"

System::System(){
    m_currentState = BOOT;
}

System::~System(){
    //Smart pointers destroyed
}

void System::registerModule(Module* m){
    m_modules.push_back(m);
}

void System::enable(SystemModule module) {
    for(auto& mod : m_modules){
        if(mod->getModule() == module){
            mod->enable();
        }
    }
}

void System::disable(SystemModule module) {
    for(auto& mod : m_modules){
        if(mod->getModule() == module){
            mod->disable();
        }
    }
}

void System::update() {
    // Update each enabled subsystem
    for(auto& module : m_modules) {
        if(module->isEnabled()) {
            module->update();
        }
    }

    switch (m_currentState){
    case BOOT:
        //Boot finished
        handleBootState();
        break;
    case IDLE:
        //Wait for launch
        handleIdleState();
        break;
    case ARMED:
        //Wait for launch
        handleArmedState();
        break;
    case RUNNING:
        //Match
        handleRunningState();
        break;
    case STOPPED:
        //Do nothing and display score
        handleStoppedState();
        break;
    default:
        break;
    }
}

void System::handleBootState(){
	enable(LIDAR);
    enable(INPUTS);
    enable(SCREEN);
	enable(MOTION);
    enable(NEOPIXEL);
	enable(ACTUATOR);
    m_currentState = IDLE;
}


void System::handleIdleState(){

}

void System::handleArmedState(){

}

void System::handleRunningState(){

}

void System::handleStoppedState(){

}

void System::wait(unsigned long temps){
	unsigned long initTemps = millis();
	while ((millis() - initTemps) <= temps){
		update();
	}
}

void System::waitUntil(Job& obj){
	while (obj.isPending()){
		obj.update();
		
		update();
	}
}




/*
void Robot::WaitLaunch(){
	while (_state != RobotState::STARTING){
		Update();
		switch (_state){
		case RobotState::IDLE :
			if(StarterPlaced()){ //Arm
				_state = RobotState::ARMED;
				FreezeSettings();

				if(intercom.IsConnected()){
					intercom.SendRequest("displayLidar");
				}
			}
			if(ButtonReleased()){ //Recalage
				if	   (IsBlue()  && IsPrimary() ) 					RecalagePrimaryBlue();
				else if(IsBlue()  && IsSecondary() && IsCherry() ) 	RecalageSecondaryBlue();
				else if(IsBlue()  && IsSecondary() && IsCake() ) 	RecalageSecondaryCakeBlue();
				else if(IsGreen() && IsPrimary() ) 					RecalagePrimaryGreen();
				else if(IsGreen() && IsSecondary() && IsCherry() ) 	RecalageSecondaryGreen();
				else if(IsGreen() && IsSecondary() && IsCake() ) 	RecalageSecondaryCakeGreen();
				//TestOrientation();
				//TestSteppers();
			}
			break;

		case RobotState::ARMED :
			if(StarterPulled()){ //Start match
				_state = RobotState::STARTING;
				DisableDisguisement();
			}else if(StarterCancelled()){ //Unarm
				_state = RobotState::IDLE;
				UnfreezeSettings();
				EnableDisguisement();
				intercom.SendRequest("displayIntercom");
				while(ButtonPressed()) Update(); //Wait for resetButton to be released
			}
			break;
	
		default:
			break;
		}

		//delay(10);
	};
}*/



