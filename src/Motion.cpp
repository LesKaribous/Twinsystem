#include "Motion.h"
#include "Controller.h"
#include "Match.h"

#include "Debugger.h"
#include "Geometry.h"
#include "Settings.h"
#include "Intercom.h"

namespace Motion
{

	Vec3 cPosition 		= {0,0,0};
	Vec3 cTarget 		= {0,0,0};
	Vec3 calibration 	= {1,1,1};

	Vec2 controlPoint   = {0,0};

 	bool probedX = false, probedY = false;
	bool absolute 		= true;
	bool probing = false;

	void init(){
		calibration = Settings::ROBOT == Settings::PRIMARY ?  
			Settings::Calibration::Primary.Cartesian : Settings::Calibration::Secondary.Cartesian;
		
		absolute = Settings::ABSOLUTE;
		Controller::setFeedrate(100);
	}

	void computeSync(){
		Match::update();
		Intercom::checkSerial();
	}

	Vec3 GetPosition(){
		return Vec3(cPosition.a, cPosition.b, cPosition.c * RAD_TO_DEG);
	}

	Vec3 GetTarget(){
		return cTarget;
	}

	bool isAbsolute(){
		return absolute;
	}

	bool isRelative(){
		return !absolute;
	}

	bool isProbed(){
		return probedX && probedY;
	}
	bool isXProbed(){
		return probedX;
	}
	bool isYProbed(){
		return probedY;
	}

	void SetTarget(Vec3 target){
		target.c *= DEG_TO_RAD;
		if(absolute){
			target.sub(cPosition);
			target.rotateZ(cPosition.c);
		}
		
		while(target.c > PI) target.c -= 2.0f*PI;
		while(target.c < -PI) target.c += 2.0f*PI;
		cTarget = target;
		Intercom::focus();
	}

	void SetPosition(Vec2 newPos){
		cPosition = Vec3(newPos, cPosition.c*RAD_TO_DEG);
	}
	void SetPosition(Vec3 newPos){
		cPosition = newPos.mult(Vec3(1.0,1.0,DEG_TO_RAD).toMatrix());
	}

    void SetAbsolute(bool state){
		absolute = state;
	}

    void SetRelative(bool state){
		SetAbsolute(!state);
	}


    void turn(float angle){
		Controller::setFeedrate(30);
		if (absolute) move({cPosition.a, cPosition.b, angle});
		else move({0, 0, angle});
		Controller::setFeedrate(100);
	}

    void goPolar(float heading, float length){
		PolarVec target(-heading*DEG_TO_RAD, length); // Switch to left hand coord system
		go(target.toVec2());
	}

	void go(float x, float y){
		Vec2 target = {x, y};
		go(target);
	}

	void go(Vec2 target){
		if (absolute) move({target.a, target.b, cPosition.c*RAD_TO_DEG});
		else move({target.a, target.b, 0});
	}

	void goTurn(float x, float y, float heading){
		goTurn({x, y, heading});
	}
	void goTurn(Vec3 target){
		go(Vec2(target.a, target.b));
		turn(target.c);
	}

	void probeBorder(Vec2 borderPos){
		boolean tAbsolute = isAbsolute();
		probing = true;
		
		align(borderPos);

		SetRelative();

		Controller::setFeedrate(FAST);
		go(-borderPos.mag() - 80,.0);
		Controller::setFeedrate(SLOW);
		go(-120,.0);

		float _offset = Settings::Geometry::offset ;

		if(cPosition.a < 0.0 + _offset){
			cPosition.a = 0.0 + _offset;
			probedX = true;
		}else if(cPosition.a > 3000.0 - _offset)
			cPosition.a = 3000.0 - _offset; 
		else if(cPosition.b < 0.0){
			cPosition.b = 0.0 + _offset;
			probedY = true;
		}else if(cPosition.b > 2000.0)
			cPosition.b = 2000.0 - _offset;
		
		go(100,.0);

		Controller::setFeedrate(FAST);
		SetAbsolute(tAbsolute);
		probing = false;
	}

	bool isProbing(){
		return probing;
	}

	void align(Vec2 coord){
		boolean tAbsolute = isAbsolute();
		SetAbsolute(tAbsolute);
		turn(coord.heading()*RAD_TO_DEG+90);
		SetAbsolute(tAbsolute);
	}

	//Raw relative move request
	void move(Vec3 target){
		SetTarget(target);

		target.mult(calibration.toMatrix()); //Apply calibration (X mm,Y mm,ROT rad)
		target = ik(target); //Apply inverse kinematics (X mm,Y mm,ROT rad) -> (Va, Vb, Vc) : steps
		target.mult(Settings::Stepper::STEP_MODE * RAD_TO_DEG);

		Controller::move(target, true);
		while(running()) computeSync();

		if(absolute) cPosition.add(cTarget);
		else cPosition.add(cTarget.rotateZ(-cPosition.c));
	}

	bool running(){
		return Controller::isRunning();
	}

	Vec3 ik(Vec3 target){
		float c60 = cosf(PI/3.0f),
			  s60 = sinf(PI/3.0f),
			  L = Settings::RADIUS,
			  R = Settings::WHEEL_RADIUS;
			  
		Matrix3x3 P = {
			   0,   1 , L,
			-s60, -c60, L,
			 s60, -c60, L
		};

		return target.mult(P).mult(1/R);
	}

}
