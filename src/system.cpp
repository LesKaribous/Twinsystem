#include "system.h"
#include "Settings.h"

unsigned long lastSeen = 0;
void OnDummyRequestResponse(String answer){
    if(answer.startsWith("obstacle")) obstacle = true;
	else obstacle = false;
}

System::System(){
    m_score = 0;
    m_currentState = BOOT;

    //Standalone modules
    lidar = std::make_unique<Lidar>();
    screen = std::make_unique<Screen>();
    inputs = std::make_unique<Inputs>();
    motion = std::make_unique<Motion>();
    planner = std::make_unique<Planner>();
    neopixel = std::make_unique<NeoPixel>();
    actuators = std::make_unique<Actuators>();

    //Extension Modules
    e_test = std::make_unique<ExtTest>(*this);
    e_motion = std::make_unique<ExtMotion>(*this);
    e_strategy = std::make_unique<ExtStrategy>(*this);

    //Modules array for grouped action (update ...)
    m_modules[0] = lidar.get();
    m_modules[1] = screen.get();
    m_modules[2] = inputs.get();
    m_modules[3] = motion.get();
    m_modules[4] = planner.get();
    m_modules[5] = neopixel.get();
    m_modules[6] = actuators.get();
}

System::~System(){
    //Smart pointers destroyed
}

void System::enable(SystemModule component) {
    // Enable a component
    switch (component) {
        case LIDAR:
            lidar->enable();
            break;
        case SCREEN:
            screen->enable();
            break;
        case INPUTS:
            inputs->enable();
            break;
        case MOTION:
            motion->enable();
            break;
        case PLANNER:
            planner->enable();
            break;
        case NEOPIXEL:
            neopixel->enable();
            break;
        case ACTUATOR:
            actuators->enable();
            break;
        // ... handle other components
    }
}

void System::disable(SystemModule component) {
    // Enable a component
    switch (component) {
        case LIDAR:
            lidar->disable();
            break;
        case SCREEN:
            screen->disable();
            break;
        case INPUTS:
            inputs->disable();
            break;
        case MOTION:
            motion->disable();
            break;
        case PLANNER:
            planner->disable();
            break;
        case NEOPIXEL:
            neopixel->disable();
            break;
        case ACTUATOR:
            actuators->disable();
            break;
        // ... handle other components
    }
}


void System::execute(const String& command) {

}

void System::execute(std::shared_ptr<Task> task){

}

void System::addToScore(int points, int multiplicateur){
    m_score += (points * multiplicateur);
}

int  System::getScore(){
    return m_score;
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
        handleIdleState();
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
    if(inputs->starterPlaced()){ //Arm
        m_currentState = ARMED;
        inputs->freezeSettings();

        if(lidar->isConnected()){
            lidar->activateDisplay();
        }
    }
    if(inputs->buttonReleased()){ //Recalage
        if	   (inputs->isBlue()  && inputs->isPrimary()) 					        e_strategy->recalagePrimaryBlue();
        else if(inputs->isBlue()  && inputs->isSecondary() && inputs->isCherry()) 	e_strategy->recalageSecondaryBlue();
        else if(inputs->isBlue()  && inputs->isSecondary() && inputs->isCake()) 	e_strategy->recalageSecondaryCakeBlue();
        
        else if(inputs->isGreen() && inputs->isPrimary()) 					        e_strategy->recalagePrimaryGreen();
        else if(inputs->isGreen() && inputs->isSecondary() && inputs->isCherry()) 	e_strategy->recalageSecondaryGreen();
        else if(inputs->isGreen() && inputs->isSecondary() && inputs->isCake()) 	e_strategy->recalageSecondaryCakeGreen();
    }
}

void System::handleArmedState(){
    if(inputs->starterPulled()){ //Start match
        m_currentState = RUNNING;
        disable(NEOPIXEL);
    }else if(inputs->starterCancelled()){ //Unarm
        m_currentState = IDLE;
        inputs->unfreezeSettings();
        enable(NEOPIXEL);
        lidar->activateDisplay();
        inputs->waitButtonRelease(); //Wait for resetButton to be released
    }
}

