#include "Robot.h"
#include "Settings.h"

#include "debug/Console.h"


void  Robot::ProbeBorder(TableCompass tc, RobotCompass rc){
	boolean wasAbsolute = motion.IsAbsolute();
	_probing = true;

	motion.SetRelative();
	Align(rc, GetCompassOrientation(tc));

	motion.steppers.SetFeedrate(SLOW);
	GoPolar(GetCompassOrientation(rc),200);
	motion.steppers.SetFeedrate(10);
	GoPolar(GetCompassOrientation(rc),80);

	float _offset = GetOffsets(rc);

	Vec3 position = motion.GetAbsPosition();

	if(tc == TableCompass::NORTH){
		position.a = 3000.0 - _offset; //We hit Xmax
		_probedX = true;
		motion.SetAbsPosition(position);
	}
	if(tc == TableCompass::SOUTH){
		position.a = 0.0 + _offset; //We hit Xmin
		_probedX = true;
		motion.SetAbsPosition(position);
	}
	if(tc == TableCompass::EAST){
		position.b = 2000.0 - _offset; //We hit Ymax
		_probedY = true;
		motion.SetAbsPosition(position);
	}
	if(tc == TableCompass::WEST){
		position.b = 0.0 + _offset; //We hit Ymin
		_probedY = true;
		motion.SetAbsPosition(position);
	}

	motion.SetAbsPosition(position);

	GoPolar(GetCompassOrientation(rc),-100);

	motion.steppers.SetFeedrate(FAST);

	if(wasAbsolute) motion.SetAbsolute();
	_probing = false;
}


void Robot::ProbeObstacle(Vec2 obstaclePosition,TableCompass tc, RobotCompass rc){
	boolean wasAbsolute = motion.IsAbsolute();
	_probing = true;

	motion.SetRelative();
	Align(rc, GetCompassOrientation(tc));

	motion.steppers.SetFeedrate(5);
	GoPolar(GetCompassOrientation(rc),80);

	float _offset = GetOffsets(rc);

	Vec3 position = motion.GetAbsPosition();

	if(tc == TableCompass::NORTH){
		position.a = obstaclePosition.a - _offset; //We hit Xmax
		_probedX = true;
		motion.SetAbsPosition(position);
	}
	if(tc == TableCompass::SOUTH){
		position.a = obstaclePosition.a + _offset; //We hit Xmin
		_probedX = true;
		motion.SetAbsPosition(position);
	}
	if(tc == TableCompass::EAST){
		position.b = obstaclePosition.a - _offset; //We hit Ymax
		_probedY = true;
		motion.SetAbsPosition(position);
	}
	if(tc == TableCompass::WEST){
		position.b = obstaclePosition.a + _offset; //We hit Ymin
		_probedY = true;
		motion.SetAbsPosition(position);
	}

	motion.SetAbsPosition(position);

	motion.steppers.SetFeedrate(FAST);

	if(wasAbsolute) motion.SetAbsolute();
	_probing = false;
}
