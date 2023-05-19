#include "app.h"
#include "match/poi.h"
#include "match/score.h"

#define lidar (*_lidarPtr)
#define screen (*_screenPtr)
#define inputs (*_inputsPtr)
#define motion (*_motionPtr)
#define planner (*_plannerPtr)
#define neopixel (*_neopixelPtr)
//#define terminal (*_terminalPtr)
#define actuators (*_actuatorsPtr)
#define localisation (*_localisationPtr)

using namespace POI;
using namespace Score;

void SystemApplication::addScore(int points, int multiplicateur){
    _score += (points * multiplicateur);
}

void SystemApplication::optimizeScore(float k){
    _score *= k;
}

int SystemApplication::score(){
    return _score;
}

void SystemApplication::connectModules(){
    //Tracked values
	//TODO Create events to handle this at the UI Level

    screen.x.SetValue(motion.getAbsPosition().x);
    screen.y.SetValue(motion.getAbsPosition().y);
    screen.z.SetValue(motion.getAbsPosition().z);
    screen.score.SetValue(_score);
    screen.time.SetValue(chrono.getTimeLeftSeconds());
    screen.intercom.SetValue(lidar.isConnected());


    if(_state == RobotState::IDLE){

        //localisation.addMeasure(Vec3(inputs.getDistanceSensorA(), inputs.getDistanceSensorB(), inputs.getDistanceSensorC()));

        
        //if(millis() - _lastDrift > 200){
            //_lastDrift = millis();
            //localisation.estimateDrift(motion.getAbsPosition());
        //}

        //if(terminal.commandAvailable())
        //    processCommand(terminal.dequeCommand());

        screen.probing.SetValue(_probing);
        screen.probed.SetValue(_probed);
        screen.armed.SetValue(_state == RobotState::ARMED);
        screen.started.SetValue(_state == RobotState::STARTED);
        screen.starter.SetValue(inputs.hasStarter());
        screen.teamSwitch.SetValue(inputs.isGreen());
        screen.twinSwitch.SetValue(inputs.isPrimary());
        screen.resetButton.SetValue(inputs.buttonPressed());
        screen.strategySwitch.SetValue(inputs.isCake());

        if(inputs.getDownTrapState() && !actuators.trap.isClosed())
            actuators.trap.close();
        else if(inputs.getUpTrapState() && actuators.trap.isClosed())
            actuators.trap.open();
        
        if(inputs.getLowTurbineState() && inputs.hasTurbineStateChanged())
            actuators.setTurbine(50);
        else if(inputs.getHighTurbineState() && inputs.hasTurbineStateChanged())
            actuators.setTurbine(100);
        else if((!inputs.getLowTurbineState() || !inputs.getHighTurbineState()) && inputs.hasTurbineStateChanged())
            actuators.stopTurbine();
    }


    

	//screen.started.SetValue(_state == RobotState::STARTED);
	//screen.intercom.SetValue(lidar.isConnected());
	//screen.time.SetValue(score());
	//screen.score.SetValue(chrono.getTimeLeftSeconds());

}

