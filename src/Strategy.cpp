#include "Robot.h"
#include "Settings.h"
#include "Poi.h"
#include "Points.h"

#include "debug/Console.h"

using namespace POI;
using namespace Score;


void Robot::RecalagePrimaryBlue(){

    motion.steppers.Engage();

    actuators.Close(RobotCompass::AB);
    actuators.Close(RobotCompass::BC);
    actuators.Close(RobotCompass::CA);

    motion.SetAbsPosition(Vec3(-1,-1,0));
    ProbeBorder(TableCompass::SOUTH, RobotCompass::BC);
    ProbeBorder(TableCompass::EAST, RobotCompass::CA);
	motion.SetAbsolute();
	Align(RobotCompass::AB, GetCompassOrientation(TableCompass::NORTH));

    actuators.Ungrab(RobotCompass::AB);

    motion.steppers.Sleep();
}

void Robot::RecalagePrimaryGreen(){
	
    motion.steppers.Engage();

    actuators.Close(RobotCompass::AB);
    actuators.Close(RobotCompass::BC);
    actuators.Close(RobotCompass::CA);

    motion.SetAbsPosition(Vec3(-1,-1,0));
    ProbeBorder(TableCompass::SOUTH, RobotCompass::BC);
    ProbeBorder(TableCompass::WEST, RobotCompass::AB);
	motion.SetAbsolute();
	Align(RobotCompass::AB, GetCompassOrientation(TableCompass::NORTH));

    actuators.Ungrab(RobotCompass::AB);

    motion.steppers.Sleep();
}

void Robot::RecalageSecondaryBlue(){
    Console::info("Robot") << "Recalage Secondary Blue" << Console::endl;
    motion.steppers.Engage();

    actuators.Close(RobotCompass::AB);
    actuators.Close(RobotCompass::BC);
    actuators.Close(RobotCompass::CA);

    motion.SetAbsPosition(Vec3(-1,-1,PI));
    ProbeBorder(TableCompass::NORTH, RobotCompass::BC);
    ProbeBorder(TableCompass::EAST, RobotCompass::AB);
	motion.SetAbsolute();
    Go(b3);
	Align(RobotCompass::A, GetCompassOrientation(TableCompass::WEST));

    motion.steppers.Sleep();
}

void Robot::RecalageSecondaryGreen(){
    Console::info("Robot") << "Recalage Secondary Green" << Console::endl;
    motion.steppers.Engage();

    actuators.Close(RobotCompass::AB);
    actuators.Close(RobotCompass::BC);
    actuators.Close(RobotCompass::CA);

    motion.SetAbsPosition(Vec3(-1,-1,PI));
    ProbeBorder(TableCompass::NORTH, RobotCompass::BC);
    ProbeBorder(TableCompass::WEST, RobotCompass::CA);
	motion.SetAbsolute();
    Go(v3);
	Align(RobotCompass::A, GetCompassOrientation(TableCompass::EAST));

    motion.steppers.Sleep();
}


void Robot::MatchPrimaryBlue(){

    motion.steppers.Engage();
	motion.SetAbsolute();

    match.AddToScore(basket);

    actuators.Ungrab(RobotCompass::AB);
	actuators.Ungrab(RobotCompass::BC);
	actuators.Ungrab(RobotCompass::CA);

    actuators.Ungrab(RobotCompass::AB);
	Go(cakePinkSE);
	actuators.Grab(RobotCompass::AB);
	Align(RobotCompass::BC, GetCompassOrientation(TableCompass::NORTH));

    actuators.Ungrab(RobotCompass::BC);
    Go(cakeYellowSE);
    actuators.Grab(RobotCompass::BC);
    Align(RobotCompass::CA, 45);

    actuators.Ungrab(RobotCompass::CA);
    Go(cakeBrownSE);
    actuators.Grab(RobotCompass::CA);

    //Dépose du premier Gateau
    Align(RobotCompass::BC, -120);
    Go(retreatBlue1);
    actuators.Unlock(RobotCompass::BC);
    Go(dropBlue1);
    actuators.Ungrab(RobotCompass::BC);
    Go(retreatBlue2);
    actuators.Close(RobotCompass::BC);
    match.AddToScore(cakeWithCherry);

    //Dépose du second Gateau
    Align(RobotCompass::AB, -120);
    actuators.Unlock(RobotCompass::AB);
    Go(dropBlue2);
    actuators.Ungrab(RobotCompass::AB);
    Go(retreatBlue3);
    actuators.Close(RobotCompass::AB);
    match.AddToScore(cakeWithCherry);

    //Dépose du troisieme Gateau
    Align(RobotCompass::CA, -120);
    actuators.Unlock(RobotCompass::CA);
    Go(dropBlue3);
    actuators.Ungrab(RobotCompass::CA);
    Go(retreatBlue3);
    actuators.Close(RobotCompass::CA);
    match.AddToScore(cakeWithCherry);
    
    //recalage
    Go(retreatBlue1);
    ProbeBorder(TableCompass::SOUTH, RobotCompass::C);
    motion.SetAbsolute();
    //Va chercher les balles
    Align(RobotCompass::A, GetCompassOrientation(TableCompass::WEST));
    //setTurbine(255);
    Go(ballBeginSE);
    Go(ballEndSE);
    //setTurbine(0);
    // Va en dépose des balles
    Go(ballBlueBasket);
    Align(RobotCompass::A, GetCompassOrientation(TableCompass::SOUTH));
    // Dépose
    actuators.trap.open();
    Wait(2000);
    actuators.trap.close();
    match.AddToScore(exactCherryCounting);
    //recalage
    ProbeBorder(TableCompass::SOUTH, RobotCompass::A);
    motion.SetAbsolute();
    // Fin de match
    Go(blueEndPrimary);
    match.AddToScore(wheelsOnPlate/2);
	// Fin de match
	motion.steppers.Disengage();

}

