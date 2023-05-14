#include "extension/strategy/extStrategy.h"
#include "system.h"

#include "settings.h"
#include "math/geometry.h"
#include "map/poi.h"
#include "map/score.h"

using namespace POI;
using namespace Score;

ExtStrategy::ExtStrategy(System& sys) : system(sys), 
motion(*sys.motion), actuators(*sys.actuators){

}

ExtStrategy::~ExtStrategy(){
}


void ExtStrategy::recalagePrimaryBlue(){

    motion.steppers.engage();

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,0));
    probeBorder(TableCompass::SOUTH, RobotCompass::BC);
    probeBorder(TableCompass::EAST, RobotCompass::CA);
	motion.setAbsolute();
	system.e_motion->align(RobotCompass::AB, getCompassOrientation(TableCompass::NORTH));

    actuators.ungrab(RobotCompass::AB);

    motion.steppers.sleep();
}

void ExtStrategy::recalagePrimaryGreen(){
	
    motion.steppers.engage();

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,0));
    probeBorder(TableCompass::SOUTH, RobotCompass::BC);
    probeBorder(TableCompass::WEST, RobotCompass::AB);
	motion.setAbsolute();
	system.e_motion->align(RobotCompass::AB, getCompassOrientation(TableCompass::NORTH));

    actuators.ungrab(RobotCompass::AB);

    motion.steppers.sleep();
}

void ExtStrategy::recalageSecondaryBlue(){
    Console::info("Robot") << "Recalage Secondary Blue Cherry" << Console::endl;
    motion.steppers.engage();

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,PI));
    probeBorder(TableCompass::NORTH, RobotCompass::BC);
    probeBorder(TableCompass::EAST, RobotCompass::AB);
	motion.setAbsolute();
    system.e_motion->go(b3);
	system.e_motion->align(RobotCompass::A, getCompassOrientation(TableCompass::WEST));

    motion.steppers.sleep();
}

void ExtStrategy::recalageSecondaryGreen(){
    Console::info("Robot") << "Recalage Secondary Green Cherry" << Console::endl;
    motion.steppers.engage();

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,PI));
    probeBorder(TableCompass::NORTH, RobotCompass::BC);
    probeBorder(TableCompass::WEST, RobotCompass::CA);
	motion.setAbsolute();
    system.e_motion->go(v3);
	system.e_motion->align(RobotCompass::A, getCompassOrientation(TableCompass::EAST));

    motion.steppers.sleep();
}

void ExtStrategy::recalageSecondaryCakeBlue(){
    Console::info("Robot") << "Recalage Secondary Blue Cake" << Console::endl;
    motion.steppers.engage();

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,PI));
    probeBorder(TableCompass::NORTH, RobotCompass::BC);
    probeBorder(TableCompass::WEST, RobotCompass::AB);
	motion.setAbsolute();
    system.e_motion->go(b4);
	system.e_motion->align(RobotCompass::AB, getCompassOrientation(TableCompass::SOUTH));

    actuators.ungrab(RobotCompass::AB);

    motion.steppers.sleep();
}

void ExtStrategy::recalageSecondaryCakeGreen(){
    Console::info("Robot") << "Recalage Secondary Green Cake " << Console::endl;
    motion.steppers.engage();

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,PI));
    probeBorder(TableCompass::NORTH, RobotCompass::BC);
    probeBorder(TableCompass::EAST, RobotCompass::AB);
	motion.setAbsolute();
    system.e_motion->go(v4);
	system.e_motion->align(RobotCompass::AB, getCompassOrientation(TableCompass::SOUTH));

    actuators.ungrab(RobotCompass::AB);

    motion.steppers.sleep();
}