/*
void SystemApplication::processCommand(Command c){  

    if(!terminal.isEnabled())return;
    if(c.isValidFormat("go(x,y)")){
        go(c.getVec2());
    }else if(c.isValidFormat("turn(angle)")){
        turn(c.getFloat());
    }else if(c.isValidFormat("align(side, absAngle)")){

        String side = c.getString();
        float orientation = c.getFloat(1);

        if(side.equals("A"))         align(RobotCompass::A, orientation);
        else if(side.equals("AB"))   align(RobotCompass::AB, orientation);
        else if(side.equals("B"))    align(RobotCompass::B, orientation);
        else if(side.equals("BC"))   align(RobotCompass::BC, orientation);
        else if(side.equals("C"))    align(RobotCompass::C, orientation);
        else if(side.equals("CA"))   align(RobotCompass::CA, orientation);

    }else if(c.isValidFormat("goPolar(heading, lenght)")){
        goPolar(c.getFloat(0), c.getFloat(1));
    }else if(c.isValidFormat("close(side)")){
        String side = c.getString();
        if(side.equals("AB")) actuators.close(RobotCompass::AB);
        else if(side.equals("BC")) actuators.close(RobotCompass::BC);
        else if(side.equals("CA")) actuators.close(RobotCompass::CA);
    }else if(c.isValidFormat("open(side)")){
        String side = c.getString();
        THROW(side)
        if(side.equals("AB")) actuators.open(RobotCompass::AB);
        else if(side.equals("BC")) actuators.open(RobotCompass::BC);
        else if(side.equals("CA")) actuators.open(RobotCompass::CA);
    }else if(c.isValidFormat("grab(side)")){
        String side = c.getString();
        if(side.equals("AB")) actuators.grab(RobotCompass::AB);
        else if(side.equals("BC")) actuators.grab(RobotCompass::BC);
        else if(side.equals("CA")) actuators.grab(RobotCompass::CA);
    }else if(c.isValidFormat("ungrab(side)")){
        String side = c.getString();
        if(side.equals("AB")) actuators.ungrab(RobotCompass::AB);
        else if(side.equals("BC")) actuators.ungrab(RobotCompass::BC);
        else if(side.equals("CA")) actuators.ungrab(RobotCompass::CA);
    }else if(c.isValidFormat("openTrap()")){
        actuators.openTrap();
    }else if(c.isValidFormat("closeTrap()")){
        actuators.closeTrap();
    }else if(c.isValidFormat("recalage()")){
        handleRecalage();
        return;
    }else if(c.isValidFormat("start()")){
        _state = RobotState::STARTING;
    }else if(c.isValidFormat("pause()")){
        motion.pause();
    }else if(c.isValidFormat("resume()")){
        motion.resume();
    }else if(c.isValidFormat("cancel()")){
        motion.cancel();
    }else if(c.isValidFormat("setRelative()")){
        motion.setRelative();
    }else if(c.isValidFormat("setAbsolute()")){
        motion.setAbsolute();
    }else if(c.isValidFormat("testMotion()")){
        testMotion();
    }else if(c.isValidFormat("testSteppers()")){
        testSteppers();
    }else if(c.isValidFormat("testDetection()")){
        testDetection();
    }else if(c.isValidFormat("testOrientation()")){
        testOrientation();
    }else if(c.isValidFormat("enableMotion()")){
        system.enable(MOTION);
    }else if(c.isValidFormat("disableMotion()")){
        system.disable(MOTION);
    }else if(c.isValidFormat("setAbsolute()")){
        motion.setAbsolute();
    }else if(c.isValidFormat("setRelative()")){
        motion.setRelative();
    }else{
        Console::error("Terminal") << "Unknown command" << Console::endl;
    }
}
*/

SystemApplication::SystemApplication(){
    _state = RobotState::IDLE;
    _score = 0;

    //Standalone modules
    _screenPtr = std::make_unique<Screen>();

    screen.drawBootProgress(10, "Loading Actuators...");
    _actuatorsPtr = std::make_unique<Actuators>();

    screen.drawBootProgress(20, "Loading Lidar...");
    _lidarPtr = std::make_unique<Lidar>();

    screen.drawBootProgress(30, "Loading Inputs...");
    _inputsPtr = std::make_unique<Inputs>();

    screen.drawBootProgress(40, "Loading Motion...");
    _motionPtr = std::make_unique<Motion>();

    //screen.drawBootProgress(50, "Loading Planner...");
    //_plannerPtr = std::make_unique<Planner>();

    screen.drawBootProgress(60, "Loading Neopixel...");
    _neopixelPtr = std::make_unique<NeoPixel>();

    //screen.drawBootProgress(70, "Loading Terminal...");
    //_terminalPtr = std::make_unique<Terminal>();

    //screen.drawBootProgress(80, "Loading Localisation...");
    //_localisationPtr = std::make_unique<Localisation>();

    screen.drawBootProgress(90, "Linking modules...");

    system.registerModule(_lidarPtr.get());
    system.registerModule(_screenPtr.get());
    system.registerModule(_inputsPtr.get());
    system.registerModule(_motionPtr.get());
    //system.registerModule(_plannerPtr.get());
    system.registerModule(_neopixelPtr.get());
    //system.registerModule(_terminalPtr.get());
    system.registerModule(_actuatorsPtr.get());
    //system.registerModule(_localisationPtr.get());

    system.enable(LIDAR);
    system.enable(INPUTS);
    system.enable(SCREEN);
	//system.enable(MOTION);
    system.enable(NEOPIXEL);
    //system.enable(TERMINAL);
	system.enable(ACTUATORS);
    //system.enable(LOCALISATION);

    screen.drawBootProgress(100, "Boot complete...");
    delay(200);
    screen.setPage(Page::INIT);
}

