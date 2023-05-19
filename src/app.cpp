#include "app.h"
#include "match/poi.h"
#include "match/score.h"

#define lidar (*_lidarPtr)
#define screen (*_screenPtr)
#define inputs (*_inputsPtr)
#define motion (*_motionPtr)
#define planner (*_plannerPtr)
#define neopixel (*_neopixelPtr)
#define terminal (*_terminalPtr)
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

    screen.x.SetValue(motion.getEstimatedAbsPosition().x);
    screen.y.SetValue(motion.getEstimatedAbsPosition().y);
    screen.z.SetValue(motion.getEstimatedAbsPosition().z);
    screen.score.SetValue(_score);
    screen.time.SetValue(chrono.getTimeLeftSeconds());
    screen.intercom.SetValue(lidar.isConnected());


    if(_state == RobotState::IDLE){

        //localisation.addMeasure(Vec3(inputs.getDistanceSensorA(), inputs.getDistanceSensorB(), inputs.getDistanceSensorC()));
        
        //if(millis() - _lastDrift > 200){
            //_lastDrift = millis();
            //localisation.estimateDrift(motion.getAbsPosition());
        //}

        if(terminal.commandAvailable())
            processCommand(terminal.dequeCommand());

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
}


void SystemApplication::processCommand(Command c){  

    if(!terminal.isEnabled())return;
    if(c.isValidFormat("go(x,y)")){
        go(c.getVec2());
    }else if(c.isValidFormat("turn(angle)")){
        turn(c.getFloat());
    }else if(c.isValidFormat("setAbsPosition(x,y)")){
        motion.setAbsPosition(Vec3(c.getFloat(0), c.getFloat(1), motion.getAbsPosition().c));

        THROW(c.getFloat(0));
        THROW(c.getFloat(1));

    }else if(c.isValidFormat("setAbsPosition(x,y,t)")){
        motion.setAbsPosition(Vec3(c.getFloat(0), c.getFloat(1), c.getFloat(2)));

        THROW(c.getFloat(0));
        THROW(c.getFloat(1));
        THROW(c.getFloat(2));

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

    screen.drawBootProgress(70, "Loading Terminal...");
    _terminalPtr = std::make_unique<Terminal>();

    //screen.drawBootProgress(80, "Loading Localisation...");
    //_localisationPtr = std::make_unique<Localisation>();

    screen.drawBootProgress(90, "Linking modules...");

    system.registerModule(_lidarPtr.get());
    system.registerModule(_screenPtr.get());
    system.registerModule(_inputsPtr.get());
    system.registerModule(_motionPtr.get());
    //system.registerModule(_plannerPtr.get());
    system.registerModule(_neopixelPtr.get());
    system.registerModule(_terminalPtr.get());
    system.registerModule(_actuatorsPtr.get());
    //system.registerModule(_localisationPtr.get());

    system.enable(LIDAR);
    system.enable(INPUTS);
    system.enable(SCREEN);
	//system.enable(MOTION);
    //system.enable(NEOPIXEL);
    system.enable(TERMINAL);
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
    system.disable(NEOPIXEL);
	while (_state != RobotState::STARTING){
		update();
		switch (_state){
		case RobotState::IDLE :
			if(inputs.starterPlaced()){ //Arm
				setState(RobotState::ARMED);
                //system.disable(NEOPIXEL);

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

                system.disable(MOTION);
                lidar.ignoreObstacles(false);
			}
			break;

		case RobotState::ARMED :
			if(inputs.starterPulled()){ //Start match
				setState(RobotState::STARTING);
			}else if(inputs.starterCancelled()){ //Unarm
				setState(RobotState::IDLE);
				inputs.unfreezeSettings();
				//system.enable(NEOPIXEL);
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

void SystemApplication::dessin(){
system.disable(TERMINAL);
	chrono.start();
    system.enable(MOTION);

    system.disable(INPUTS);
    system.disable(NEOPIXEL);


motion.setAbsPosition(Vec3(0,0,0));
motion.setAbsolute();
motion.goAwait(-50.0,77.0);
motion.goAwait(-53.0,61.0);
motion.goAwait(-60.0,47.0);
motion.goAwait(-73.0,37.0);
motion.goAwait(-87.0,29.0);
motion.goAwait(-101.0,22.0);
motion.goAwait(-117.0,19.0);
motion.goAwait(-135.0,16.0);
motion.goAwait(-153.0,17.0);
motion.goAwait(-168.0,21.0);
motion.goAwait(-181.0,29.0);
motion.goAwait(-193.0,44.0);
motion.goAwait(-201.0,57.0);
motion.goAwait(-203.0,76.0);
motion.goAwait(-204.0,91.0);
motion.goAwait(-201.0,106.0);
motion.goAwait(-188.0,119.0);
motion.goAwait(-175.0,129.0);
motion.goAwait(-160.0,135.0);
motion.goAwait(-144.0,137.0);
motion.goAwait(-127.0,136.0);
motion.goAwait(-111.0,132.0);
motion.goAwait(-92.0,123.0);
motion.goAwait(-78.0,117.0);
motion.goAwait(-72.0,103.0);
motion.goAwait(-65.0,89.0);
motion.goAwait(-57.0,73.0);
motion.goAwait(-40.0,54.0);
motion.goAwait(-26.0,46.0);
motion.goAwait(-11.0,41.0);
motion.goAwait(4.0,38.0);
motion.goAwait(19.0,43.0);
motion.goAwait(33.0,49.0);
motion.goAwait(47.0,58.0);
motion.goAwait(56.0,71.0);
motion.goAwait(55.0,88.0);
motion.goAwait(54.0,103.0);
motion.goAwait(45.0,116.0);
motion.goAwait(30.0,117.0);
motion.goAwait(13.0,124.0);
motion.goAwait(-1.0,131.0);
motion.goAwait(-16.0,133.0);
motion.goAwait(-32.0,132.0);
motion.goAwait(-47.0,130.0);
motion.goAwait(-62.0,124.0);
motion.goAwait(-69.0,110.0);
motion.goAwait(-53.0,53.0);
motion.goAwait(-40.0,43.0);
motion.goAwait(-28.0,32.0);
motion.goAwait(-15.0,13.0);
motion.goAwait(-5.0,0.0);
motion.goAwait(5.0,-12.0);
motion.goAwait(14.0,-25.0);
motion.goAwait(21.0,-39.0);
motion.goAwait(27.0,-54.0);
motion.goAwait(29.0,-69.0);
motion.goAwait(23.0,-83.0);
motion.goAwait(15.0,-96.0);
motion.goAwait(2.0,-106.0);
motion.goAwait(-14.0,-106.0);
motion.goAwait(-29.0,-105.0);
motion.goAwait(-44.0,-101.0);
motion.goAwait(-58.0,-90.0);
motion.goAwait(-70.0,-76.0);
motion.goAwait(-77.0,-62.0);
motion.goAwait(-78.0,-47.0);
motion.goAwait(-78.0,-31.0);
motion.goAwait(-78.0,-15.0);
motion.goAwait(-78.0,1.0);
motion.goAwait(-78.0,17.0);
motion.goAwait(-78.0,33.0);
motion.goAwait(-83.0,18.0);
motion.goAwait(-91.0,0.0);
motion.goAwait(-97.0,-15.0);
motion.goAwait(-103.0,-33.0);
motion.goAwait(-110.0,-47.0);
motion.goAwait(-114.0,-65.0);
motion.goAwait(-120.0,-79.0);
motion.goAwait(-130.0,-91.0);
motion.goAwait(-144.0,-99.0);
motion.goAwait(-161.0,-103.0);
motion.goAwait(-178.0,-104.0);
motion.goAwait(-194.0,-102.0);
motion.goAwait(-207.0,-94.0);
motion.goAwait(-215.0,-75.0);
motion.goAwait(-218.0,-59.0);
motion.goAwait(-215.0,-44.0);
motion.goAwait(-204.0,-28.0);
motion.goAwait(-194.0,-13.0);
motion.goAwait(-183.0,-2.0);
motion.goAwait(-171.0,8.0);
motion.goAwait(-192.0,11.0);
motion.goAwait(-215.0,4.0);
motion.goAwait(-240.0,-3.0);
motion.goAwait(-262.0,-9.0);
motion.goAwait(-281.0,-16.0);
motion.goAwait(-296.0,-20.0);
motion.goAwait(-312.0,-9.0);
motion.goAwait(-323.0,4.0);
motion.goAwait(-333.0,19.0);
motion.goAwait(-338.0,38.0);
motion.goAwait(-341.0,53.0);
motion.goAwait(-338.0,68.0);
motion.goAwait(-333.0,83.0);
motion.goAwait(-316.0,84.0);
motion.goAwait(-298.0,85.0);
motion.goAwait(-280.0,85.0);
motion.goAwait(-262.0,85.0);
motion.goAwait(-245.0,87.0);
motion.goAwait(-229.0,89.0);
motion.goAwait(-226.0,108.0);
motion.goAwait(-236.0,121.0);
motion.goAwait(-245.0,134.0);
motion.goAwait(-254.0,147.0);
motion.goAwait(-264.0,164.0);
motion.goAwait(-274.0,176.0);
motion.goAwait(-287.0,198.0);
motion.goAwait(-294.0,214.0);
motion.goAwait(-300.0,228.0);
motion.goAwait(-284.0,235.0);
motion.goAwait(-267.0,240.0);
motion.goAwait(-251.0,241.0);
motion.goAwait(-233.0,239.0);
motion.goAwait(-216.0,237.0);
motion.goAwait(-199.0,235.0);
motion.goAwait(-184.0,230.0);
motion.goAwait(-177.0,214.0);
motion.goAwait(-168.0,195.0);
motion.goAwait(-163.0,180.0);
motion.goAwait(-155.0,160.0);
motion.goAwait(-148.0,146.0);
motion.goAwait(-145.0,131.0);
motion.goAwait(-142.0,151.0);
motion.goAwait(-138.0,171.0);
motion.goAwait(-135.0,186.0);
motion.goAwait(-128.0,200.0);
motion.goAwait(-116.0,210.0);
motion.goAwait(-100.0,216.0);
motion.goAwait(-83.0,217.0);
motion.goAwait(-62.0,216.0);
motion.goAwait(-47.0,214.0);
motion.goAwait(-30.0,213.0);
motion.goAwait(-24.0,199.0);
motion.goAwait(-32.0,183.0);
motion.goAwait(-41.0,168.0);
motion.goAwait(-47.0,154.0);
motion.goAwait(-53.0,140.0);
motion.goAwait(-60.0,125.0);


}


void SystemApplication::danse(){
    motion.setAbsPosition(Vec3(500,500,0));
    motion.setRelative();
    system.enable(NEOPIXEL);

    //00:00 - 8:00
    motion.setFeedrate(20);
    motion.moveAsync({0,0,-2000});
    delay(2000);motion.forceCancel();
    motion.moveAsync({0,0,2000});
    delay(2000);motion.forceCancel();
    motion.moveAsync({0,0,-2000});
    delay(2000);motion.forceCancel();
    motion.moveAsync({0,0,2000});
    delay(2000);motion.forceCancel();
    //delay(650);

    // 8:650 : 16:00
    motion.setFeedrate(40);
    for(int i = 0; i < 8; i++){
        motion.moveAsync({0,0,3000});
        actuators.danseRight();
        delay(460);motion.forceCancel();
        motion.moveAsync({0,0,-3000});
        actuators.danseLeft();
        delay(460);motion.forceCancel();
    }
    motion.moveAsync({0,0,3000});
    actuators.danseRight();
    delay(460);motion.forceCancel();

    motion.setFeedrate(60);
    // 16:146 : 24:00
    motion.moveAsync({0,0,3000});
    delay(4000);motion.forceCancel();

    motion.moveAsync({0,0,-3000});
    delay(4000);motion.forceCancel();

    actuators.close();
    motion.moveAwait({0,-400,0});

    motion.setFeedrate(40);
    for(int i = 0; i < 8; i++){
        motion.moveAsync({0,0,3000});
        actuators.danseRight();
        delay(460);motion.forceCancel();
        motion.moveAsync({0,0,-3000});
        actuators.danseLeft();
        delay(460);motion.forceCancel();
    }
    motion.moveAsync({0,0,3000});
    actuators.danseRight();
    delay(460);motion.forceCancel();


    motion.setFeedrate(30);
    motion.moveAsync({0,0,3000});
    actuators.applause(RobotCompass::AB);
    actuators.applause(RobotCompass::BC);
    actuators.applause(RobotCompass::CA); 
    motion.forceCancel();
    motion.moveAsync({0,0,-3000});
    actuators.applause(RobotCompass::AB);
    actuators.applause(RobotCompass::BC);
    actuators.applause(RobotCompass::CA); 

    delay(10000);
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

