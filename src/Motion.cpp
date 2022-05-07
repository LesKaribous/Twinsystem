#include "Motion.h"
#include "Controller.h"
#include "Match.h"

#include "Debugger.h"
#include "Geometry.h"

namespace Motion
{

	Vec3 cPosition 		= {0,0,0};
	Vec3 cTarget 		= {0,0,0};
	Vec3 calibration 	= {1,1,1};

	Vec2 controlPoint   = {0,0};

	bool absolute 		= true;

	void init(){
		calibration = Settings::ROBOT == Settings::PRIMARY ?  
			Settings::Calibration::Primary.Cartesian : Settings::Calibration::Secondary.Cartesian;
		
		absolute = Settings::ABSOLUTE;
		Controller::setFeedrate(100);
	}

	Vec3 GetPosition(){
		return cPosition;
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

	void SetPosition(Vec2 newPos){
		cPosition = Vec3(newPos, cPosition.c);
	}
	void SetPosition(Vec3 newPos){
		cPosition = newPos;
	}

    void SetAbsolute(bool state){
		absolute = state;
	}

    void SetRelative(bool state){
		SetAbsolute(!state);
	}


    void turn(float angle){
		if (absolute) move({cPosition.a, cPosition.b, angle});
		else move({0, 0, angle});
	}

    void goPolar(float heading, float length){
		PolarVec target = {heading, length};
		go(target.toVec2());
	}

	void go(float x, float y){
		Vec2 target = {x, y};
		go(target);
	}

	void go(Vec2 target){
		if (absolute) move({target.a, target.b, cPosition.c});
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
		
		align(borderPos);

		SetRelative();

		Controller::setFeedrate(FAST);
		go(-borderPos.mag() + 20,.0);
		Controller::setFeedrate(SLOW);
		go(-borderPos.mag() - 10,.0);

		
		float offset = 112.61f;

		if(cPosition.a < 0.0 + offset)
			cPosition.a = 0.0 + offset;
		
		if(cPosition.a > 3000.0 - offset)
			cPosition.a = 3000.0 - offset; 

		if(cPosition.b < 0.0)
			cPosition.a = 0.0 + offset;
		
		if(cPosition.b > 2000.0)
			cPosition.a = 2000.0 - offset;
		
		go(100,.0);

		Controller::setFeedrate(FAST);
		SetAbsolute(tAbsolute);
	}

	void align(Vec2 coord){
		boolean tAbsolute = isAbsolute();

		SetAbsolute(tAbsolute);
		turn(coord.heading()-180.0);

		SetAbsolute(tAbsolute);
	}

	//Raw move request
	void move(Vec3 target){
		if(absolute){
			target.sub(cPosition);
		}
		cTarget = target;
		target.mult(calibration.toMatrix()); //Apply calibration (X,Y,ROT)
		target = ik(target); //Apply inverse kinematics (X,Y,ROT) -> (Va, Vb, Vc)
		target.mult(Settings::Stepper::STEP_MODE * RAD_TO_DEG);

		Controller::move(target, true);
		while(running()) Match::update();
		cPosition.add(cTarget);
	}


	bool running(){
		return Controller::isRunning();
	}

	/*
	void SetControlPoint(Vec2 point){
		//What is going on when robot is moving ??
		controlPoint = point;
		cPosition.a += point.a;
		cPosition.b += point.b;
	}
	*/

	Vec3 ik(Vec3 target){
		float c60 = cosf(PI/3.0f),
			  s60 = sinf(PI/3.0f),
			  L = Settings::RADIUS,
			  R = Settings::WHEEL_RADIUS;

		
		target.c *= DEG_TO_RAD;
		target.rotateZ(-cPosition.c * DEG_TO_RAD);

		Matrix3x3 P = {
			   0,   1 , L,
			-s60, -c60, L,
			 s60, -c60, L
		};

		return target.mult(P).mult(1/R);
	}

}
