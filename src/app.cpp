#include "app.h"
#include "match/poi.h"
#include "match/score.h"

#define lidar (*_lidarPtr)
#define screen (*_screenPtr)
#define inputs (*_inputsPtr)
#define motion (*_motionPtr)
#define planner (*_plannerPtr)
#define neopixel (*_neopixelPtr)
#define actuators (*_actuatorsPtr)

using namespace POI;
using namespace Score;

/*
unsigned long lastSeen = 0;
void OnDummyRequestResponse(String answer){
    if(answer.startsWith("obstacle")) obstacle = true;
	else obstacle = false;
}
*/

void SystemApplication::addScore(int points, int multiplicateur){
    _score += (points * multiplicateur);
}

int SystemApplication::score(){
    return _score;
}

void SystemApplication::connectModules(){
    //Tracked values
	//TODO Create events to handle this at the UI Level

    /*
	screen.x.SetValue(motion.getAbsPosition().x);
	screen.y.SetValue(motion.getAbsPosition().y);
	screen.z.SetValue(motion.getAbsPosition().z);
	screen.probed.SetValue(isProbed());
	screen.probing.SetValue(isProbing());
	screen.armed.SetValue(_state != RobotState::ARMED && _state != RobotState::IDLE);
	screen.started.SetValue(_state == RobotState::STARTED);
	screen.intercom.SetValue(lidar.isConnected());
	screen.time.SetValue(getScore());
	screen.score.SetValue(chrono.getTimeLeftSeconds());
	*/



}

SystemApplication::SystemApplication(){
    //Standalone modules
    _lidarPtr = std::make_unique<Lidar>();
    _screenPtr = std::make_unique<Screen>();
    _inputsPtr = std::make_unique<Inputs>();
    _motionPtr = std::make_unique<Motion>();
    _plannerPtr = std::make_unique<Planner>();
    _neopixelPtr = std::make_unique<NeoPixel>();
    _actuatorsPtr = std::make_unique<Actuators>();

    system.registerModule(_lidarPtr.get());
    system.registerModule(_screenPtr.get());
    system.registerModule(_inputsPtr.get());
    system.registerModule(_motionPtr.get());
    system.registerModule(_plannerPtr.get());
    system.registerModule(_neopixelPtr.get());
    system.registerModule(_actuatorsPtr.get());
}

SystemApplication::~SystemApplication(){}

void SystemApplication::startMatch(){
	chrono.start();
	screen.setPage(Page::MATCH);
	//actuators.Engage();
	motion.steppers.engage();
	_state = RobotState::STARTED;

	testMotion(); motion.steppers.disengage(); return;
	//TestSteppers(); motion.steppers.Disengage(); return;
	//TestDetection(); motion.steppers.Disengage(); return;

	if	   (inputs.isBlue()  && inputs.isPrimary())                         matchPrimaryBlue();
	else if(inputs.isBlue()  && inputs.isSecondary() && inputs.isCherry())  matchSecondaryBlue();
	else if(inputs.isBlue()  && inputs.isSecondary() && inputs.isCake())    matchSecondaryCakeBlue();

	else if(inputs.isGreen() && inputs.isPrimary())                         matchPrimaryGreen();
	else if(inputs.isGreen() && inputs.isSecondary() && inputs.isCherry())  matchSecondaryGreen();
	else if(inputs.isGreen() && inputs.isSecondary() && inputs.isCake())    matchSecondaryCakeGreen();
	
    motion.steppers.disengage();
}



void SystemApplication::handleFinishedMatch(){
	if(_state == RobotState::STARTED || _state == RobotState::FINISHING){
		_state = RobotState::FINISHED;
		motion.cancel();
		if	   (inputs.isBlue()  && inputs.isPrimary())     finishPrimaryBlue();
		else if(inputs.isBlue()  && inputs.isSecondary())   finishSecondaryBlue();
		else if(inputs.isGreen() && inputs.isPrimary())     finishPrimaryGreen();
		else if(inputs.isGreen() && inputs.isSecondary())   finishSecondaryGreen();

		motion.steppers.disengage();
		actuators.disengage();
		system.enable(NEOPIXEL);

		screen.update();
		//Suicide
		while (true){delay(100);}
	}
}

