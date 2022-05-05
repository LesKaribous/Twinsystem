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
	bool absolute 		= true;

	void init(){
		calibration = Settings::ROBOT == Settings::PRIMARY ?  
			Settings::Calibration::Primary.Cartesian : Settings::Calibration::Secondary.Cartesian;
		
		absolute = Settings::ABSOLUTE;
	}

	Vec3 GetPosition(){
		return cPosition;
	}

	Vec3 GetTarget(){
		return cTarget;
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
		go({x, y});
	}

	void go(Vec2 target){
		if (absolute) move({target.a, target.b, cPosition.c});
		else move({target.a, target.b, 0});
	}

	void go(float x, float y, float heading){
		go({x, y, heading});
	}
	void go(Vec3 target){
		move({target.a, target.b, target.c});
	}


	//Raw move request
	void move(Vec3 target){
		if(absolute) target.sub(cPosition);
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
	
	Vec3 ik(Vec3 target){
		float c60 = cosf(PI/3.0f),
			  s60 = sinf(PI/3.0f),
			  L = Settings::RADIUS,
			  R = Settings::WHEEL_RADIUS;

		target.c *= DEG_TO_RAD;

		Matrix3x3 P = {
			   0,   1 , L,
			-s60, -c60, L,
			 s60, -c60, L
		};
				
		return target.mult(P).mult(1/R);
	}

}
