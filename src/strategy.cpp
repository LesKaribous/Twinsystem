#include "strategy.h"
#include "os/routines.h"

void match(){
    //start match
    motion.setFeedrate(0.5);
    if(ihm.isColorBlue()) matchBlue();
    else matchYellow();
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
    float offset = 50.0;
    float newTargetY = target.y;
    
    for(int i = 0; i < 3; i++){

        actuators.moveElevator(rc,ElevatorPose::GRAB);
        actuators.open(rc);

        if(tc == TableCompass::EAST) newTargetY = newTargetY + offset;
        else if(tc == TableCompass::WEST) newTargetY = newTargetY - offset;
        motion.go(target.x, newTargetY);

        actuators.grab(rc);
        os.wait(1000,true);
        actuators.moveElevator(rc,ElevatorPose::UP);
        rc = nextActuator(rc);
        if(i<2) motion.align(rc, getCompassOrientation(tc)); // Ne pas effectuer la rotation sur la derniere action
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
    int RobotCompassSize = 6;
    return static_cast<RobotCompass>((static_cast<int>(rc) + 2) % RobotCompassSize);
}

RobotCompass previousActuator(RobotCompass rc){
    int RobotCompassSize = 6;
    return static_cast<RobotCompass>((static_cast<int>(rc) + RobotCompassSize - 2) % RobotCompassSize);
}

void probeBorder(TableCompass tc, RobotCompass rc, float clearance){
	boolean wasAbsolute = motion.isAbsolute();
    float currentFeed = motion.getFeedrate();
	bool m_probing = true;
    motion.setSync();

    motion.setFeedrate(0.1);
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
	//_probing = false;
}