void SystemApplication::handleNearlyFinishedMatch(){
	if(_state == RobotState::STARTED){
		_state = RobotState::FINISHING;
		motion.cancel();
		if	   (inputs.isBlue()  && inputs.isPrimary())     nearlyFinishPrimaryBlue();
		else if(inputs.isBlue()  && inputs.isSecondary())   nearlyFinishSecondaryBlue();
        else if(inputs.isGreen() && inputs.isPrimary())     nearlyFinishPrimaryGreen();
		else if(inputs.isGreen() && inputs.isSecondary())   nearlyFinishSecondaryGreen();
		motion.steppers.disengage();
	}
}


void SystemApplication::go(Vec2 v){
	motion.goAsync(v);
	system.waitUntil(motion.getCurrentJob());
}

void SystemApplication::go(float x, float y){
	motion.goAsync(x, y);
	system.waitUntil(motion.getCurrentJob());
}

void SystemApplication::goAlign(Vec2 target, RobotCompass rc, float orientation){
    motion.goAlignAsync(target, rc, orientation);
	system.waitUntil(motion.getCurrentJob());
}

void SystemApplication::turn(float a){
	motion.turnAsync(a);
	system.waitUntil(motion.getCurrentJob());
}

void SystemApplication::align(RobotCompass rc, float orientation){
 	bool wasRelative = motion.isRelative();
	motion.setAbsolute();
	motion.alignAsync(rc, orientation);
	system.waitUntil(motion.getCurrentJob());
	if(wasRelative) motion.setRelative();
}

void SystemApplication::goPolar(float heading, float length){
	PolarVec target = PolarVec(heading*DEG_TO_RAD, length);
	bool wasAbsolute = motion.isAbsolute();
	motion.setRelative();
	motion.goAsync(target.toVec2());
	system.waitUntil(motion.getCurrentJob());

	if(wasAbsolute) motion.setAbsolute();
}


void  SystemApplication::probeBorder(TableCompass tc, RobotCompass rc){
 	boolean wasAbsolute = motion.isAbsolute();
	_probing = true;

	motion.setRelative();
	align(rc, getCompassOrientation(tc));

	motion.steppers.setFeedrate(SLOW);
	goPolar(getCompassOrientation(rc),200);
	motion.steppers.setFeedrate(10);
	goPolar(getCompassOrientation(rc),80);

	float _offset = getOffsets(rc);

	Vec3 position = motion.getAbsPosition();

	if(tc == TableCompass::NORTH){
		position.a = 3000.0 - _offset; //We hit Xmax
		_probedX = true;
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::SOUTH){
		position.a = 0.0 + _offset; //We hit Xmin
		_probedX = true;
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::EAST){
		position.b = 2000.0 - _offset; //We hit Ymax
		_probedY = true;
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::WEST){
		position.b = 0.0 + _offset; //We hit Ymin
		_probedY = true;
		motion.setAbsPosition(position);
	}

	motion.setAbsPosition(position);

	goPolar(getCompassOrientation(rc),-100);

	motion.steppers.setFeedrate(FAST);

	if(wasAbsolute) motion.setAbsolute();
	_probing = false;
}

void SystemApplication::probeObstacle(Vec2 obstaclePosition,TableCompass tc, RobotCompass rc){

    boolean wasAbsolute = motion.isAbsolute();
	_probing = true;

	motion.setRelative();
	align(rc, getCompassOrientation(tc));

	motion.steppers.setFeedrate(10);
	goPolar(getCompassOrientation(rc),80);

	float _offset = getOffsets(rc);

	Vec3 position = motion.getAbsPosition();

	if(tc == TableCompass::NORTH){
		position.a = obstaclePosition.a - _offset; //We hit Xmax
		_probedX = true;
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::SOUTH){
		position.a = obstaclePosition.a + _offset; //We hit Xmin
		_probedX = true;
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::EAST){
		position.b = obstaclePosition.a - _offset; //We hit Ymax
		_probedY = true;
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::WEST){
		position.b = obstaclePosition.a + _offset; //We hit Ymin
		_probedY = true;
		motion.setAbsPosition(position);
	}

	motion.setAbsPosition(position);

	motion.steppers.setFeedrate(FAST);

	if(wasAbsolute) motion.setAbsolute();
	_probing = false;
}