void ExtStrategy::matchPrimaryBlue(){

    motion.steppers.engage();
	motion.setAbsolute();

    system.addToScore(basket);

    actuators.ungrab(RobotCompass::AB);
	actuators.ungrab(RobotCompass::BC);
	actuators.ungrab(RobotCompass::CA);

    actuators.ungrab(RobotCompass::AB);
	system.e_motion->go(cakePinkSE);
	actuators.grab(RobotCompass::AB);
	system.e_motion->align(RobotCompass::BC, getCompassOrientation(TableCompass::NORTH));

    actuators.ungrab(RobotCompass::BC);
    system.e_motion->go(cakeYellowSE);
    actuators.grab(RobotCompass::BC);
    system.e_motion->align(RobotCompass::CA, 45);

    actuators.ungrab(RobotCompass::CA);
    system.e_motion->go(cakeBrownSE);
    actuators.grab(RobotCompass::CA);

    //----ATTENTION----
    system.disable(LIDAR);
    //Dépose du premier Gateau
    system.e_motion->align(RobotCompass::BC, -120);
    system.e_motion->go(retreatBlue1);
    actuators.unlock(RobotCompass::BC);
    system.e_motion->go(dropBlue1);
    actuators.ungrab(RobotCompass::BC);
    system.e_motion->go(retreatBlue2);
    actuators.close(RobotCompass::BC);
    system.addToScore(cakeWithCherry);

    //Dépose du second Gateau
    system.e_motion->align(RobotCompass::AB, -120);
    actuators.unlock(RobotCompass::AB);
    system.e_motion->go(dropBlue2);
    actuators.ungrab(RobotCompass::AB);
    system.e_motion->go(retreatBlue3);
    actuators.close(RobotCompass::AB);
    system.addToScore(cakeWithCherry);

    //Dépose du troisieme Gateau
    system.e_motion->align(RobotCompass::CA, -120);
    actuators.unlock(RobotCompass::CA);
    system.e_motion->go(dropBlue3);
    actuators.ungrab(RobotCompass::CA);
    system.e_motion->go(retreatBlue3);
    actuators.close(RobotCompass::CA);
    system.addToScore(cakeWithCherry);
    
    //recalage
    system.e_motion->go(retreatBlue1);
    probeBorder(TableCompass::SOUTH, RobotCompass::C);
    motion.setAbsolute();
    //Va chercher les balles
    system.e_motion->align(RobotCompass::A, getCompassOrientation(TableCompass::WEST));
    actuators.suckBall();
    system.e_motion->go(ballBeginSE);
    system.e_motion->go(ballEndSE);
    actuators.stopTurbine();
    // Va en dépose des balles
    system.e_motion->go(ballBlueBasket);
    system.e_motion->align(RobotCompass::A, getCompassOrientation(TableCompass::SOUTH));
    system.e_motion->go(ballBlueBasketPlus);
    // Dépose
    actuators.trap.open();
    system.wait(2000);
    actuators.trap.close();
    system.addToScore(exactCherryCounting);
    //recalage
    probeBorder(TableCompass::SOUTH, RobotCompass::A);
    motion.setAbsolute();

    //----ATTENTION----
    system.enable(LIDAR);

    // Fin de match
    system.e_motion->go(blueEndPrimary);
    system.addToScore(wheelsOnPlate/2);
	system.enable(NEOPIXEL);

    // Fin de match
    actuators.disengage();
	motion.steppers.disengage();

}

