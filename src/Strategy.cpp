#include "Robot.h"
#include "Settings.h"
#include "Poi.h"

#include "debug/Console.h"


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