SystemApplication::~SystemApplication(){}

void SystemApplication::update(){
	system.update();
	chrono.getTimeLeft();
    connectModules();

	if(_state == RobotState::STARTED || _state == RobotState::FINISHING){

		if(chrono.isFinished()){
			handleFinishedMatch(); //Stop robot, motor disengage
		}else if(chrono.isNearlyFinished()){
			handleNearlyFinishedMatch();//go home
		}

		lidar.checkLidar(motion.getTargetDirection() * RAD_TO_DEG);
		lidar.checkObstacle();

		if(lidar.obstacleDetected() && motion.isMoving() && !motion.isRotating()){
			motion.pause();
            THROW("PAUSED")
		}else if(motion.getCurrentJob().isPaused() && !lidar.obstacleDetected()){
		    motion.resume();
            THROW("RESUMED")
		}
	}
}

void SystemApplication::waitLaunch(){
	while (_state != RobotState::STARTING){
		update();
		switch (_state){
		case RobotState::IDLE :
			if(inputs.starterPlaced()){ //Arm
				setState(RobotState::ARMED);
                system.disable(NEOPIXEL);

				inputs.freezeSettings();
				if(lidar.isConnected()){
					lidar.displayRadar(true);
				}
			}
			if(inputs.buttonReleased()){ //Recalage
                system.enable(MOTION);
                lidar.ignoreObstacles(true);
                
                //testMotion(); break;              
                
                handleRecalage();

				//TestSteppers();
                lidar.ignoreObstacles(false);
			}
			break;

		case RobotState::ARMED :
			if(inputs.starterPulled()){ //Start match
				setState(RobotState::STARTING);
			}else if(inputs.starterCancelled()){ //Unarm
				setState(RobotState::IDLE);
				inputs.unfreezeSettings();
				system.enable(NEOPIXEL);
				lidar.displayRadar(false);
				while(inputs.buttonPressed()) update(); //Wait for resetButton to be released
			}
			break;
	
		default:
			break;
		}

		//delay(10);
	};
}

void SystemApplication::startMatch(){
    system.disable(TERMINAL);
	chrono.start();
    system.enable(MOTION);

    system.disable(INPUTS);
    system.disable(NEOPIXEL);
    

    lidar.displayRadar(true);

	screen.setPage(Page::MATCH);
	//actuators.Engage();
	setState(RobotState::STARTED);

	//testMotion(); handleFinishedMatch();
	//TestSteppers(); motion.Disengage(); return;
	//TestDetection(); motion.Disengage(); return;

	if	   (inputs.isBlue()  && inputs.isPrimary())                         matchPrimaryBlue();
	else if(inputs.isBlue()  && inputs.isSecondary() && inputs.isCherry())  matchSecondaryBlue();
	else if(inputs.isBlue()  && inputs.isSecondary() && inputs.isCake())    matchSecondaryCakeBlue();

	else if(inputs.isGreen() && inputs.isPrimary())                         matchPrimaryGreen();
	else if(inputs.isGreen() && inputs.isSecondary() && inputs.isCherry())  matchSecondaryGreen();
	else if(inputs.isGreen() && inputs.isSecondary() && inputs.isCake())    matchSecondaryCakeGreen();

    handleFinishedMatch();
}

