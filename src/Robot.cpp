#include "Robot.h"
#include "Settings.h"

#include "debug/Console.h"

#include <Adafruit_NeoPixel.h>
#define NEOPIXEL_PIN Pin::Led::neopixel   // Change this to match the pin you are using
#define NUM_LEDS 8       // Change this to match the number of LEDs in your strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool obstacle = false;
void OnDummyRequestResponse(String answer){
    if(answer.startsWith("obstacle")) obstacle = true;
	else obstacle = false;
}


Robot::Robot(){
	
	_state = RobotState::IDLE; //Do not remove
	/*
	_team = Settings::Match::GREEN;
	_avoidance = Settings::Match::AVOIDANCE;
	*/
	_probedX = false, 
	_probedY = false;
	_probing = false;
}

void Robot::Update() {
	motion.UpdatePosition();
	PollEvents();
	System::Update();
	match.GetTimeLeft();
	if(_state == RobotState::IDLE)Console::println("IDLE");
	if(_state == RobotState::ARMED)Console::println("ARMED");
	if(_state == RobotState::STARTING)Console::println("STARTING");
	if(_state == RobotState::STARTED)Console::println("STARTED");
	if(_state == RobotState::STOPPED)Console::println("STOPPED");

	if(_state == RobotState::STARTED){
		Console::error("Robot") << "Checking lidar" << Console::endl;
		CheckLidar();
		
		if(match.IsNearlyFinished()) NearlyFinishedMatch();//go home
		if(match.IsFinished()) FinishedMatch(); //Stop robot, motor disengage

	}
}

void Robot::Go(Vec2 v){
	motion.GoAsync(v);
	WaitUntil(motion);
}

void Robot::Go(float x, float y){
	motion.GoAsync(x, y);
	WaitUntil(motion);
}

void Robot::Turn(float a){
	motion.TurnAsync(a);
	WaitUntil(motion);
}

void Robot::Align(RobotCompass rc, float orientation){
	bool wasRelative = motion.IsRelative();
	motion.SetAbsolute();
	motion.AlignAsync(rc, orientation);
	WaitUntil(motion);
	if(wasRelative) motion.SetRelative();
}

void Robot::GoPolar(float heading, float length){
	PolarVec target = PolarVec(heading*DEG_TO_RAD, length);
	bool wasAbsolute = motion.IsAbsolute();
	motion.SetRelative();
	motion.GoAsync(target.toVec2());
	WaitUntil(motion);

	if(wasAbsolute) motion.SetAbsolute();
}



void Robot::CheckLidar(){
	if(_avoidance && motion.IsBusy()){
		if(millis() - _lastLidarCheck > 100){
			_lastLidarCheck = millis();
			float heading = -motion.GetAbsoluteTargetDirection() * RAD_TO_DEG;
			heading = fmod(heading, 360.0);
			if(heading < 0) heading += 360.0;
			intercom.SendRequest("checkLidar(" + String(heading) + ")", OnDummyRequestResponse);
		}

		if(ObstacleDetected() && motion.IsRunning() && !motion.IsRotating()){
			motion.Pause();
		}else if(motion.IsPaused() && !ObstacleDetected()){
			motion.Resume();
		}
	}
}

bool Robot::ObstacleDetected(){
	return obstacle;
}

void Robot::EnableAvoidance(){
	_avoidance = true;
}

void Robot::DisableAvoidance(){
	_avoidance = false;
	if(motion.IsPaused() && ObstacleDetected()){
		obstacle = false;
		motion.Resume();
	}
	
}




void Robot::PollEvents(){
    //Tracked values
    robotPositionTracker.SetValue(motion.GetAbsPosition());
    robotProbedTracker.SetValue(IsProbed());
    robotProbingTracker.SetValue(IsProbing());
    robotArmedTracker.SetValue(_state == RobotState::ARMED);
    robotStartedTracker.SetValue(_state != RobotState::ARMED && _state != RobotState::IDLE);
    intercomConnectionTracker.SetValue(intercom.IsConnected());


	//TODO 
	scoreTracker.SetValue(match.GetScore());
    timeTracker.SetValue(match.GetTimeLeftSeconds());

	//Console::info("Time left :") << match.GetTimeLeftSeconds() << "s" << Console::endl;

	//TODO Create events to handle this at the UI Level
	if(robotPositionTracker.HasChanged()){
		ui.fields.x.SetValue(robotPositionTracker.GetValue().a);
		ui.fields.y.SetValue(robotPositionTracker.GetValue().b);
		ui.fields.z.SetValue(robotPositionTracker.GetValue().c);
	}

	if(robotProbedTracker.HasChanged()){
		ui.fields.probed.SetValue(robotProbedTracker.GetValue());
	}

	if(robotProbingTracker.HasChanged()){
		ui.fields.probing.SetValue(robotProbingTracker.GetValue());
	}

	if(robotArmedTracker.HasChanged()){
		ui.fields.armed.SetValue(robotArmedTracker.GetValue());
	}

	if(robotStartedTracker.HasChanged()) {
		ui.fields.started.SetValue(robotStartedTracker.GetValue());
	}

	if(intercomConnectionTracker.HasChanged()){
		ui.fields.intercom.SetValue(intercomConnectionTracker.GetValue());
	}

	if(timeTracker.HasChanged()) {
		ui.fields.time.SetValue(timeTracker.GetValue());
	}

	if(scoreTracker.HasChanged()){
		ui.fields.score.SetValue(scoreTracker.GetValue());
	}

}

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
				if	   (IsBlue()  && IsPrimary()	) RecalagePrimaryBlue		();
				else if(IsBlue()  && IsSecondary()	) RecalageSecondaryBlue		();
				else if(IsGreen() && IsPrimary()	) RecalagePrimaryGreen		();
				else if(IsGreen() && IsSecondary()	) RecalageSecondaryGreen	();
				//TestOrientation();
				//TestSteppers();
			}
			break;

		case RobotState::ARMED :
			if(StarterPulled()){ //Start match
				_state = RobotState::STARTING;
			}else if(StarterCancelled()){ //Unarm
				_state = RobotState::IDLE;
				UnfreezeSettings();
				intercom.SendRequest("displayIntercom");
				while(ButtonPressed()) Update(); //Wait for resetButton to be released
			}
			break;
	
		default:
			break;
		}

		//delay(10);
	};

	
}

