#include "strategy.h"
#include "os/routines.h"

void match(){
    //start match
    motion.setFeedrate(0.4);
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
    float startOffset = 260.0;
    float grabOffset = 60.0;
    float pushOffset = 20.0;
    float newTargetY = target.y;

    // Ralentir
    motion.setFeedrate(0.1);

    // Mettre les bras en position Grab
    actuators.moveElevator(RobotCompass::AB,ElevatorPose::GRAB);
    actuators.moveElevator(RobotCompass::BC,ElevatorPose::GRAB);
    actuators.moveElevator(RobotCompass::CA,ElevatorPose::GRAB);

    // On se positionne en bordure de zone
    if(tc == TableCompass::EAST) newTargetY = newTargetY - startOffset;
    else if(tc == TableCompass::WEST) newTargetY = newTargetY + startOffset;
    motion.go(target.x, newTargetY);
    
    for(int i = 0; i < 3; i++){

        
        actuators.open(rc);

        // Avancer vers la plante
        if(tc == TableCompass::EAST) newTargetY = newTargetY + grabOffset;
        else if(tc == TableCompass::WEST) newTargetY = newTargetY - grabOffset;
        motion.go(target.x, newTargetY);

        // Prendre la plante
        actuators.close(rc);
        waitMs(1000);

        // Avancer un peu avant de grab
        if(tc == TableCompass::EAST) motion.go(target.x, newTargetY + pushOffset);
        else if(tc == TableCompass::WEST) motion.go(target.x, newTargetY - pushOffset);

        actuators.grab(rc);
        waitMs(1000);

        // Reculer
        if(tc == TableCompass::EAST) motion.go(target.x, newTargetY - grabOffset);
        else if(tc == TableCompass::WEST) motion.go(target.x, newTargetY + grabOffset);
        
        // Lever les plantes et suivant
        actuators.moveElevator(rc,ElevatorPose::UP);
        rc = nextActuator(rc);
        if(i<2) motion.align(rc, getCompassOrientation(tc)); // Ne pas effectuer la rotation sur la derniere action
    }

    motion.setFeedrate(0.4);    
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

void waitMs(unsigned long time){
    // To fix with asynch wait
    //os.wait(time,false);
    delay(time); // WIP -> To fix
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

    motion.setFeedrate(0.15);
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