void SystemApplication::endMatch(){
    optimizeScore(0.8);
    screen.score.SetValue(_score);
    motion.cancel();
    system.disable(LIDAR);
    system.disable(MOTION);

    system.disable(ACTUATORS);
    system.disable(INPUTS);


    Console::println("Fin du programme");
    while(true){
        screen.update();
    }
    //system.disable(SCREEN);
}


void SystemApplication::handleRecalage(){
        if	   (inputs.isBlue()  && inputs.isPrimary() )                    recalagePrimaryBlue();
    else if(inputs.isBlue()  && inputs.isSecondary() && inputs.isCherry())  recalageSecondaryBlue();
    else if(inputs.isBlue()  && inputs.isSecondary() && inputs.isCake()) 	recalageSecondaryCakeBlue();
    else if(inputs.isGreen() && inputs.isPrimary() ) 					    recalagePrimaryGreen();
    else if(inputs.isGreen() && inputs.isSecondary() && inputs.isCherry()) 	recalageSecondaryGreen();
    else if(inputs.isGreen() && inputs.isSecondary() && inputs.isCake()) 	recalageSecondaryCakeGreen();
}

void SystemApplication::handleFinishedMatch(){ 
    if(_state != RobotState::FINISHED){
        setState(RobotState::FINISHED);
        motion.forceCancel();
        system.disable(MOTION);

        if	   (inputs.isBlue()  && inputs.isPrimary())     finishPrimaryBlue();
        else if(inputs.isBlue()  && inputs.isSecondary())   finishSecondaryBlue();
        else if(inputs.isGreen() && inputs.isPrimary())     finishPrimaryGreen();
        else if(inputs.isGreen() && inputs.isSecondary())   finishSecondaryGreen();

        system.disable(MOTION);
        system.disable(ACTUATORS);
        system.enable(NEOPIXEL);

        screen.update();
        endMatch();
    }

}

void SystemApplication::handleNearlyFinishedMatch(){
	if(_state == RobotState::STARTED){
		setState(RobotState::FINISHING);
		motion.cancel();
		if	   (inputs.isBlue()  && inputs.isPrimary())     nearlyFinishPrimaryBlue();
		else if(inputs.isBlue()  && inputs.isSecondary())   nearlyFinishSecondaryBlue();
        else if(inputs.isGreen() && inputs.isPrimary())     nearlyFinishPrimaryGreen();
		else if(inputs.isGreen() && inputs.isSecondary())   nearlyFinishSecondaryGreen();
	}
}



void SystemApplication::wait(unsigned long temps){
	unsigned long initTemps = millis();
	while ((millis() - initTemps) <= temps){
		update();
	}
}

void SystemApplication::waitUntil(Job& obj){
    THROW("WaitUntil Job finished or cancelled")
	while (obj.isPending() || obj.isPaused()){
        //THROW(obj.toString())		
		update();
	}
    THROW(obj.toString())
}


void SystemApplication::go(Vec2 v){
	motion.goAsync(v);
	waitUntil(motion.getCurrentJob());
}

void SystemApplication::go(float x, float y){
	motion.goAsync(x, y);
	waitUntil(motion.getCurrentJob());
}

void SystemApplication::goAlign(Vec2 target, RobotCompass rc, float orientation){
    motion.goAlignAsync(target, rc, orientation);
	waitUntil(motion.getCurrentJob());
}

void SystemApplication::turn(float a){
	motion.turnAsync(a);
	waitUntil(motion.getCurrentJob());
}

void SystemApplication::align(RobotCompass rc, float orientation){
 	bool wasRelative = motion.isRelative();
	motion.setAbsolute();
	motion.alignAsync(rc, orientation);
	waitUntil(motion.getCurrentJob());
	if(wasRelative) motion.setRelative();
}

void SystemApplication::goPolar(float heading, float length){
	PolarVec target = PolarVec(heading*DEG_TO_RAD, length);
	bool wasAbsolute = motion.isAbsolute();
	motion.setRelative();
	motion.goAsync(target.toVec2());
	waitUntil(motion.getCurrentJob());

	if(wasAbsolute) motion.setAbsolute();
}