void Robot::MatchPrimaryGreen(){
	motion.steppers.Engage();
	motion.SetAbsolute();

    match.AddToScore(basket);

    actuators.Ungrab(RobotCompass::AB);
    actuators.Ungrab(RobotCompass::BC);
    actuators.Ungrab(RobotCompass::CA);

    actuators.Ungrab(RobotCompass::AB);
    Go(cakePinkSW);
    actuators.Grab(RobotCompass::AB);
    Align(RobotCompass::BC, GetCompassOrientation(TableCompass::NORTH));

    actuators.Ungrab(RobotCompass::BC);
    Go(cakeYellowSW);
    actuators.Grab(RobotCompass::BC);
    Align(RobotCompass::CA, -45);

    actuators.Ungrab(RobotCompass::CA);
    Go(cakeBrownSW);
    actuators.Grab(RobotCompass::CA);

    //Dépose du premier Gateau
    Align(RobotCompass::BC, 120);
    Go(retreatGreen1);
    actuators.Unlock(RobotCompass::BC);
    Go(dropGreen1);
    actuators.Ungrab(RobotCompass::BC);
    Go(retreatGreen2);
    actuators.Close(RobotCompass::BC);
    match.AddToScore(cakeWithCherry);

    //Dépose du second Gateau
    Align(RobotCompass::AB, 120);
    actuators.Unlock(RobotCompass::AB);
    Go(dropGreen2);
    actuators.Ungrab(RobotCompass::AB);
    Go(retreatGreen3);
    actuators.Close(RobotCompass::AB);
    match.AddToScore(cakeWithCherry);

    //Dépose du troisieme Gateau
    Align(RobotCompass::CA, 120);
    actuators.Unlock(RobotCompass::CA);
    Go(dropGreen3);
    actuators.Ungrab(RobotCompass::CA);
    Go(retreatGreen3);
    actuators.Close(RobotCompass::CA);
    match.AddToScore(cakeWithCherry);
    
    //recalage
    Go(retreatGreen1);
    ProbeBorder(TableCompass::SOUTH, RobotCompass::C);
    motion.SetAbsolute();
    //Va chercher les balles
    Align(RobotCompass::A, GetCompassOrientation(TableCompass::EAST));
    //setTurbine(255);
    Go(ballBeginSW);
    Go(ballEndSW);
    //setTurbine(0);
    // Va en dépose des balles
    Go(ballGreenBasket);
    Align(RobotCompass::A, GetCompassOrientation(TableCompass::SOUTH));
    // Dépose
    actuators.trap.open();
    Wait(2000);
    actuators.trap.close();
    match.AddToScore(exactCherryCounting);
    //recalage
    ProbeBorder(TableCompass::SOUTH, RobotCompass::A);
    motion.SetAbsolute();
    // Fin de match
    Go(greenEndPrimary);
    match.AddToScore(wheelsOnPlate/2);

    // Fin de match
	motion.steppers.Disengage();

}