bool  SystemApplication::isProbed(){
	return _probedX && _probedY;
}
bool  SystemApplication::isXProbed(){
	return _probedX;
}
bool  SystemApplication::isYProbed(){
	return _probedY;
}
bool  SystemApplication::isProbing(){
	return _probing;
}



void SystemApplication::testSteppers(){
	motion.setAbsPosition(Vec3(0,0,0));
	motion.setAbsolute();
	go(1000,1000);
}

void SystemApplication::testOrientation(){
	motion.setAbsPosition({0,0,0});
	motion.setAbsolute();
	align(RobotCompass::A, getCompassOrientation(TableCompass::EAST));
	align(RobotCompass::AB, getCompassOrientation(TableCompass::EAST));
}


void SystemApplication::testDetection(){
	motion.setAbsPosition({10,10,0});
	motion.setAbsolute();

	lidar.getMaxLidarDist(Vec2(motion.getAbsPosition().a, motion.getAbsPosition().b), 0);

	lidar.getMaxLidarDist(Vec2(motion.getAbsPosition().a, motion.getAbsPosition().b), 90*DEG_TO_RAD);
	
	lidar.getMaxLidarDist(Vec2(motion.getAbsPosition().a, motion.getAbsPosition().b), 270*DEG_TO_RAD);
	
	lidar.getMaxLidarDist(Vec2(motion.getAbsPosition().a, motion.getAbsPosition().b), -90*DEG_TO_RAD);
	
}


void SystemApplication::testMotion(){
	motion.setAbsPosition({0,0,0});
	motion.setAbsolute();

	lidar.disable();

}



void SystemApplication::recalagePrimaryBlue(){
    motion.steppers.engage();

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,0));
    probeBorder(TableCompass::SOUTH, RobotCompass::BC);
    probeBorder(TableCompass::EAST, RobotCompass::CA);
	motion.setAbsolute();
	align(RobotCompass::AB, getCompassOrientation(TableCompass::NORTH));

    actuators.ungrab(RobotCompass::AB);

    motion.steppers.sleep();
}

void SystemApplication::recalagePrimaryGreen(){
    motion.steppers.engage();

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,0));
    probeBorder(TableCompass::SOUTH, RobotCompass::BC);
    probeBorder(TableCompass::WEST, RobotCompass::AB);
	motion.setAbsolute();
	align(RobotCompass::AB, getCompassOrientation(TableCompass::NORTH));

    actuators.ungrab(RobotCompass::AB);

    motion.steppers.sleep();
}

void SystemApplication::recalageSecondaryBlue(){
    Console::info("Robot") << "Recalage Secondary Blue Cherry" << Console::endl;
    motion.steppers.engage();

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,PI));
    probeBorder(TableCompass::NORTH, RobotCompass::BC);
    probeBorder(TableCompass::EAST, RobotCompass::AB);
	motion.setAbsolute();
    go(b3);
	align(RobotCompass::A, getCompassOrientation(TableCompass::WEST));

    motion.steppers.sleep();
}

void SystemApplication::recalageSecondaryGreen(){
    Console::info("Robot") << "Recalage Secondary Green Cherry" << Console::endl;
    motion.steppers.engage();

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,PI));
    probeBorder(TableCompass::NORTH, RobotCompass::BC);
    probeBorder(TableCompass::WEST, RobotCompass::CA);
	motion.setAbsolute();
    go(v3);
	align(RobotCompass::A, getCompassOrientation(TableCompass::EAST));

    motion.steppers.sleep();
}

void SystemApplication::recalageSecondaryCakeBlue(){
    Console::info("Robot") << "Recalage Secondary Blue Cake" << Console::endl;
    motion.steppers.engage();

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,PI));
    probeBorder(TableCompass::NORTH, RobotCompass::BC);
    probeBorder(TableCompass::WEST, RobotCompass::AB);
	motion.setAbsolute();
    go(b4);
	align(RobotCompass::AB, getCompassOrientation(TableCompass::SOUTH));

    actuators.ungrab(RobotCompass::AB);

    motion.steppers.sleep();
}

