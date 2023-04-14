#include "Robot.h"
#include "Settings.h"

Robot::Robot(){
	_state = RobotState::IDLE;
	_team = Settings::Match::GREEN;
	_avoidance = Settings::Match::AVOIDANCE;
}

bool Robot::WaitLaunch(){
	while (_state != RobotState::STARTING){
		Update();
		
		switch (_state){
		case RobotState::IDLE :
			if(ui.inputs.starter.GetState()){
				_state = RobotState::ARMED;
				FreezeSettings();
			}
			if(ui.inputs.resetButton.GetState()){
				Recalage();
			}
			break;

		case RobotState::ARMED :
			if(!ui.inputs.starter.GetState() && !ui.inputs.resetButton.GetState()){
				_state = RobotState::STARTING;
			}else if(!ui.inputs.starter.GetState() && ui.inputs.resetButton.GetState()){
				_state = RobotState::IDLE;
				UnfreezeSettings();
			}
			break;
	
		default:
			break;
		}
	}

	delay(10); // ?
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

void Robot::TestSteppers(){
	//motion.Go(d, 0);

	motion.steppers.Engage();

	motion.steppers.Move(Vec3(1000,0,0));
	Wait(1000);	
	motion.steppers.Move(Vec3(0,1000,0));
	Wait(1000);	
	motion.steppers.Move(Vec3(0,0,1000));
	Wait(1000);	

	motion.steppers.Disengage();
}

void Robot::Recalage(){
	// Laisser passer le robot secondaire
	//SetAvoidance(false);
	//SetRelative();
	actuators.Close(RobotCompass::AB);
	actuators.Close(RobotCompass::BC);
	actuators.Close(RobotCompass::CA);
	// ------
	motion.SetPosition(Vec3(-1,-1,0));
	motion.ProbeBorder(TableCompass::SOUTH, RobotCompass::BC);
	motion.ProbeBorder(TableCompass::EAST, RobotCompass::CA);
	motion.SetAbsolute();
	motion.Turn(-60);
	actuators.Ungrab(RobotCompass::AB);
	actuators.Ungrab(RobotCompass::BC);
	actuators.Ungrab(RobotCompass::CA);

	motion.steppers.Disengage();
}


void Robot::Match(){
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
	motion.Go(cakeRose1);
	actuators.Grab(RobotCompass::AB);
	motion.Align(RobotCompass::BC, GetCompassOrientation(TableCompass::NORTH));

	actuators.Ungrab(RobotCompass::BC);
	motion.Go(cakeJaune1);
	actuators.Grab(RobotCompass::BC);
	motion.Align(RobotCompass::CA, 45);

	actuators.Ungrab(RobotCompass::CA);
	motion.Go(cakeMarron1);
	actuators.Grab(RobotCompass::CA);

	motion.Go(b1safe);
	//Dépose du premier Gateau
	motion.Align(RobotCompass::BC, -120);
	motion.Go(depose1);
	actuators.Ungrab(RobotCompass::BC);
	actuators.Unlock(RobotCompass::BC);
	delay(500);
	motion.Go(retrait1);
	motion.Go(retrait2);
	actuators.Close(RobotCompass::BC);
	//Dépose du second Gateau
	motion.Align(RobotCompass::AB, -120);
	motion.Go(depose2);
	actuators.Ungrab(RobotCompass::AB);
	actuators.Unlock(RobotCompass::AB);
	delay(500);
	motion.Go(retrait2);
	motion.Go(retrait3);
	actuators.Close(RobotCompass::AB);
	//Dépose du troisieme Gateau
	motion.Align(RobotCompass::CA, -120);
	motion.Go(depose3);
	actuators.Ungrab(RobotCompass::CA);
	actuators.Unlock(RobotCompass::CA);
	delay(500);
	motion.Go(retrait3);
	actuators.Close(RobotCompass::CA);
	
	
	//recalage
	motion.ProbeBorder(TableCompass::SOUTH, RobotCompass::C);
	motion.SetAbsolute();
	//Va chercher le balles
	motion.Align(RobotCompass::A, GetCompassOrientation(TableCompass::WEST));
	motion.Go(ballBlueBegin);
	motion.Go(ballBlueEnd);
	// Va en dépose des balles
	motion.Go(ballBlueBasket);
	motion.Align(RobotCompass::A, GetCompassOrientation(TableCompass::SOUTH));
	//recalage
	motion.ProbeBorder(TableCompass::SOUTH, RobotCompass::A);
	motion.SetAbsolute();
	// Dépose

	// Fin de match
	motion.Go(b5safe);
	motion.steppers.Disengage();

}