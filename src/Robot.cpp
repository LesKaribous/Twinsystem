#include "Robot.h"
#include "Settings.h"

#include "debug/Console.h"
/*
long _lastDummy = 0;
void OnDummyRequestResponse(String answer){
    Serial.println("dummy request responded");
}
*/

Robot::Robot(){
	_state = RobotState::IDLE;
	_team = Settings::Match::GREEN;
	_avoidance = Settings::Match::AVOIDANCE;
	_probedX = false, 
	_probedY = false;
	_probing = false;
}

void Robot::Update() {
	motion.UpdatePosition();
	PollEvents();
	System::Update();
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
	motion.AlignAsync(rc, orientation);
	WaitUntil(motion);
}

void Robot::GoPolar(float heading, float length){
	PolarVec target = PolarVec(heading*DEG_TO_RAD, length);
	bool wasAbsolute = motion.IsAbsolute();
	motion.SetRelative();
	motion.GoAsync(target.toVec2());
	WaitUntil(motion);

	if(wasAbsolute) motion.SetAbsolute();
}

void Robot::PollEvents(){
    //Tracked values
    robotPositionTracker.SetValue(motion.GetAbsPosition());
    robotProbedTracker.SetValue(IsProbed());
    robotProbingTracker.SetValue(IsProbing());
    robotArmedTracker.SetValue(_state == RobotState::ARMED);
    robotStartedTracker.SetValue(_state != RobotState::ARMED && _state != RobotState::IDLE);
    intercomConnectionTracker.SetValue(intercom.IsConnected());


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

		delay(10);
	};

	
}

