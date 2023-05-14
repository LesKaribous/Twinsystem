#include "extension/test/extTest.h"
#include "system.h"
#include "Settings.h"

ExtTest::ExtTest(System& sys) : system(sys),  lidar(*sys.lidar),
motion(*sys.motion), actuators(*sys.actuators){

}

ExtTest::~ExtTest(){

}


void ExtTest::testSteppers(){
	motion.setAbsPosition(Vec3(0,0,0));
	motion.setAbsolute();
	system.e_motion->go(1000,1000);
}

void ExtTest::testOrientation(){
	motion.setAbsPosition({0,0,0});
	motion.setAbsolute();
	system.e_motion->align(RobotCompass::A, getCompassOrientation(TableCompass::EAST));
	system.e_motion->align(RobotCompass::AB, getCompassOrientation(TableCompass::EAST));
}


void ExtTest::testDetection(){

	motion.setAbsPosition({10,10,0});
	motion.setAbsolute();

	lidar.getMaxLidarDist(Vec2(motion.getAbsPosition().a, motion.getAbsPosition().b), 0);

	lidar.getMaxLidarDist(Vec2(motion.getAbsPosition().a, motion.getAbsPosition().b), 90*DEG_TO_RAD);
	
	lidar.getMaxLidarDist(Vec2(motion.getAbsPosition().a, motion.getAbsPosition().b), 270*DEG_TO_RAD);
	
	lidar.getMaxLidarDist(Vec2(motion.getAbsPosition().a, motion.getAbsPosition().b), -90*DEG_TO_RAD);
	
}


void ExtTest::testMotion(){

	motion.setAbsPosition({0,0,0});
	motion.setAbsolute();

	lidar.disable();

}
