#include "Motion.h"
#include "Geometry.h"

#include "Twinsystem.h"
#include "Debugger.h"

namespace Motion
{

	State cState = State::IDLE;

	Vec3 cPosition 		= {0,0,0};
	Vec3 cStartPosition = {0,0,0};
	Vec3 cTarget 		= {0,0,0};
	Vec3 calibration 	= {1,1,1};

	Vec2 controlPoint   = {0,0};

 	bool probedX = false, probedY = false;
	bool absolute 		= true;
	bool probing = false;

	bool avoidance = false;

	void init(){
		calibration = Settings::ROBOT == Settings::PRIMARY ?  
			Settings::Calibration::Primary.Cartesian : Settings::Calibration::Secondary.Cartesian;
		
		absolute = Settings::ABSOLUTE;
		Controller::setFeedrate(100);
	}

	void debugState(){
		switch(cState){
			case State::IDLE :
				Debugger::log("IDLE", INFO);
			break;
			case State::RUNNING :
				Debugger::log("RUNNING", INFO);
			break;
			case State::PAUSED :
				Debugger::log("PAUSED", INFO);
			break;
			case State::CANCELLED :
				Debugger::log("CANCELLED", INFO);
			break;
			case State::ARRIVED :
				Debugger::log("ARRIVED", INFO);
			break;
			default:
				Debugger::log("NONE", INFO);
			break;
		}
		Debugger::log("Collision :", Intercom::collision(), INFO);
	}

	void pause(){
		debugState();
		if(cState == State::RUNNING){
			debugState();
			cState = State::PAUSED;
			debugState();
			Debugger::log("Controller stopped", INFO);
			Controller::stop();
			Debugger::log("Hoo stopped", ERROR);
		}
	}

    void resume(){
		debugState();
		if(cState == State::PAUSED){
			debugState();
			cState = State::RUNNING;
			debugState();
			Debugger::log("Controller resumed", INFO);
			Controller::resume();
		}
	}


	
	void update(){
		//Motion::updatePosition();
		System::update();

		if(Intercom::collision() && avoidance){
			pause();
		}else{
			resume();
		}

		if(cState == State::RUNNING){
			if(Controller::arrived()){
				debugState();
				cState = State::ARRIVED;
				debugState();
			}
		}
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
		Debugger::log("Hello There 2", ERROR);
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
		//Set new target
		target = SetTarget(target);
		cStartPosition = cPosition; //Save start position
		
		debugState();
		cState = State::RUNNING; //robot is moving from now
		debugState();

		target.mult(calibration.toMatrix()); //Apply cartesian calibration
		target = ik(target);				 //Apply inverse kinematics result in steps
		target.mult(Settings::Stepper::STEP_MODE * RAD_TO_DEG); //Apply stepping multiplier

		Controller::move(target, true); //Execute move
		while(running()) update(); //Wait for move to end and check for opponent
		
		//Need to take actual rotation when adding relative target
		cPosition = cStartPosition.add(cTarget.rotateZ(-cPosition.c)); 
		cStartPosition = cPosition;
		debugState();
		cState = State::IDLE; //Ready for next move
		debugState();
	}

	bool running(){
		return !(cState == State::ARRIVED || cState == State::CANCELLED);
	}




	// -------------------------------
	// ----- Setters and Getters -----
	// -------------------------------

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

	
    void SetAvoidance(bool state){
		if(Settings::AVOIDANCE == Settings::ADVERSAIRE_OUI)
			avoidance = state;
		else avoidance = false;
	}



}