void Robot::StartMatch(){
	ui.SetPage(Page::MATCH);
	motion.steppers.Engage();
	if	   (IsBlue()  && IsPrimary()	) MatchPrimaryBlue	();
	else if(IsBlue()  && IsSecondary()	) MatchSecondaryBlue();
	else if(IsGreen() && IsPrimary()	) MatchPrimaryGreen	();
	else if(IsGreen() && IsSecondary()	) MatchSecondaryGreen();
	motion.steppers.Disengage();
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


void Robot::TestSteppers(){
	/*
	motion.steppers.DisableAsync();
	motion.steppers.Move(Vec3(1000,0,0));
	motion.steppers.Move(Vec3(0,1000,0));
	motion.steppers.Move(Vec3(0,0,1000));
	motion.steppers.Sleep();
	

	motion.steppers.EnableAsync();
	motion.steppers.Move(Vec3(1000,0,0));
	WaitUntil(motion.steppers);
	motion.steppers.Move(Vec3(-1000,0,0));
	WaitUntil(motion.steppers);

	motion.steppers.Move(Vec3(0,1000,0));
	WaitUntil(motion.steppers);
	motion.steppers.Move(Vec3(0,-1000,0));
	WaitUntil(motion.steppers);

	motion.steppers.Move(Vec3(0,0,1000));
	WaitUntil(motion.steppers);
	motion.steppers.Move(Vec3(0,0,-1000));
	WaitUntil(motion.steppers);


	motion.steppers.Move(Vec3(1000,1000,1000));
	WaitUntil(motion.steppers);
	motion.steppers.Move(Vec3(-1000,-1000,-1000));
	WaitUntil(motion.steppers);

	motion.steppers.Sleep();
	*/
	
	motion.SetAbsPosition(Vec3(0,0,0));
	motion.SetAbsolute();
	Go(100,0);
	Go(0,0);

	Turn(90);
	Turn(0);

	motion.SetRelative();
	Go(100,0);
	Go(-100,0);

	Turn(90);
	Turn(-90);
}

void Robot::TestOrientation(){
	motion.SetAbsPosition({0,0,0});
	motion.SetAbsolute();
	Align(RobotCompass::A, GetCompassOrientation(TableCompass::EAST));
	Align(RobotCompass::AB, GetCompassOrientation(TableCompass::EAST));
}

void Robot::RecalagePrimaryBlue(){
	// Laisser passer le robot secondaire
	//SetAvoidance(false);
	motion.SetRelative();
	actuators.Close(RobotCompass::AB);
	actuators.Close(RobotCompass::BC);
	actuators.Close(RobotCompass::CA);
	// ------
	motion.SetAbsPosition(Vec3(-1,-1,0));
	ProbeBorder(TableCompass::SOUTH, RobotCompass::BC);
	ProbeBorder(TableCompass::WEST, RobotCompass::BC);
	motion.SetAbsolute();
	Align(RobotCompass::AB, GetCompassOrientation(TableCompass::NORTH));
	actuators.Ungrab(RobotCompass::AB);
	actuators.Ungrab(RobotCompass::BC);
	actuators.Ungrab(RobotCompass::CA);

	motion.steppers.Sleep();
}


void Robot::RecalagePrimaryGreen(){
	// Laisser passer le robot secondaire
	//SetAvoidance(false);
	motion.SetRelative();
	actuators.Close(RobotCompass::AB);
	actuators.Close(RobotCompass::BC);
	actuators.Close(RobotCompass::CA);
	// ------
	motion.SetAbsPosition(Vec3(-1,-1,90));
	ProbeBorder(TableCompass::SOUTH, RobotCompass::BC);
	ProbeBorder(TableCompass::EAST, RobotCompass::CA);
	motion.SetAbsolute();
	Turn(-60);
	actuators.Ungrab(RobotCompass::AB);
	actuators.Ungrab(RobotCompass::BC);
	actuators.Ungrab(RobotCompass::CA);

	motion.steppers.Sleep();
}

void Robot::RecalageSecondaryGreen(){

}

void Robot::RecalageSecondaryBlue(){

}

void  Robot::ProbeBorder(TableCompass tc, RobotCompass rc){
	boolean wasAbsolute = motion.IsAbsolute();
	_probing = true;

	motion.SetRelative();
	Align(rc, GetCompassOrientation(tc));

	motion.steppers.SetFeedrate(SLOW);
	GoPolar(GetCompassOrientation(rc),200);
	motion.steppers.SetFeedrate(10);
	GoPolar(GetCompassOrientation(rc),80);

	float _offset = GetOffsets(rc);

	Vec3 position = motion.GetAbsPosition();

	if(tc == TableCompass::NORTH){
		position.a = 3000.0 - _offset; //We hit Xmax
		_probedX = true;
		motion.SetAbsPosition(position);
	}
	if(tc == TableCompass::SOUTH){
		position.a = 0.0 + _offset; //We hit Xmin
		_probedX = true;
		motion.SetAbsPosition(position);
	}
	if(tc == TableCompass::EAST){
		position.b = 2000.0 - _offset; //We hit Ymax
		_probedY = true;
		motion.SetAbsPosition(position);
	}
	if(tc == TableCompass::WEST){
		position.b = 0.0 + _offset; //We hit Ymin
		_probedY = true;
		motion.SetAbsPosition(position);
	}

	motion.SetAbsPosition(position);

	GoPolar(GetCompassOrientation(rc),-100);

	motion.steppers.SetFeedrate(FAST);

	if(wasAbsolute) motion.SetAbsolute();
	_probing = false;
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


void Robot::MatchPrimaryBlue(){
	motion.steppers.Engage();
	motion.SetAbsolute();
	Vec2 cakeRose1(575,1775);
	Vec2 cakeJaune1(775,1775);
	Vec2 cakeMarron1(1125,1275);

	Vec2 b1(225,1775);

	Vec2 ballBlueBegin(300,1125);
	Vec2 ballBlueEnd(150,1125);
	Vec2 ballBlueBasket(200,1675);

	Vec2 b1safe(450,1550);
	Vec2 b1safe2(300,1300);
	Vec2 b5safe(900,400);

	Vec2 depose1(200,1800);
	Vec2 depose2(350,1800);
	Vec2 depose3(450,1800);

	Vec2 retrait1(200,1650);
	Vec2 retrait2(350,1650);
	Vec2 retrait3(450,1650);
	
	actuators.Ungrab(RobotCompass::AB);
	Go(cakeRose1);
	actuators.Grab(RobotCompass::AB);
	Align(RobotCompass::BC, GetCompassOrientation(TableCompass::NORTH));

	actuators.Ungrab(RobotCompass::BC);
	Go(cakeJaune1);
	actuators.Grab(RobotCompass::BC);
	Align(RobotCompass::CA, 45);

	actuators.Ungrab(RobotCompass::CA);
	Go(cakeMarron1);
	actuators.Grab(RobotCompass::CA);

	Go(b1safe);
	//Dépose du premier Gateau
	Align(RobotCompass::BC, -120);
	Go(depose1);
	actuators.Ungrab(RobotCompass::BC);
	actuators.Unlock(RobotCompass::BC);
	delay(500);
	Go(retrait1);
	Go(retrait2);
	actuators.Close(RobotCompass::BC);
	//Dépose du second Gateau
	Align(RobotCompass::AB, -120);
	Go(depose2);
	actuators.Ungrab(RobotCompass::AB);
	actuators.Unlock(RobotCompass::AB);
	delay(500);
	Go(retrait2);
	Go(retrait3);
	actuators.Close(RobotCompass::AB);
	//Dépose du troisieme Gateau
	Align(RobotCompass::CA, -120);
	Go(depose3);
	actuators.Ungrab(RobotCompass::CA);
	actuators.Unlock(RobotCompass::CA);
	delay(500);
	Go(retrait3);
	actuators.Close(RobotCompass::CA);
	
	
	//recalage
	ProbeBorder(TableCompass::SOUTH, RobotCompass::C);
	motion.SetAbsolute();
	//Va chercher le balles
	Align(RobotCompass::A, GetCompassOrientation(TableCompass::WEST));
	Go(ballBlueBegin);
	Go(ballBlueEnd);
	// Va en dépose des balles
	Go(ballBlueBasket);
	Align(RobotCompass::A, GetCompassOrientation(TableCompass::SOUTH));
	//recalage
	ProbeBorder(TableCompass::SOUTH, RobotCompass::A);
	motion.SetAbsolute();
	// Dépose

	// Fin de match
	Go(b5safe);
	motion.steppers.Disengage();

}


void Robot::MatchPrimaryGreen(){
	motion.steppers.Engage();
	motion.SetAbsolute();
	Vec2 cakeRose1(575,2000 - 1775);
	Vec2 cakeJaune1(775,2000 - 1775);
	Vec2 cakeMarron1(1125,2000 - 1275);

	Vec2 b1(225,1775);

	Vec2 ballBlueBegin(300,2000 - 1125);
	Vec2 ballBlueEnd(150,2000 - 1125);
	Vec2 ballBlueBasket(200,2000 - 1675);

	Vec2 b1safe(450,2000 - 1550);
	Vec2 b1safe2(300,2000 - 1300);
	Vec2 b5safe(900,2000 - 400);

	Vec2 depose1(200,2000 - 1800);
	Vec2 depose2(350,2000 - 1800);
	Vec2 depose3(450,2000 - 1800);

	Vec2 retrait1(200,2000 - 1650);
	Vec2 retrait2(350,2000 - 1650);
	Vec2 retrait3(450,2000 - 1650);
	
	actuators.Ungrab(RobotCompass::AB);
	Go(cakeRose1);
	actuators.Grab(RobotCompass::AB);
	Align(RobotCompass::BC, GetCompassOrientation(TableCompass::NORTH));

	actuators.Ungrab(RobotCompass::BC);
	Go(cakeJaune1);
	actuators.Grab(RobotCompass::BC);
	Align(RobotCompass::CA, 45);

	actuators.Ungrab(RobotCompass::CA);
	Go(cakeMarron1);
	actuators.Grab(RobotCompass::CA);

	Go(b1safe);
	//Dépose du premier Gateau
	Align(RobotCompass::BC, -120);
	Go(depose1);
	actuators.Ungrab(RobotCompass::BC);
	actuators.Unlock(RobotCompass::BC);
	delay(500);
	Go(retrait1);
	Go(retrait2);
	actuators.Close(RobotCompass::BC);
	//Dépose du second Gateau
	Align(RobotCompass::AB, -120);
	Go(depose2);
	actuators.Ungrab(RobotCompass::AB);
	actuators.Unlock(RobotCompass::AB);
	delay(500);
	Go(retrait2);
	Go(retrait3);
	actuators.Close(RobotCompass::AB);
	//Dépose du troisieme Gateau
	Align(RobotCompass::CA, -120);
	Go(depose3);
	actuators.Ungrab(RobotCompass::CA);
	actuators.Unlock(RobotCompass::CA);
	delay(500);
	Go(retrait3);
	actuators.Close(RobotCompass::CA);
	
	
	//recalage
	ProbeBorder(TableCompass::SOUTH, RobotCompass::C);
	motion.SetAbsolute();
	//Va chercher le balles
	Align(RobotCompass::A, GetCompassOrientation(TableCompass::WEST));
	Go(ballBlueBegin);
	Go(ballBlueEnd);
	// Va en dépose des balles
	Go(ballBlueBasket);
	Align(RobotCompass::A, GetCompassOrientation(TableCompass::SOUTH));
	//recalage
	ProbeBorder(TableCompass::SOUTH, RobotCompass::A);
	motion.SetAbsolute();
	// Dépose

	// Fin de match
	Go(b5safe);
	motion.steppers.Disengage();

}

void Robot::MatchSecondaryGreen(){}
void Robot::MatchSecondaryBlue(){}


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