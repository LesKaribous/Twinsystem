#include "extension/motion/extMotion.h"
#include "system.h"

ExtMotion::ExtMotion(System& sys) : system(sys), motion(*sys.motion){
}

ExtMotion::~ExtMotion(){}

void ExtMotion::go(Vec2 v){
	motion.goAsync(v);
	system.waitUntil(motion.getCurrentJob());
}

void ExtMotion::go(float x, float y){
	motion.goAsync(x, y);
	system.waitUntil(motion.getCurrentJob());
}

void ExtMotion::goAlign(Vec2 target, RobotCompass rc, float orientation){
	motion.goAlignAsync(target, rc, orientation);
	system.waitUntil(motion.getCurrentJob());
}

void ExtMotion::turn(float a){
	motion.turnAsync(a);
	system.waitUntil(motion.getCurrentJob());
}

void ExtMotion::align(RobotCompass rc, float orientation){
	bool wasRelative = motion.isRelative();
	motion.setAbsolute();
	motion.alignAsync(rc, orientation);
	system.waitUntil(motion.getCurrentJob());
	if(wasRelative) motion.setRelative();
}

void ExtMotion::goPolar(float heading, float length){
	PolarVec target = PolarVec(heading*DEG_TO_RAD, length);
	bool wasAbsolute = motion.isAbsolute();
	motion.setRelative();
	motion.goAsync(target.toVec2());
	system.waitUntil(motion.getCurrentJob());

	if(wasAbsolute) motion.setAbsolute();
}

bool  ExtMotion::isProbed(){
	return _probedX && _probedY;
}
bool  ExtMotion::isXProbed(){
	return _probedX;
}
bool  ExtMotion::isYProbed(){
	return _probedY;
}
bool  ExtMotion::isProbing(){
	return _probing;
}