void ExtStrategy::matchPrimaryGreen(){
	motion.steppers.engage();
	motion.setAbsolute();

    system.addToScore(basket);

    actuators.ungrab(RobotCompass::AB);
    actuators.ungrab(RobotCompass::BC);
    actuators.ungrab(RobotCompass::CA);

    actuators.ungrab(RobotCompass::AB);
    system.e_motion->go(cakePinkSW);
    actuators.grab(RobotCompass::AB);
    system.e_motion->align(RobotCompass::BC, getCompassOrientation(TableCompass::NORTH));

    actuators.ungrab(RobotCompass::BC);
    system.e_motion->go(cakeYellowSW);
    actuators.grab(RobotCompass::BC);
    system.e_motion->align(RobotCompass::CA, -45);

    actuators.ungrab(RobotCompass::CA);
    system.e_motion->go(cakeBrownSW);
    actuators.grab(RobotCompass::CA);

    //----ATTENTION----
    system.disable(LIDAR);

    //Dépose du premier Gateau
    system.e_motion->align(RobotCompass::BC, 120);
    system.e_motion->go(retreatGreen1);
    actuators.unlock(RobotCompass::BC);
    system.e_motion->go(dropGreen1);
    actuators.ungrab(RobotCompass::BC);
    system.e_motion->go(retreatGreen2);
    actuators.close(RobotCompass::BC);
    system.addToScore(cakeWithCherry);

    //Dépose du second Gateau
    system.e_motion->align(RobotCompass::AB, 120);
    actuators.unlock(RobotCompass::AB);
    system.e_motion->go(dropGreen2);
    actuators.ungrab(RobotCompass::AB);
    system.e_motion->go(retreatGreen3);
    actuators.close(RobotCompass::AB);
    system.addToScore(cakeWithCherry);

    //Dépose du troisieme Gateau
    system.e_motion->align(RobotCompass::CA, 120);
    actuators.unlock(RobotCompass::CA);
    system.e_motion->go(dropGreen3);
    actuators.ungrab(RobotCompass::CA);
    system.e_motion->go(retreatGreen3);
    actuators.close(RobotCompass::CA);
    system.addToScore(cakeWithCherry);
    
    //recalage
    system.e_motion->go(retreatGreen1);
    probeBorder(TableCompass::SOUTH, RobotCompass::C);
    motion.setAbsolute(); // Todo : utile ? Déja fait dans probeBorder ?
    //Va chercher les balles
    actuators.suckBall();
    system.e_motion->align(RobotCompass::A, getCompassOrientation(TableCompass::EAST));
    // Todo Probe Ball support
    system.e_motion->go(ballEndSW);
    ProbeObstacle(Vec2(200,990),TableCompass::EAST,RobotCompass::A); // Todo : POI pour probe obstacle
    system.e_motion->go(ballEndSW);
    system.e_motion->go(ballBeginSW);
    actuators.stopTurbine();
    // Va en dépose des balles
    system.e_motion->go(ballGreenBasket);
    system.e_motion->align(RobotCompass::A, getCompassOrientation(TableCompass::SOUTH));
    system.e_motion->go(ballGreenBasketPlus);
    // Dépose
    actuators.trap.open();
    system.wait(2000);
    actuators.trap.close();
    system.addToScore(exactCherryCounting);
    //recalage
    probeBorder(TableCompass::SOUTH, RobotCompass::A);
    motion.setAbsolute();

    //----ATTENTION----
    system.enable(LIDAR);

    // Fin de match
    system.e_motion->go(greenEndPrimary);
    system.addToScore(wheelsOnPlate/2);
    system.enable(NEOPIXEL);

    // Fin de match
    actuators.disengage();
	motion.steppers.disengage();

}

void ExtStrategy::matchSecondaryBlue(){
    motion.steppers.engage();
	motion.setAbsolute();

    //----ATTENTION----
    system.disable(LIDAR);

    // Va chercher les balles
    // Lance la turbine
    actuators.suckBall();
    // Prendre les balles
    system.e_motion->align(RobotCompass::A, getCompassOrientation(TableCompass::WEST));
    system.e_motion->go(ballEndNE);
    system.e_motion->go(ballBeginNE);
    // Stop la turbine
    actuators.stopTurbine();

    // Ouvre tous les actionneurs pour gagner du temps
    actuators.ungrab(RobotCompass::AB);
    actuators.ungrab(RobotCompass::BC);
    actuators.ungrab(RobotCompass::CA);
    // Postionnement et orientation "comme en vert"
    system.e_motion->go(v4);

    //----ATTENTION----
    system.enable(LIDAR);

    system.e_motion->align(RobotCompass::AB, getCompassOrientation(TableCompass::SOUTH));
    // Va chercher les gateaux
    actuators.ungrab(RobotCompass::AB);
    system.e_motion->go(cakePinkNE);
    actuators.grab(RobotCompass::AB);
    system.e_motion->align(RobotCompass::BC, getCompassOrientation(TableCompass::SOUTH));

    actuators.ungrab(RobotCompass::BC);
    system.e_motion->go(cakeYellowNE);
    actuators.grab(RobotCompass::BC);
    system.e_motion->align(RobotCompass::CA, 135);

    actuators.ungrab(RobotCompass::CA);
    system.e_motion->go(cakeBrownNE);
    actuators.grab(RobotCompass::CA);
    
    // Depose les gateaux
    system.e_motion->align(RobotCompass::AB, getCompassOrientation(TableCompass::EAST));
    actuators.unlock(RobotCompass::AB);
    system.e_motion->go(dropB2_01);
    actuators.ungrab(RobotCompass::AB);
    system.addToScore(cakeWithCherry);

    actuators.unlock(RobotCompass::BC);
    actuators.unlock(RobotCompass::CA);

    system.e_motion->go(dropB2_02);
    system.e_motion->align(RobotCompass::BC, getCompassOrientation(TableCompass::EAST));
    actuators.ungrab(RobotCompass::BC);
    system.addToScore(cakeWithCherry);

    system.e_motion->go(dropB2_03);
    system.e_motion->align(RobotCompass::CA, getCompassOrientation(TableCompass::EAST));
    actuators.ungrab(RobotCompass::CA);
    system.addToScore(cakeWithCherry);

    system.e_motion->go(cakeBrownNE);
    //Fin du match
    system.e_motion->go(blueEndSecondary);

    system.addToScore(wheelsOnPlate/2);

    // Fin de match
    actuators.trap.open();
    system.wait(1000);
    actuators.disengage();
    motion.steppers.disengage();
}