void  SystemApplication::probeBorder(TableCompass tc, RobotCompass rc){
    _probing = true;
 	boolean wasAbsolute = motion.isAbsolute();

	motion.setRelative();
	align(rc, getCompassOrientation(tc));

	motion.setFeedrate(5);
	goPolar(getCompassOrientation(rc),200);
	motion.setFeedrate(2);
	goPolar(getCompassOrientation(rc),80);

	float _offset = getOffsets(rc);

	Vec3 position = motion.getAbsPosition();

	if(tc == TableCompass::NORTH){
		position.a = 3000.0 - _offset; //We hit Xmax
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::SOUTH){
		position.a = 0.0 + _offset; //We hit Xmin
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::EAST){
		position.b = 2000.0 - _offset; //We hit Ymax
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::WEST){
		position.b = 0.0 + _offset; //We hit Ymin
		motion.setAbsPosition(position);
	}

	motion.setAbsPosition(position);

	goPolar(getCompassOrientation(rc),-100);

	motion.setFeedrate(FAST);

	if(wasAbsolute) motion.setAbsolute();
    _probing = true;
    _probing = false;
}

void SystemApplication::probeObstacle(Vec2 obstaclePosition,TableCompass tc, RobotCompass rc){
    boolean wasAbsolute = motion.isAbsolute();

	motion.setRelative();
	align(rc, getCompassOrientation(tc));

	motion.setFeedrate(5);
	goPolar(getCompassOrientation(rc),80);

	float _offset = getOffsets(rc);

	Vec3 position = motion.getAbsPosition();

	if(tc == TableCompass::NORTH){
		position.a = obstaclePosition.a - _offset; //We hit Xmax
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::SOUTH){
		position.a = obstaclePosition.a + _offset; //We hit Xmin
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::EAST){
		position.b = obstaclePosition.a - _offset; //We hit Ymax
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::WEST){
		position.b = obstaclePosition.a + _offset; //We hit Ymin
		motion.setAbsPosition(position);
	}

	motion.setAbsPosition(position);

	motion.setFeedrate(FAST);

	if(wasAbsolute) motion.setAbsolute();
}

void SystemApplication::testSteppers(){
	motion.setAbsPosition(Vec3(0,0,0));
	motion.setAbsolute();
	go(1000,1000);
}

void SystemApplication::testOrientation(){
	motion.setAbsPosition({0,0,0});
	motion.setRelative();
	//align(RobotCompass::A, getCompassOrientation(TableCompass::EAST));
	//align(RobotCompass::AB, getCompassOrientation(TableCompass::EAST));
    int i = 0;
    lidar.displayRadar();

    again:
    goPolar(i, 50);lidar.checkLidar(-motion.getTargetDirection() * RAD_TO_DEG);delay(1000);
    goPolar(i, -50);
    i+=15;
    goto again;
}

void SystemApplication::setState(RobotState st){
    printState(_state);
	_state = st;
    printState(st);
}

void SystemApplication::printState(RobotState st){
	if(st == RobotState::IDLE)		Console::trace("RoboState") << "IDLE" 		<< Console::endl;
	else if(st == RobotState::ARMED)		Console::trace("RoboState") <<"ARMED" 		<< Console::endl;
	else if(st == RobotState::STARTING)	Console::trace("RoboState") <<"STARTING" 	<< Console::endl;
	else if(st == RobotState::STARTED)	Console::trace("RoboState") <<"STARTED" 	<< Console::endl;
	else if(st == RobotState::FINISHING)	Console::trace("RoboState") <<"FINISHING" 	<< Console::endl;
	else if(st == RobotState::FINISHED)	Console::trace("RoboState") <<"FINISHED" 	<< Console::endl;
	else Console::trace("RoboState") << "UNKNOWN" << Console::endl;
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

    motion.turnAwait(30);

    motion.goAwait(100,0);
    motion.goAwait(0,0);

    motion.goAsync(100,0);
    delay(500);
    motion.cancel();
    motion.goAsync(100,0);
    waitUntil(motion.getCurrentJob());
    motion.goAwait(0,0);
    motion.goAwait(100,0);
}