void Robot::StartMatch(){
	match.Start();
	DisableDisguisement();
	ui.SetPage(Page::MATCH);
	motion.steppers.Engage();
	_state = RobotState::STARTED;

	TestDetection();motion.steppers.Disengage();return;

	if	   (IsBlue()  && IsPrimary()	) MatchPrimaryBlue	();
	else if(IsBlue()  && IsSecondary()	) MatchSecondaryBlue();
	else if(IsGreen() && IsPrimary()	) MatchPrimaryGreen	();
	else if(IsGreen() && IsSecondary()	) MatchSecondaryGreen();
	motion.steppers.Disengage();
}

void Robot::FinishedMatch(){

	if	   (IsBlue()  && IsPrimary()	) FinishPrimaryBlue();
	else if(IsBlue()  && IsSecondary()	) FinishSecondaryBlue();
	else if(IsGreen() && IsPrimary()	) FinishPrimaryGreen();
	else if(IsGreen() && IsSecondary()	) FinishSecondaryGreen();
	motion.steppers.Disengage();
}

void Robot::NearlyFinishedMatch(){
	if	   (IsBlue()  && IsPrimary()	) NearlyFinishPrimaryBlue();
	else if(IsBlue()  && IsSecondary()	) NearlyFinishSecondaryBlue();
	else if(IsGreen() && IsPrimary()	) NearlyFinishPrimaryGreen();
	else if(IsGreen() && IsSecondary()	) NearlyFinishSecondaryGreen();
	motion.steppers.Disengage();
}

void Robot::EnableDisguisement(){
  strip.begin();
  for(int i=0; i<NUM_LEDS; i++) {
    strip.setPixelColor(i, 0, 0, 255);
  }
  strip.show();
}

void Robot::DisableDisguisement(){
  for(int i=0; i<NUM_LEDS; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}


bool Robot::ButtonPressed(){
	return ui.inputs.resetButton.GetState();
}

bool Robot::ButtonReleased(){
	return ui.inputs.resetButton.HasChanged() && !ui.inputs.resetButton.GetState();
}

bool Robot::HasStarter(){
	return ui.inputs.starter.GetState();
}

bool Robot::StarterPulled(){
	return !ui.inputs.starter.GetState() && !ui.inputs.resetButton.GetState();
}

bool Robot::StarterPlaced(){
	return ui.inputs.starter.GetState() && ui.inputs.starter.HasChanged();
}

bool Robot::StarterCancelled(){
	return !ui.inputs.starter.GetState() && ui.inputs.resetButton.GetState();
}

bool Robot::IsPrimary(){
	return ui.inputs.twinSwitch.GetState() == Settings::Match::PRIMARY;
}
bool Robot::IsSecondary(){
	return ui.inputs.twinSwitch.GetState() == Settings::Match::SECONDARY;
}
bool Robot::IsBlue(){
	return ui.inputs.teamSwitch.GetState() == Settings::Match::BLUE;
}
bool Robot::IsGreen(){
	return ui.inputs.teamSwitch.GetState() == Settings::Match::GREEN;
}

bool Robot::GetStrategyState(){
	return ui.inputs.strategySwitch.GetState();
}

bool Robot::GetRobotType(){
	return ui.inputs.twinSwitch.GetState();
}

bool  Robot::IsProbed(){
	return _probedX && _probedY;
}
bool  Robot::IsXProbed(){
	return _probedX;
}
bool  Robot::IsYProbed(){
	return _probedY;
}

bool  Robot::IsProbing(){
	return _probing;
}

void Robot::FreezeSettings(){
	ui.inputs.teamSwitch.Disable();
	ui.inputs.strategySwitch.Disable();
	//ui.inputs.avoidanceSwitch.Disable();
}

void Robot::UnfreezeSettings(){
	ui.inputs.teamSwitch.Enable();
	ui.inputs.strategySwitch.Enable();
	//ui.inputs.avoidanceSwitch.Enable();
}