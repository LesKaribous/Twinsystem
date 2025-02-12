#include "strategy.h"
#include "poi.h"
#include "robot.h"
#include "routines.h"

void match(){
    //start match
    motion.setFeedrate(1.0);

    if(ihm.isColorA()) 
        matchA();
    else 
        matchB();

}

void recalage(){
    motion.setSync();
    actuators.moveElevator(RobotCompass::BC,ElevatorPose::UP);
    waitMs(800);
    THROW(1)
    if(ihm.isColorA()){
        probeBorder(TableCompass::SOUTH, RobotCompass::BC,100);
        probeBorder(TableCompass::WEST,  RobotCompass::BC,100);
        async motion.go(POI::b1);
        async motion.align(RobotCompass::CA, getCompassOrientation(TableCompass::EAST));
        actuators.moveElevator(RobotCompass::AB,ElevatorPose::GRAB);
        actuators.moveElevator(RobotCompass::BC,ElevatorPose::GRAB);
        actuators.moveElevator(RobotCompass::CA,ElevatorPose::GRAB);
    }
    else{
        motion.setAbsPosition({-1,-1,PI});
        probeBorder(TableCompass::NORTH, RobotCompass::BC,100);
        probeBorder(TableCompass::WEST,  RobotCompass::BC,100);
        async motion.go(POI::y1);
        async motion.align(RobotCompass::AB, getCompassOrientation(TableCompass::EAST));
        actuators.moveElevator(RobotCompass::AB,ElevatorPose::GRAB);
        actuators.moveElevator(RobotCompass::BC,ElevatorPose::GRAB);
        actuators.moveElevator(RobotCompass::CA,ElevatorPose::GRAB);
    }
}

void matchA(){
    //do Match
    motion.disableOptimization();
    async motion.turn(3600);
    chrono.onMatchNearlyFinished();
    chrono.onMatchFinished();
}

void matchB(){
    //do Match
    motion.disableOptimization();
    async motion.turn(-3600);
    
    chrono.onMatchNearlyFinished();
    chrono.onMatchFinished();
}


void waitMs(unsigned long time){
    os.wait(time,false);
    //delay(time);
}


RobotCompass nextActuator(RobotCompass rc){
    int RobotCompassSize = 6;
    return static_cast<RobotCompass>((static_cast<int>(rc) + 2) % RobotCompassSize);
}

RobotCompass previousActuator(RobotCompass rc){
    int RobotCompassSize = 6;
    return static_cast<RobotCompass>((static_cast<int>(rc) + RobotCompassSize - 2) % RobotCompassSize);
}

void probeBorder(TableCompass tc, RobotCompass rc, float clearance, float approachDist, float probeDist){
    

	boolean wasAbsolute = motion.isAbsolute();
    float currentFeedrate = motion.getFeedrate();
	//bool m_probing = true;
    //motion.setSync();
    //Console::println("Recalling");
    //motion.resetCompass();
    motion.setFeedrate(0.3);
	async motion.align(rc, getCompassOrientation(tc));

    motion.setRelative();
	async motion.goPolar(getCompassOrientation(rc),approachDist);
	async motion.goPolar(getCompassOrientation(rc),probeDist);

	float _offset = getOffsets(rc);

	Vec3 position = motion.getAbsPosition();

	if(tc == TableCompass::NORTH){
		position.a = 3000.0 - _offset; //We hit Xmax
		//_probedX = true;
	}else if(tc == TableCompass::SOUTH){
		position.a = 0.0 + _offset; //We hit Xmin
		//_probedX = true;
	}else if(tc == TableCompass::EAST){
		position.b = 2000.0 - _offset; //We hit Ymax
		//_probedY = true;
	}else if(tc == TableCompass::WEST){
		position.b = 0.0 + _offset; //We hit Ymin
		//_probedY = true;
	}
    position.c = DEG_TO_RAD * (getCompassOrientation(tc) - getCompassOrientation(rc));
	Console::println(position.c * RAD_TO_DEG);
    motion.setAbsPosition(position);

	async motion.goPolar(getCompassOrientation(rc),-clearance);
	if(wasAbsolute) motion.setAbsolute();
    motion.setFeedrate(currentFeedrate);


    
}