void ExtStrategy::matchSecondaryGreen(){
    motion.steppers.engage();
	motion.setAbsolute();

    //----ATTENTION----
    system.disable(LIDAR);

    // Va chercher les balles
    // Lance la turbine
    actuators.suckBall();
    // Prendre les balles
    system.e_motion->align(RobotCompass::A, getCompassOrientation(TableCompass::EAST));
    system.e_motion->go(ballEndNW);
    system.e_motion->go(ballBeginNW);
    //Stop Turbine
    actuators.stopTurbine();

    // Ouvre tous les actionneurs pour gagner du temps
    actuators.ungrab(RobotCompass::AB);
    actuators.ungrab(RobotCompass::BC);
    actuators.ungrab(RobotCompass::CA);
    // Postionnement et orientation "comme en vert"
    system.e_motion->go(b4);

    //----ATTENTION----
    system.enable(LIDAR);

    system.e_motion->align(RobotCompass::AB, getCompassOrientation(TableCompass::SOUTH));
    // Va chercher les gateaux
    actuators.ungrab(RobotCompass::AB);
    system.e_motion->go(cakePinkNW);
    actuators.grab(RobotCompass::AB);
    system.e_motion->align(RobotCompass::BC, getCompassOrientation(TableCompass::SOUTH));

    actuators.ungrab(RobotCompass::BC);
    system.e_motion->go(cakeYellowNW);
    actuators.grab(RobotCompass::BC);
    system.e_motion->align(RobotCompass::CA, -135);

    actuators.ungrab(RobotCompass::CA);
    system.e_motion->go(cakeBrownNW);
    actuators.grab(RobotCompass::CA);
    
    // Depose les gateaux
    system.e_motion->align(RobotCompass::AB, getCompassOrientation(TableCompass::WEST));
    actuators.unlock(RobotCompass::AB);
    system.e_motion->go(dropV2_01);
    actuators.ungrab(RobotCompass::AB);
    system.addToScore(cakeWithCherry);

    actuators.unlock(RobotCompass::BC);
    actuators.unlock(RobotCompass::CA);

    system.e_motion->go(dropV2_02);
    system.e_motion->align(RobotCompass::BC, getCompassOrientation(TableCompass::WEST));
    actuators.ungrab(RobotCompass::BC);
    system.addToScore(cakeWithCherry);

    system.e_motion->go(dropV2_03);
    system.e_motion->align(RobotCompass::CA, getCompassOrientation(TableCompass::WEST));
    actuators.ungrab(RobotCompass::CA);
    system.addToScore(cakeWithCherry);

    system.e_motion->go(cakeBrownNW);
    //Fin du match
    system.e_motion->go(greenEndSecondary);

    system.addToScore(wheelsOnPlate/2);

    // Fin de match
    actuators.trap.open();
    system.wait(1000);
    actuators.disengage();
    motion.steppers.disengage();
}

void ExtStrategy::matchSecondaryCakeBlue(){
    motion.steppers.engage();
	motion.setAbsolute();

    actuators.ungrab(RobotCompass::AB);
	actuators.ungrab(RobotCompass::BC);
	actuators.ungrab(RobotCompass::CA);

    actuators.ungrab(RobotCompass::AB);
	system.e_motion->go(cakePinkNW);
	actuators.grab(RobotCompass::AB);
	system.e_motion->align(RobotCompass::BC, getCompassOrientation(TableCompass::SOUTH));

    actuators.ungrab(RobotCompass::BC);
    system.e_motion->go(cakeYellowNW);
    actuators.grab(RobotCompass::BC);
    system.e_motion->align(RobotCompass::CA, -135);

    actuators.ungrab(RobotCompass::CA);
    system.e_motion->go(cakeBrownNW);
    actuators.grab(RobotCompass::CA);

    // Depose les gateaux
    system.e_motion->align(RobotCompass::AB, getCompassOrientation(TableCompass::EAST));
    actuators.unlock(RobotCompass::AB);
    system.e_motion->go(dropB2_01);
    actuators.ungrab(RobotCompass::AB);
    system.addToScore(cakeWithCherry);

    actuators.unlock(RobotCompass::BC);
    actuators.unlock(RobotCompass::CA);

    system.e_motion->go(dropB2_02);
    system.e_motion->align(RobotCompass::BC, getCompassOrientation(TableCompass::EAST));
    actuators.ungrab(RobotCompass::BC);
    system.addToScore(cakeWithCherry);

    system.e_motion->go(dropB2_03);
    system.e_motion->align(RobotCompass::CA, getCompassOrientation(TableCompass::EAST));
    actuators.ungrab(RobotCompass::CA);
    system.addToScore(cakeWithCherry);

    system.e_motion->go(cakeBrownNE);

    //Fin du match
    system.e_motion->go(blueEndSecondary);

    system.addToScore(wheelsOnPlate/2);

    // Fin de match
    actuators.trap.open();
    system.wait(1000);
    actuators.disengage();
    motion.steppers.disengage();
}