void System::handleRunningState(){

    //Console::error("Robot") << "Checking lidar" << Console::endl;
    lidar->checkLidar(-motion->getTargetDirection() * RAD_TO_DEG);
    
    if(chrono.isNearlyFinished()){
        motion->cancel();
        e_strategy->handleNearlyFinishedMatch();//go home
    }

    if(chrono.isFinished()){
        motion->cancel();
        e_strategy->handleFinishedMatch(); //Stop robot, motor disengage
    }
	

    //Execute Objectives


    /*
    if (m_planner->isEnabled()) {
        // Get a task to execute if planner has one
        if (auto task = m_planner->getTaskToExecute()) {
            executeTask(task);
        }

        // Handle objective completion
        if (m_planner->isObjectiveComplete()) {
            m_score += m_planner->collectReward(); //Reward can be collected only once
        }
    }
    */
}

void System::handleStoppedState(){

}

void System::wait(int temps){
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
void System::Update() {
	motion.UpdatePosition();
	PollEvents();
	System::Update();
	match.GetTimeLeft();

	if(_state == RobotState::STARTED || _state == RobotState::FINISHING){
		//Console::error("Robot") << "Checking lidar" << Console::endl;
		CheckLidar();
		
		if(match.IsNearlyFinished()){
			motion.Cancel();
			NearlyFinishedMatch();//go home
		}

		if(match.IsFinished()){
			motion.Cancel();
			FinishedMatch(); //Stop robot, motor disengage
		}
	}


}*/


void System::updateUiFields(){
    //Tracked values
	//TODO Create events to handle this at the UI Level

	screen->x.SetValue(motion->getAbsPosition().a);
	screen->y.SetValue(motion->getAbsPosition().b);
	screen->z.SetValue(motion->getAbsPosition().c);
	screen->probed.SetValue(e_motion->isProbed());
	screen->probing.SetValue(e_motion->isProbing());
	screen->armed.SetValue(m_currentState != ARMED && m_currentState != IDLE);
	screen->started.SetValue(m_currentState == IDLE);
	screen->intercom.SetValue(lidar->isConnected());
	screen->time.SetValue(getScore());
	screen->score.SetValue(chrono.getTimeLeftSeconds());
	
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

void Robot::StartMatch(){
	match.Start();
	ui.SetPage(Page::MATCH);
	//actuators.Engage();
	motion.steppers.Engage();
	_state = RobotState::STARTED;

	
	TestMotion(); motion.steppers.Disengage(); return;
	//TestSteppers(); motion.steppers.Disengage(); return;
	//TestDetection(); motion.steppers.Disengage(); return;

	if	   (IsBlue()  && IsPrimary()	) MatchPrimaryBlue	();
	else if(IsBlue()  && IsSecondary() && IsCherry()) MatchSecondaryBlue();
	else if(IsBlue()  && IsSecondary() && IsCake()	) MatchSecondaryCakeBlue();
	else if(IsGreen() && IsPrimary()	) MatchPrimaryGreen	();
	else if(IsGreen() && IsSecondary() && IsCherry()) MatchSecondaryGreen();
	else if(IsGreen() && IsSecondary() && IsCake()	) MatchSecondaryCakeGreen();
	motion.steppers.Disengage();
}

void Robot::FinishedMatch(){
	if(_state == RobotState::STARTED || _state == RobotState::FINISHING){
		_state = RobotState::FINISHED;
		motion.Cancel();
		if	   (IsBlue()  && IsPrimary()	) FinishPrimaryBlue();
		else if(IsBlue()  && IsSecondary()	) FinishSecondaryBlue();
		else if(IsGreen() && IsPrimary()	) FinishPrimaryGreen();
		else if(IsGreen() && IsSecondary()	) FinishSecondaryGreen();
		motion.steppers.Disengage();
		actuators.Disengage();
		EnableDisguisement();
		ui.Update();
		delay(1000);
		//Suicide
		while (true){delay(100);}
	}
}

void Robot::NearlyFinishedMatch(){
	if(_state == RobotState::STARTED){
		_state = RobotState::FINISHING;
		motion.Cancel();
		if	   (IsBlue()  && IsPrimary()	) NearlyFinishPrimaryBlue();
		else if(IsBlue()  && IsSecondary()	) NearlyFinishSecondaryBlue();
		else if(IsGreen() && IsPrimary()	) NearlyFinishPrimaryGreen();
		else if(IsGreen() && IsSecondary()	) NearlyFinishSecondaryGreen();
		motion.steppers.Disengage();
	}
}