void SystemApplication::recalagePrimaryBlue(){

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,0));
    probeBorder(TableCompass::SOUTH, RobotCompass::BC);
    probeBorder(TableCompass::EAST, RobotCompass::CA);
	motion.setAbsolute();
	align(RobotCompass::AB, getCompassOrientation(TableCompass::NORTH));

    actuators.ungrab(RobotCompass::AB);
}

void SystemApplication::recalagePrimaryGreen(){
    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,0));
    probeBorder(TableCompass::SOUTH, RobotCompass::BC);
    probeBorder(TableCompass::WEST, RobotCompass::AB);
	motion.setAbsolute();
	align(RobotCompass::AB, getCompassOrientation(TableCompass::NORTH));

    actuators.ungrab(RobotCompass::AB);
}

void SystemApplication::recalageSecondaryBlue(){
    Console::info("Robot") << "Recalage Secondary Blue Cherry" << Console::endl;

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,PI));
    probeBorder(TableCompass::NORTH, RobotCompass::BC);
    probeBorder(TableCompass::EAST, RobotCompass::AB);
	motion.setAbsolute();
    go(b3);
	align(RobotCompass::A, getCompassOrientation(TableCompass::WEST));
}

void SystemApplication::recalageSecondaryGreen(){
    Console::info("Robot") << "Recalage Secondary Green Cherry" << Console::endl;

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    motion.setAbsPosition(Vec3(-1,-1,PI));
    probeBorder(TableCompass::NORTH, RobotCompass::BC);
    probeBorder(TableCompass::WEST, RobotCompass::CA);
	motion.setAbsolute();
    go(v3);
	align(RobotCompass::A, getCompassOrientation(TableCompass::EAST));

}

void SystemApplication::recalageSecondaryCakeBlue(){
    Console::info("Robot") << "Recalage Secondary Blue Cake" << Console::endl;

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

}

void SystemApplication::recalageSecondaryCakeGreen(){
    Console::info("Robot") << "Recalage Secondary Green Cake " << Console::endl;

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

}

void SystemApplication::matchPrimaryBlue(){
	motion.setAbsolute();
    addScore(basket);

    lidar.ignoreObstacles(true); //Ignore obstacle as match start

    actuators.ungrab(RobotCompass::AB);
	actuators.ungrab(RobotCompass::BC);
	actuators.ungrab(RobotCompass::CA);

    //Cake Rose
    actuators.ungrab(RobotCompass::AB);
	go(cakePinkSE);
	actuators.grab(RobotCompass::AB);
	align(RobotCompass::BC, getCompassOrientation(TableCompass::NORTH));

    //Cake Jaune
    lidar.ignoreObstacles(false);//Watch out now
    actuators.ungrab(RobotCompass::BC);
    go(cakeYellowSE);
    actuators.grab(RobotCompass::BC);
    align(RobotCompass::CA, 45);
    
    if(inputs.isBrown()){
        //Cake Marron
        actuators.ungrab(RobotCompass::CA);
        go(cakeBrownSE);
        actuators.grab(RobotCompass::CA);
    }
    else
        actuators.close(RobotCompass::CA);


    //----ATTENTION----
    lidar.ignoreObstacles(true); //Ignore obstacles while going home
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

    if(inputs.isBrown()){
        //Dépose du troisieme Gateau
        align(RobotCompass::CA, -120);
        actuators.unlock(RobotCompass::CA);
        go(dropBlue3);
        actuators.ungrab(RobotCompass::CA);
        go(retreatBlue3);
        actuators.close(RobotCompass::CA);
        addScore(cakeWithCherry);
    }

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
    wait(2000);
    actuators.trap.close();
    addScore(exactCherryCounting);
    addScore(cherryInBasket, 5);
    //recalage
    probeBorder(TableCompass::SOUTH, RobotCompass::A);
    motion.setAbsolute();

    //----ATTENTION----
    lidar.ignoreObstacles(false);

    // Fin de match
    go(blueEndPrimary);
	system.enable(NEOPIXEL);
}