void SystemApplication::recalageSecondaryCakeGreen(){
    Console::info("Robot") << "Recalage Secondary Green Cake " << Console::endl;
    motion.steppers.engage();

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,PI));
    probeBorder(TableCompass::NORTH, RobotCompass::BC);
    probeBorder(TableCompass::EAST, RobotCompass::AB);
	motion.setAbsolute();
    go(v4);
	align(RobotCompass::AB, getCompassOrientation(TableCompass::SOUTH));

    actuators.ungrab(RobotCompass::AB);

    motion.steppers.sleep();
}

void SystemApplication::matchPrimaryBlue(){
    motion.steppers.engage();
	motion.setAbsolute();

    addScore(basket);

    actuators.ungrab(RobotCompass::AB);
	actuators.ungrab(RobotCompass::BC);
	actuators.ungrab(RobotCompass::CA);

    actuators.ungrab(RobotCompass::AB);
	go(cakePinkSE);
	actuators.grab(RobotCompass::AB);
	align(RobotCompass::BC, getCompassOrientation(TableCompass::NORTH));

    actuators.ungrab(RobotCompass::BC);
    go(cakeYellowSE);
    actuators.grab(RobotCompass::BC);
    align(RobotCompass::CA, 45);

    actuators.ungrab(RobotCompass::CA);
    go(cakeBrownSE);
    actuators.grab(RobotCompass::CA);

    //----ATTENTION----
    system.disable(LIDAR);
    //Dépose du premier Gateau
    align(RobotCompass::BC, -120);
    go(retreatBlue1);
    actuators.unlock(RobotCompass::BC);
    go(dropBlue1);
    actuators.ungrab(RobotCompass::BC);
    go(retreatBlue2);
    actuators.close(RobotCompass::BC);
    addScore(cakeWithCherry);

    //Dépose du second Gateau
    align(RobotCompass::AB, -120);
    actuators.unlock(RobotCompass::AB);
    go(dropBlue2);
    actuators.ungrab(RobotCompass::AB);
    go(retreatBlue3);
    actuators.close(RobotCompass::AB);
    addScore(cakeWithCherry);

    //Dépose du troisieme Gateau
    align(RobotCompass::CA, -120);
    actuators.unlock(RobotCompass::CA);
    go(dropBlue3);
    actuators.ungrab(RobotCompass::CA);
    go(retreatBlue3);
    actuators.close(RobotCompass::CA);
    addScore(cakeWithCherry);
    
    //recalage
    go(retreatBlue1);
    probeBorder(TableCompass::SOUTH, RobotCompass::C);
    motion.setAbsolute();
    //Va chercher les balles
    align(RobotCompass::A, getCompassOrientation(TableCompass::WEST));
    actuators.suckBall();
    go(ballBeginSE);
    go(ballEndSE);
    actuators.stopTurbine();
    // Va en dépose des balles
    go(ballBlueBasket);
    align(RobotCompass::A, getCompassOrientation(TableCompass::SOUTH));
    go(ballBlueBasketPlus);
    // Dépose
    actuators.trap.open();
    system.wait(2000);
    actuators.trap.close();
    addScore(exactCherryCounting);
    //recalage
    probeBorder(TableCompass::SOUTH, RobotCompass::A);
    motion.setAbsolute();

    //----ATTENTION----
    system.enable(LIDAR);

    // Fin de match
    go(blueEndPrimary);
    addScore(wheelsOnPlate/2);
	system.enable(NEOPIXEL);

    // Fin de match
    actuators.disengage();
	motion.steppers.disengage();

}

