#include "modules/test/test.h"
#include "Settings.h"

void Test::testSteppers(){
	motion->setAbsPosition(Vec3(0,0,0));
	motion->setAbsolute();
	motion->go(1000,1000);
}

void Test::testOrientation(){
	motion->setAbsPosition({0,0,0});
	motion->setAbsolute();
	motion->align(RobotCompass::A, GetCompassOrientation(TableCompass::EAST));
	motion->align(RobotCompass::AB, GetCompassOrientation(TableCompass::EAST));
}


void Test::testDetection(){

	motion->setAbsPosition({10,10,0});
	motion->setAbsolute();

	lidar->getMaxLidarDist(Vec2(motion->getAbsPosition().a, motion->getAbsPosition().b), 0);

	lidar->getMaxLidarDist(Vec2(motion->getAbsPosition().a, motion->getAbsPosition().b), 90*DEG_TO_RAD);
	
	lidar->getMaxLidarDist(Vec2(motion->getAbsPosition().a, motion->getAbsPosition().b), 270*DEG_TO_RAD);
	
	lidar->getMaxLidarDist(Vec2(motion->getAbsPosition().a, motion->getAbsPosition().b), -90*DEG_TO_RAD);
	
}


void Test::testMotion(){

	motion->setAbsPosition({0,0,0});
	motion->setAbsolute();

	lidar->disable();

	motion->GoAlign(Vec2(100,0), RobotCompass::A, GetCompassOrientation(TableCompass::EAST));
	
}