void Robot::MatchSecondaryBlue(){
    motion.steppers.Engage();
	motion.SetAbsolute();

    // Va chercher les balles
    // Lance la turbine
    //setTurbine(255);
    // Prendre les balles
    Align(RobotCompass::A, GetCompassOrientation(TableCompass::WEST));
    Go(ballEndNE);
    Go(ballBeginNE);
    // Stop la turbine
    //setTurbine(0);

    // Ouvre tous les actionneurs pour gagner du temps
    actuators.Ungrab(RobotCompass::AB);
    actuators.Ungrab(RobotCompass::BC);
    actuators.Ungrab(RobotCompass::CA);
    // Postionnement et orientation "comme en vert"
    Go(v4);

    actuators.trap.open();
    Wait(2000);
    actuators.trap.close();

    Align(RobotCompass::AB, GetCompassOrientation(TableCompass::SOUTH));
    // Va chercher les gateaux
    actuators.Ungrab(RobotCompass::AB);
    Go(cakePinkNE);
    actuators.Grab(RobotCompass::AB);
    Align(RobotCompass::BC, GetCompassOrientation(TableCompass::SOUTH));

    actuators.Ungrab(RobotCompass::BC);
    Go(cakeYellowNE);
    actuators.Grab(RobotCompass::BC);
    Align(RobotCompass::CA, 135);

    actuators.Ungrab(RobotCompass::CA);
    Go(cakeBrownNE);
    actuators.Grab(RobotCompass::CA);
    
    // Depose les gateaux
    Align(RobotCompass::AB, GetCompassOrientation(TableCompass::EAST));
    actuators.Unlock(RobotCompass::AB);
    Go(dropB2_01);
    actuators.Ungrab(RobotCompass::AB);
    match.AddToScore(cakeWithCherry);

    actuators.Unlock(RobotCompass::BC);
    actuators.Unlock(RobotCompass::CA);

    Go(dropB2_02);
    Align(RobotCompass::BC, GetCompassOrientation(TableCompass::EAST));
    actuators.Ungrab(RobotCompass::BC);
    match.AddToScore(cakeWithCherry);

    Go(dropB2_03);
    Align(RobotCompass::CA, GetCompassOrientation(TableCompass::EAST));
    actuators.Ungrab(RobotCompass::CA);
    match.AddToScore(cakeWithCherry);

    Go(cakeBrownNE);
    //Fin du match
    Go(blueEndSecondary);

    match.AddToScore(wheelsOnPlate/2);

    // Fin de match
    motion.steppers.Disengage();
}

void Robot::MatchSecondaryGreen(){
    motion.steppers.Engage();
	motion.SetAbsolute();

    // Va chercher les balles
    // Lance la turbine
    //setTurbine(255);
    // Prendre les balles
    Align(RobotCompass::A, GetCompassOrientation(TableCompass::EAST));
    Go(ballEndNW);
    Go(ballBeginNW);
    // Stop la turbine
    //setTurbine(0);

    // Ouvre tous les actionneurs pour gagner du temps
    actuators.Ungrab(RobotCompass::AB);
    actuators.Ungrab(RobotCompass::BC);
    actuators.Ungrab(RobotCompass::CA);
    // Postionnement et orientation "comme en vert"
    Go(b4);

    actuators.trap.open();
    Wait(2000);
    actuators.trap.close();

    Align(RobotCompass::AB, GetCompassOrientation(TableCompass::SOUTH));
    // Va chercher les gateaux
    actuators.Ungrab(RobotCompass::AB);
    Go(cakePinkNW);
    actuators.Grab(RobotCompass::AB);
    Align(RobotCompass::BC, GetCompassOrientation(TableCompass::SOUTH));

    actuators.Ungrab(RobotCompass::BC);
    Go(cakeYellowNW);
    actuators.Grab(RobotCompass::BC);
    Align(RobotCompass::CA, -135);

    actuators.Ungrab(RobotCompass::CA);
    Go(cakeBrownNW);
    actuators.Grab(RobotCompass::CA);
    
    // Depose les gateaux
    Align(RobotCompass::AB, GetCompassOrientation(TableCompass::WEST));
    actuators.Unlock(RobotCompass::AB);
    Go(dropV2_01);
    actuators.Ungrab(RobotCompass::AB);
    match.AddToScore(cakeWithCherry);

    actuators.Unlock(RobotCompass::BC);
    actuators.Unlock(RobotCompass::CA);

    Go(dropV2_02);
    Align(RobotCompass::BC, GetCompassOrientation(TableCompass::WEST));
    actuators.Ungrab(RobotCompass::BC);
    match.AddToScore(cakeWithCherry);

    Go(dropV2_03);
    Align(RobotCompass::CA, GetCompassOrientation(TableCompass::WEST));
    actuators.Ungrab(RobotCompass::CA);
    match.AddToScore(cakeWithCherry);

    Go(cakeBrownNW);
    //Fin du match
    Go(greenEndSecondary);

    match.AddToScore(wheelsOnPlate/2);

    // Fin de match
    motion.steppers.Disengage();
}