void SystemApplication::matchPrimaryGreen(){
	lidar.ignoreObstacles(true); //Ignore as match begin
    motion.setAbsolute();

    addScore(basket);

    actuators.ungrab(RobotCompass::AB);
    actuators.ungrab(RobotCompass::BC);
    actuators.ungrab(RobotCompass::CA);

    //Cake Rose
    actuators.ungrab(RobotCompass::AB);
    go(cakePinkSW);
    actuators.grab(RobotCompass::AB);
    align(RobotCompass::BC, getCompassOrientation(TableCompass::NORTH));

    //Cake Jaune
    lidar.ignoreObstacles(false);//Watch out
    actuators.ungrab(RobotCompass::BC);
    go(cakeYellowSW);
    actuators.grab(RobotCompass::BC);
    align(RobotCompass::CA, -45);

    //Cake marron
    if(inputs.isBrown()){
        actuators.ungrab(RobotCompass::CA);
        go(cakeBrownSW);
        actuators.grab(RobotCompass::CA);
    }
    else
        actuators.close(RobotCompass::CA);

    //----ATTENTION----
    lidar.ignoreObstacles(true);

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

    if(inputs.isBrown()){
        //Dépose du troisieme Gateau
        align(RobotCompass::CA, 120);
        actuators.unlock(RobotCompass::CA);
        go(dropGreen3);
        actuators.ungrab(RobotCompass::CA);
        go(retreatGreen3);
        actuators.close(RobotCompass::CA);
        addScore(cakeWithCherry);
    }

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
    wait(2000);
    actuators.trap.close();
    addScore(exactCherryCounting);
    addScore(cherryInBasket, 5);
    //recalage
    probeBorder(TableCompass::SOUTH, RobotCompass::A);
    motion.setAbsolute();

    //----ATTENTION----
    lidar.ignoreObstacles(false);

    // Fin de match
    go(greenEndPrimary);
    system.enable(NEOPIXEL);

}

void SystemApplication::matchSecondaryBlue(){
	motion.setAbsolute();

    //----ATTENTION----
    lidar.ignoreObstacles(true);

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
    lidar.ignoreObstacles(false);

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

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    //Fin du match
    go(blueEndSecondary);

    // Fin de match
    actuators.trap.open();
    wait(1000);
}

void SystemApplication::matchSecondaryGreen(){
	motion.setAbsolute();

    //----ATTENTION----
    lidar.ignoreObstacles(true);

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
    lidar.ignoreObstacles(false);

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

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);
    //Fin du match
    go(greenEndSecondary);

    // Fin de match
    actuators.trap.open();
    wait(1000);
}

void SystemApplication::matchSecondaryCakeBlue(){
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

    // ----------------------------------------
    // Depose les gateaux 
    // (Version 2 - Protégés)
    // ----------------------------------------
    //Dépose du premier Gateau
    align(RobotCompass::BC, -120);
    go(retreatB2_04);
    actuators.unlock(RobotCompass::BC);
    go(dropB2_04);
    actuators.ungrab(RobotCompass::BC);
    go(retreatB2_05);
    actuators.close(RobotCompass::BC);
    addScore(cakeWithCherry);

    //Dépose du second Gateau
    align(RobotCompass::AB, -120);
    actuators.unlock(RobotCompass::AB);
    go(dropB2_05);
    actuators.ungrab(RobotCompass::AB);
    go(retreatB2_06);
    actuators.close(RobotCompass::AB);
    addScore(cakeWithCherry);

    //Dépose du troisieme Gateau
    align(RobotCompass::CA, -120);
    actuators.unlock(RobotCompass::CA);
    go(dropB2_06);
    actuators.ungrab(RobotCompass::CA);
    go(retreatB2_06);
    actuators.close(RobotCompass::CA);
    addScore(cakeWithCherry);

    // Close All before end Match
    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    //Securise
    go(dropB2_03);
    while(chrono.getTimeLeftSeconds() >= 10)
    {
        go(protectB2_01);
        //wait(2000);
        go(protectB2_02);
        //wait(2000);
    }

    //Fin du match
    go(blueEndSecondary);

    // Fin de match
    actuators.trap.open();
    wait(1000);
}