void SystemApplication::matchPrimaryGreen(){
	motion.steppers.engage();
	motion.setAbsolute();

    addScore(basket);

    actuators.ungrab(RobotCompass::AB);
    actuators.ungrab(RobotCompass::BC);
    actuators.ungrab(RobotCompass::CA);

    actuators.ungrab(RobotCompass::AB);
    go(cakePinkSW);
    actuators.grab(RobotCompass::AB);
    align(RobotCompass::BC, getCompassOrientation(TableCompass::NORTH));

    actuators.ungrab(RobotCompass::BC);
    go(cakeYellowSW);
    actuators.grab(RobotCompass::BC);
    align(RobotCompass::CA, -45);

    actuators.ungrab(RobotCompass::CA);
    go(cakeBrownSW);
    actuators.grab(RobotCompass::CA);

    //----ATTENTION----
    system.disable(LIDAR);

    //Dépose du premier Gateau
    align(RobotCompass::BC, 120);
    go(retreatGreen1);
    actuators.unlock(RobotCompass::BC);
    go(dropGreen1);
    actuators.ungrab(RobotCompass::BC);
    go(retreatGreen2);
    actuators.close(RobotCompass::BC);
    addScore(cakeWithCherry);

    //Dépose du second Gateau
    align(RobotCompass::AB, 120);
    actuators.unlock(RobotCompass::AB);
    go(dropGreen2);
    actuators.ungrab(RobotCompass::AB);
    go(retreatGreen3);
    actuators.close(RobotCompass::AB);
    addScore(cakeWithCherry);

    //Dépose du troisieme Gateau
    align(RobotCompass::CA, 120);
    actuators.unlock(RobotCompass::CA);
    go(dropGreen3);
    actuators.ungrab(RobotCompass::CA);
    go(retreatGreen3);
    actuators.close(RobotCompass::CA);
    addScore(cakeWithCherry);
    
    //recalage
    go(retreatGreen1);
    probeBorder(TableCompass::SOUTH, RobotCompass::C);
    motion.setAbsolute(); // Todo : utile ? Déja fait dans probeBorder ?
    //Va chercher les balles
    actuators.suckBall();
    align(RobotCompass::A, getCompassOrientation(TableCompass::EAST));
    // Todo Probe Ball support
    go(ballEndSW);
    probeObstacle(Vec2(200,990),TableCompass::EAST,RobotCompass::A); // Todo : POI pour probe obstacle
    go(ballEndSW);
    go(ballBeginSW);
    actuators.stopTurbine();
    // Va en dépose des balles
    go(ballGreenBasket);
    align(RobotCompass::A, getCompassOrientation(TableCompass::SOUTH));
    go(ballGreenBasketPlus);
    // Dépose
    actuators.trap.open();
    system.wait(2000);
    actuators.trap.close();
    addScore(exactCherryCounting);
    //recalage
    probeBorder(TableCompass::SOUTH, RobotCompass::A);
    motion.setAbsolute();

    //----ATTENTION----
    system.enable(LIDAR);

    // Fin de match
    go(greenEndPrimary);
    addScore(wheelsOnPlate/2);
    system.enable(NEOPIXEL);

    // Fin de match
    actuators.disengage();
	motion.steppers.disengage();

}

void SystemApplication::matchSecondaryBlue(){
    motion.steppers.engage();
	motion.setAbsolute();

    //----ATTENTION----
    system.disable(LIDAR);

    // Va chercher les balles
    // Lance la turbine
    actuators.suckBall();
    // Prendre les balles
    align(RobotCompass::A, getCompassOrientation(TableCompass::WEST));
    go(ballEndNE);
    go(ballBeginNE);
    // Stop la turbine
    actuators.stopTurbine();

    // Ouvre tous les actionneurs pour gagner du temps
    actuators.ungrab(RobotCompass::AB);
    actuators.ungrab(RobotCompass::BC);
    actuators.ungrab(RobotCompass::CA);
    // Postionnement et orientation "comme en vert"
    go(v4);

    //----ATTENTION----
    system.enable(LIDAR);

    align(RobotCompass::AB, getCompassOrientation(TableCompass::SOUTH));
    // Va chercher les gateaux
    actuators.ungrab(RobotCompass::AB);
    go(cakePinkNE);
    actuators.grab(RobotCompass::AB);
    align(RobotCompass::BC, getCompassOrientation(TableCompass::SOUTH));

    actuators.ungrab(RobotCompass::BC);
    go(cakeYellowNE);
    actuators.grab(RobotCompass::BC);
    align(RobotCompass::CA, 135);

    actuators.ungrab(RobotCompass::CA);
    go(cakeBrownNE);
    actuators.grab(RobotCompass::CA);
    
    // Depose les gateaux
    align(RobotCompass::AB, getCompassOrientation(TableCompass::EAST));
    actuators.unlock(RobotCompass::AB);
    go(dropB2_01);
    actuators.ungrab(RobotCompass::AB);
    addScore(cakeWithCherry);

    actuators.unlock(RobotCompass::BC);
    actuators.unlock(RobotCompass::CA);

    go(dropB2_02);
    align(RobotCompass::BC, getCompassOrientation(TableCompass::EAST));
    actuators.ungrab(RobotCompass::BC);
    addScore(cakeWithCherry);

    go(dropB2_03);
    align(RobotCompass::CA, getCompassOrientation(TableCompass::EAST));
    actuators.ungrab(RobotCompass::CA);
    addScore(cakeWithCherry);

    go(cakeBrownNE);
    //Fin du match
    go(blueEndSecondary);

    addScore(wheelsOnPlate/2);

    // Fin de match
    actuators.trap.open();
    system.wait(1000);
    actuators.disengage();
    motion.steppers.disengage();
}

