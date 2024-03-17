#include "strategy.h"
#include "os/routines.h"

void match(){

}

void recalage(){
    motion.setSync();
    actuators.moveElevator(RobotCompass::BC,ElevatorPose::UP);
    if(ihm.isColorBlue()){
        probeBorder(TableCompass::SOUTH, RobotCompass::BC,100);
        probeBorder(TableCompass::WEST,  RobotCompass::BC,100);
        motion.go(POI::b1);
        motion.align(RobotCompass::CA, getCompassOrientation(TableCompass::EAST));
        actuators.moveElevator(RobotCompass::AB,ElevatorPose::DOWN);
        actuators.moveElevator(RobotCompass::BC,ElevatorPose::DOWN);
        actuators.moveElevator(RobotCompass::CA,ElevatorPose::DOWN);
    }
    else{
        probeBorder(TableCompass::NORTH, RobotCompass::BC,100);
        probeBorder(TableCompass::EAST,  RobotCompass::BC,100);
        motion.go(POI::y1);
        motion.align(RobotCompass::AB, getCompassOrientation(TableCompass::EAST));
        actuators.moveElevator(RobotCompass::AB,ElevatorPose::DOWN);
        actuators.moveElevator(RobotCompass::BC,ElevatorPose::DOWN);
        actuators.moveElevator(RobotCompass::CA,ElevatorPose::DOWN);
    }
}

void takePlants(Vec2 target, RobotCompass rc, TableCompass tc){
    float offset = 100.0;
    float newTargetY = target.y;
    if(tc == TableCompass::EAST){
        newTargetY = newTargetY - offset;
    }
    else if(tc == TableCompass::WEST){
        newTargetY = newTargetY + offset;
    }

    for(int i = 0; i < 3; i++){
        actuators.moveElevator(rc,ElevatorPose::GRAB);
        actuators.open(rc);
        motion.go(target.x, newTargetY);
        actuators.close(rc);
        actuators.moveElevator(rc,ElevatorPose::UP);
        motion.align(rc, getCompassOrientation(tc));
    }
        
}

void matchBlue(){
    motion.go(1000,400);
    // Macro to take plants
    takePlants(POI::plantSupplySW, RobotCompass::CA, TableCompass::EAST);
}

void matchYellow(){
    motion.go(2000,400);
    // Macro to take plants
    takePlants(POI::plantSupplyNW, RobotCompass::AB, TableCompass::EAST);
}

RobotCompass nextActuator(RobotCompass rc){
    return static_cast<RobotCompass>((static_cast<int>(rc) + 2) % 6);
}

RobotCompass previousActuator(RobotCompass rc){
    return static_cast<RobotCompass>((static_cast<int>(rc) - 2) % 6);
}

void probeBorder(TableCompass tc, RobotCompass rc, float clearance){
	boolean wasAbsolute = motion.isAbsolute();
    float currentFeed = motion.getFeedrate();
	bool m_probing = true;
    motion.setSync();

    motion.setFeedrate(0.5);
	motion.setRelative();
	motion.align(rc, getCompassOrientation(tc));

	motion.goPolar(getCompassOrientation(rc),200);
	motion.goPolar(getCompassOrientation(rc),80);

	float _offset = getOffsets(rc);

	Vec3 position = motion.getAbsPosition();

	if(tc == TableCompass::NORTH){
		position.a = 3000.0 - _offset; //We hit Xmax
		//_probedX = true;
		motion.setAbsPosition(position);
	}else if(tc == TableCompass::SOUTH){
		position.a = 0.0 + _offset; //We hit Xmin
		//_probedX = true;
		motion.setAbsPosition(position);
	}else if(tc == TableCompass::EAST){
		position.b = 2000.0 - _offset; //We hit Ymax
		//_probedY = true;
		motion.setAbsPosition(position);
	}else if(tc == TableCompass::WEST){
		position.b = 0.0 + _offset; //We hit Ymin
		//_probedY = true;
		motion.setAbsPosition(position);
	}

	//motion.setAbsPosition(position);

	motion.goPolar(getCompassOrientation(rc),-clearance);

	if(wasAbsolute) motion.setAbsolute();
    motion.setFeedrate(currentFeed);
	//_probing = false;
}
