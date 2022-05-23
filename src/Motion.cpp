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
	Vec3 cStartPosition = {0,0,0};
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
		/*
		if(Intercom::collision()){
			if(Controller::isRunning()){
				Controller::stop();
			}
		}else{
			if(!Controller::isRunning()){
				Controller::resume(); 
			}
		}*/

		Motion::updatePosition();
		Match::update();
		Intercom::checkSerial();
		Debugger::checkSerial();
		Intercom::focus();
		
	}
	
	Vec3 GetPosition(){
		return Vec3(cPosition.a, cPosition.b, cPosition.c * RAD_TO_DEG);
	}

	Vec3 GetTarget(){
		return cTarget;
	}
	Vec3 GetAbsTarget(){
		return cStartPosition.copy().add(cTarget);
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
		
		align(borderPos, 180);

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

	//Do not use yet
	void probeBorder(Vec2 borderPos, float orientation){
		boolean tAbsolute = isAbsolute();
		probing = true;
		
		align(borderPos, orientation); //Align back of the robot (HMI side) with the desired vector

		SetAbsolute();
		Controller::setFeedrate(FAST);
		Vec2 origin = GetPosition();

		Vec2 approach = borderPos;
		approach.mult(0.8);   	//Relative approach
		approach.add(origin); 	//Aboslute approach point
		go(approach);

		Vec2 dest = borderPos;
		dest.mult(1.5);			//Relative destination (50% futher than the border)
		dest.add(origin); 	//Aboslute destination point

		Controller::setFeedrate(SLOW);
		go(dest);

		//Calulate new position
		Vec3 newPos = GetPosition();
		Vec2 relativeOrigin = origin;
		//relativeOrigin.sub(newPos);

		float _offset = Settings::Geometry::offset ; //TODO : Adapt offset to the side of the robot

		if(newPos.a < 0.0 + _offset){
			newPos.a = 0.0 + _offset;
			newPos.c = PI + orientation*DEG_TO_RAD;
			probedX = true;
		}else if(newPos.a > 3000.0 - _offset){
			newPos.a = 3000.0 - _offset; 
		}else if(newPos.b < 0.0){
			newPos.b = 0.0 + _offset;
			probedY = true;
		}else if(newPos.b > 2000.0){
			newPos.b = 2000.0 - _offset;
		}
		
		
		go(origin); //return to last known position
		//SetPosition(newPos.add(relativeOrigin));//Set new Position
		
		Controller::setFeedrate(FAST);
		SetAbsolute(tAbsolute);
		probing = false;
	}

	bool isProbing(){
		return probing;
	}

	void align(Vec2 coord, float orientation){
		boolean tAbsolute = isAbsolute();
		SetAbsolute();
		turn(coord.heading()*RAD_TO_DEG - orientation);
		SetAbsolute(tAbsolute);
	}

	Vec3 SetTarget(Vec3 target){
		target.c *= DEG_TO_RAD;

		if(absolute){
			target.sub(cPosition); 		 //Convert Absolute target to relative
			target.rotateZ(cPosition.c);
		}
		
		while(target.c > PI) target.c -= 2.0f*PI;
		while(target.c < -PI) target.c += 2.0f*PI;
		cTarget = target;
		
		Intercom::focus();

		return target;
	}

	void updatePosition(){
		Vec3 currentStepperPos = Controller::getPosition();
		Vec3 relativePosition = fk(currentStepperPos);
		
		relativePosition.a /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;
		relativePosition.b /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;
		relativePosition.c /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;

		relativePosition.a /= calibration.a;
		relativePosition.b /= calibration.b;
		relativePosition.c /= calibration.c;

		Debugger::log("Current position", relativePosition, VERBOSE);
		cPosition = cStartPosition.copy().add(relativePosition);
	}

	//Raw relative move request
	void move(Vec3 target){
		target = SetTarget(target);
		cStartPosition = cPosition;



		target.mult(calibration.toMatrix()); //Apply calibration (X mm,Y mm,ROT rad)
		target = ik(target); //Apply inverse kinematics (X mm,Y mm,ROT rad) -> (Va, Vb, Vc) : steps
		target.mult(Settings::Stepper::STEP_MODE * RAD_TO_DEG);

		Controller::move(target, true);
		while(running()) computeSync();

		cPosition = cStartPosition.add(cTarget.rotateZ(-cPosition.c)); //Need to take actual rotation when adding relative target
		cStartPosition = cPosition;
	}

	bool running(){
		Controller::arrived();
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

	Vec3 fk(Vec3 target){
		float f1sq3 = 1.0f / sqrt(3.0f),		  
			  f1s3 = 1.0f/3.0f,
			  f2s3 = 1.0f/3.0f,
			  L = Settings::RADIUS,
			  R = Settings::WHEEL_RADIUS;
		

		Matrix3x3 P = {
			   0, -f1sq3, f1sq3,
		   -f2s3,   f1s3 , f1s3,
			f1s3/L, f1s3/L, f1s3/L
		};
		P.mult(R);
		return target.mult(P);
	}

}