void ExtStrategy::matchSecondaryCakeGreen(){
    motion.steppers.engage();
	motion.setAbsolute();

    actuators.ungrab(RobotCompass::AB);
	actuators.ungrab(RobotCompass::BC);
	actuators.ungrab(RobotCompass::CA);

    actuators.ungrab(RobotCompass::AB);
	system.e_motion->go(cakePinkNE);
	actuators.grab(RobotCompass::AB);
	system.e_motion->align(RobotCompass::BC, getCompassOrientation(TableCompass::SOUTH));

    actuators.ungrab(RobotCompass::BC);
    system.e_motion->go(cakeYellowNE);
    actuators.grab(RobotCompass::BC);
    system.e_motion->align(RobotCompass::CA, 135);

    actuators.ungrab(RobotCompass::CA);
    system.e_motion->go(cakeBrownNE);
    actuators.grab(RobotCompass::CA);

    // Depose les gateaux
    system.e_motion->align(RobotCompass::AB, getCompassOrientation(TableCompass::WEST));
    actuators.unlock(RobotCompass::AB);
    system.e_motion->go(dropV2_01);
    actuators.ungrab(RobotCompass::AB);
    system.addToScore(cakeWithCherry);

    actuators.unlock(RobotCompass::BC);
    actuators.unlock(RobotCompass::CA);

    system.e_motion->go(dropV2_02);
    system.e_motion->align(RobotCompass::BC, getCompassOrientation(TableCompass::WEST));
    actuators.ungrab(RobotCompass::BC);
    system.addToScore(cakeWithCherry);

    system.e_motion->go(dropV2_03);
    system.e_motion->align(RobotCompass::CA, getCompassOrientation(TableCompass::WEST));
    actuators.ungrab(RobotCompass::CA);
    system.addToScore(cakeWithCherry);

    system.e_motion->go(cakeBrownNW);

    //Fin du match
    system.e_motion->go(greenEndSecondary);

    system.addToScore(wheelsOnPlate/2);

    // Fin de match
    actuators.trap.open();
    system.wait(1000);
    actuators.disengage();
    motion.steppers.disengage();
}


void ExtStrategy::nearlyFinishPrimaryBlue(){
    // go to End Position
    system.e_motion->go(blueEndPrimary);
    system.addToScore(wheelsOnPlate/2);
}

void ExtStrategy::nearlyFinishPrimaryGreen(){
    // go to End Position
    system.e_motion->go(greenEndPrimary);
    system.addToScore(wheelsOnPlate/2);
}

void ExtStrategy::nearlyFinishSecondaryBlue(){
    // go to End Position
    system.e_motion->go(blueEndSecondary);
    system.addToScore(wheelsOnPlate/2);
}

void ExtStrategy::nearlyFinishSecondaryGreen(){
    // go to End Position
    system.e_motion->go(greenEndSecondary);
    system.addToScore(wheelsOnPlate/2);
}

void ExtStrategy::finishPrimaryBlue(){
    // Fin de match
    system.enable(NEOPIXEL);
    actuators.disengage();
    motion.steppers.disengage();
}

void ExtStrategy::finishPrimaryGreen(){
    // Fin de match
    system.enable(NEOPIXEL);
    actuators.disengage();
    motion.steppers.disengage();
}

void ExtStrategy::finishSecondaryBlue(){
    // Fin de match
    actuators.disengage();
    motion.steppers.disengage();
}

void ExtStrategy::finishSecondaryGreen(){
    // Fin de match
    actuators.disengage();
    motion.steppers.disengage();
}