void SystemApplication::matchSecondaryGreen(){
    motion.steppers.engage();
	motion.setAbsolute();

    //----ATTENTION----
    system.disable(LIDAR);

    // Va chercher les balles
    // Lance la turbine
    actuators.suckBall();
    // Prendre les balles
    align(RobotCompass::A, getCompassOrientation(TableCompass::EAST));
    go(ballEndNW);
    go(ballBeginNW);
    //Stop Turbine
    actuators.stopTurbine();

    // Ouvre tous les actionneurs pour gagner du temps
    actuators.ungrab(RobotCompass::AB);
    actuators.ungrab(RobotCompass::BC);
    actuators.ungrab(RobotCompass::CA);
    // Postionnement et orientation "comme en vert"
    go(b4);

    //----ATTENTION----
    system.enable(LIDAR);

    align(RobotCompass::AB, getCompassOrientation(TableCompass::SOUTH));
    // Va chercher les gateaux
    actuators.ungrab(RobotCompass::AB);
    go(cakePinkNW);
    actuators.grab(RobotCompass::AB);
    align(RobotCompass::BC, getCompassOrientation(TableCompass::SOUTH));

    actuators.ungrab(RobotCompass::BC);
    go(cakeYellowNW);
    actuators.grab(RobotCompass::BC);
    align(RobotCompass::CA, -135);

    actuators.ungrab(RobotCompass::CA);
    go(cakeBrownNW);
    actuators.grab(RobotCompass::CA);
    
    // Depose les gateaux
    align(RobotCompass::AB, getCompassOrientation(TableCompass::WEST));
    actuators.unlock(RobotCompass::AB);
    go(dropV2_01);
    actuators.ungrab(RobotCompass::AB);
    addScore(cakeWithCherry);

    actuators.unlock(RobotCompass::BC);
    actuators.unlock(RobotCompass::CA);

    go(dropV2_02);
    align(RobotCompass::BC, getCompassOrientation(TableCompass::WEST));
    actuators.ungrab(RobotCompass::BC);
    addScore(cakeWithCherry);

    go(dropV2_03);
    align(RobotCompass::CA, getCompassOrientation(TableCompass::WEST));
    actuators.ungrab(RobotCompass::CA);
    addScore(cakeWithCherry);

    go(cakeBrownNW);
    //Fin du match
    go(greenEndSecondary);

    addScore(wheelsOnPlate/2);

    // Fin de match
    actuators.trap.open();
    system.wait(1000);
    actuators.disengage();
    motion.steppers.disengage();
}

