#include "strategy.h"

void recalage(){
    motion.setSync();
    actuators.moveElevator(RobotCompass::BC,ElevatorPose::UP);
    probeBorder(TableCompass::SOUTH, RobotCompass::BC);
    probeBorder(TableCompass::WEST, RobotCompass::BC);
    //motion.go(POI::b1);
}

void probeBorder(){
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

	motion.goPolar(getCompassOrientation(rc),-100);

	if(wasAbsolute) motion.setAbsolute();
    motion.setFeedrate(currentFeed);
	//_probing = false;
});