void SystemApplication::matchSecondaryCakeGreen(){
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

    // ----------------------------------------
    // Depose les gateaux 
    // (Version 2 - Protégés)
    // ----------------------------------------
    //Dépose du premier Gateau
    align(RobotCompass::BC, 120);
    go(retreatV2_04);
    actuators.unlock(RobotCompass::BC);
    go(dropV2_04);
    actuators.ungrab(RobotCompass::BC);
    go(retreatV2_05);
    actuators.close(RobotCompass::BC);
    addScore(cakeWithCherry);

    //Dépose du second Gateau
    align(RobotCompass::AB, 120);
    actuators.unlock(RobotCompass::AB);
    go(dropV2_05);
    actuators.ungrab(RobotCompass::AB);
    go(retreatV2_06);
    actuators.close(RobotCompass::AB);
    addScore(cakeWithCherry);

    //Dépose du troisieme Gateau
    align(RobotCompass::CA, 120);
    actuators.unlock(RobotCompass::CA);
    go(dropV2_06);
    actuators.ungrab(RobotCompass::CA);
    go(retreatV2_06);
    actuators.close(RobotCompass::CA);
    addScore(cakeWithCherry);

    // Close All before end Match
    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    //Securise
    go(dropV2_03);
    while(chrono.getTimeLeftSeconds() >= 10)
    {
        go(protectV2_01);
        //wait(2000);
        go(protectV2_02);
        //wait(2000);
    }

    //Fin du match
    go(greenEndSecondary);

    // Fin de match
    actuators.trap.open();
    wait(1000);
}

void SystemApplication::matchSecondaryCakeBlueOld(){
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

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);
    //Fin du match
    go(blueEndSecondary);

    // Fin de match
    actuators.trap.open();
    wait(1000);
}

void SystemApplication::matchSecondaryCakeGreenOld(){
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

    // ----------------------------------------
    // Depose les gateaux 
    // (Version 1 - En ligne - Non protégée)
    // ----------------------------------------
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

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    //Go take some other Cake ! 17/05/2023

    go(2210,724);

    // Take YellowNW
    actuators.open(RobotCompass::CA);
    align(RobotCompass::CA, getCompassOrientation(TableCompass::WEST));
    go(cakeYellowNW);
    actuators.grab(RobotCompass::CA);

    // Take YellowNW
    actuators.open(RobotCompass::BC);
    align(RobotCompass::BC, getCompassOrientation(TableCompass::NORTH));
    go(cakePinkNW);
    actuators.grab(RobotCompass::BC);

    // Pos face to the depose Zone
    go(dropV3_03);

    // Depose des Cake
    align(RobotCompass::BC, getCompassOrientation(TableCompass::NORTH));
    go(v3);
    actuators.ungrab(RobotCompass::BC);
    addScore(cake,3);
    go(dropV3_02);
    align(RobotCompass::CA, getCompassOrientation(TableCompass::NORTH));
    actuators.ungrab(RobotCompass::CA);
    addScore(cake,3);
    go(dropV3_03);

    // Close All before end Match

    actuators.close(RobotCompass::AB);
    actuators.close(RobotCompass::BC);
    actuators.close(RobotCompass::CA);

    //Fin du match
    go(greenEndSecondary);

    // Fin de match
    actuators.trap.open();
    wait(1000);
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
}

void SystemApplication::finishPrimaryGreen(){
    // Fin de match
    system.enable(NEOPIXEL);
}

void SystemApplication::finishSecondaryBlue(){
    // Fin de match

}

void SystemApplication::finishSecondaryGreen(){
    // Fin de match

}