void SystemApplication::matchSecondaryCakeBlue(){
    motion.steppers.engage();
	motion.setAbsolute();

    actuators.ungrab(RobotCompass::AB);
	actuators.ungrab(RobotCompass::BC);
	actuators.ungrab(RobotCompass::CA);

    actuators.ungrab(RobotCompass::AB);
	go(cakePinkNW);
	actuators.grab(RobotCompass::AB);
	align(RobotCompass::BC, getCompassOrientation(TableCompass::SOUTH));

    actuators.ungrab(RobotCompass::BC);
    go(cakeYellowNW);
    actuators.grab(RobotCompass::BC);
    align(RobotCompass::CA, -135);

    actuators.ungrab(RobotCompass::CA);
    go(cakeBrownNW);
    actuators.grab(RobotCompass::CA);

    // Depose les gateaux
    align(RobotCompass::AB, getCompassOrientation(TableCompass::EAST));
    actuators.unlock(RobotCompass::AB);
    go(dropB2_01);
    actuators.ungrab(RobotCompass::AB);
    addScore(cakeWithCherry);

    actuators.unlock(RobotCompass::BC);
    actuators.unlock(RobotCompass::CA);

    go(dropB2_02);
    align(RobotCompass::BC, getCompassOrientation(TableCompass::EAST));
    actuators.ungrab(RobotCompass::BC);
    addScore(cakeWithCherry);

    go(dropB2_03);
    align(RobotCompass::CA, getCompassOrientation(TableCompass::EAST));
    actuators.ungrab(RobotCompass::CA);
    addScore(cakeWithCherry);

    go(cakeBrownNE);

    //Fin du match
    go(blueEndSecondary);

    addScore(wheelsOnPlate/2);

    // Fin de match
    actuators.trap.open();
    system.wait(1000);
    actuators.disengage();
    motion.steppers.disengage();
}

void SystemApplication::matchSecondaryCakeGreen(){
    motion.steppers.engage();
	motion.setAbsolute();

    actuators.ungrab(RobotCompass::AB);
	actuators.ungrab(RobotCompass::BC);
	actuators.ungrab(RobotCompass::CA);

    actuators.ungrab(RobotCompass::AB);
	go(cakePinkNE);
	actuators.grab(RobotCompass::AB);
	align(RobotCompass::BC, getCompassOrientation(TableCompass::SOUTH));

    actuators.ungrab(RobotCompass::BC);
    go(cakeYellowNE);
    actuators.grab(RobotCompass::BC);
    align(RobotCompass::CA, 135);

    actuators.ungrab(RobotCompass::CA);
    go(cakeBrownNE);
    actuators.grab(RobotCompass::CA);

    // Depose les gateaux
    align(RobotCompass::AB, getCompassOrientation(TableCompass::WEST));
    actuators.unlock(RobotCompass::AB);
    go(dropV2_01);
    actuators.ungrab(RobotCompass::AB);
    addScore(cakeWithCherry);

    actuators.unlock(RobotCompass::BC);
    actuators.unlock(RobotCompass::CA);

    go(dropV2_02);
    align(RobotCompass::BC, getCompassOrientation(TableCompass::WEST));
    actuators.ungrab(RobotCompass::BC);
    addScore(cakeWithCherry);

    go(dropV2_03);
    align(RobotCompass::CA, getCompassOrientation(TableCompass::WEST));
    actuators.ungrab(RobotCompass::CA);
    addScore(cakeWithCherry);

    go(cakeBrownNW);

    //Fin du match
    go(greenEndSecondary);

    addScore(wheelsOnPlate/2);

    // Fin de match
    actuators.trap.open();
    system.wait(1000);
    actuators.disengage();
    motion.steppers.disengage();
}


void SystemApplication::nearlyFinishPrimaryBlue(){
    // go to End Position
    go(blueEndPrimary);
    addScore(wheelsOnPlate/2);
}

void SystemApplication::nearlyFinishPrimaryGreen(){
    // go to End Position
    go(greenEndPrimary);
    addScore(wheelsOnPlate/2);
}

void SystemApplication::nearlyFinishSecondaryBlue(){
    // go to End Position
    go(blueEndSecondary);
    addScore(wheelsOnPlate/2);
}

void SystemApplication::nearlyFinishSecondaryGreen(){
    // go to End Position
    go(greenEndSecondary);
    addScore(wheelsOnPlate/2);
}

void SystemApplication::finishPrimaryBlue(){
    // Fin de match
    system.enable(NEOPIXEL);
    actuators.disengage();
    motion.steppers.disengage();
}

void SystemApplication::finishPrimaryGreen(){
    // Fin de match
    system.enable(NEOPIXEL);
    actuators.disengage();
    motion.steppers.disengage();
}

void SystemApplication::finishSecondaryBlue(){
    // Fin de match
    actuators.disengage();
    motion.steppers.disengage();
}

void SystemApplication::finishSecondaryGreen(){
    // Fin de match
    actuators.disengage();
    motion.steppers.disengage();
}

