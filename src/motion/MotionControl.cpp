#include "motion/MotionControl.h"
#include "Twinsystem.h"
#include "debug/Console.h"

namespace TwinSystem{
	
	MotionControl::MotionControl(Shared<StepperController> stps){
		controller = stps;
		calibration = Settings::primary() ?  
			Settings::Calibration::Primary.Cartesian : Settings::Calibration::Secondary.Cartesian;
		
		absolute = Settings::ABSOLUTE;
		controller->SetFeedrate(100);

	}

	MotionControl::~MotionControl(){}

	void  MotionControl::DebugState(){
		switch(cState){
			case State::IDLE :
				Console::println("IDLE");
			break;
			case State::RUNNING :
				Console::println("RUNNING");
			break;
			case State::PAUSED :
				Console::println("PAUSED");
			break;
			case State::CANCELLED :
				Console::println("CANCELLED");
			break;
			case State::ARRIVED :
				Console::println("ARRIVED");
			break;
			default:
				Console::println("NONE");
			break;
		}
		//Console::log("Collision :", Intercom::collision(), INFO);
	}

	void MotionControl::OnEvent(Event& e){
		//EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		//dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));
		controller->OnEvent(e);
	}


	void  MotionControl::Pause(){
		DebugState();
		if(cState == State::RUNNING){
			DebugState();
			cState = State::PAUSED;
			DebugState();
			Console::info("Controller") << "stopped" << Console::endl;
			controller->Stop();
		}
	}

    void  MotionControl::Resume(){
		DebugState();
		if(cState == State::PAUSED){
			DebugState();
			cState = State::RUNNING;
			DebugState();
			Console::info("Controller resumed");
			controller->Resume();
		}
	}


	
	void  MotionControl::Run(){
		//updatePosition();
		//System::update();
		/*
		if(Intercom::collision() && avoidance){
			pause();
		}else{
			resume();
		}
		*/
		if(cState == State::RUNNING){
			if(controller->isArrived()){
				DebugState();
				cState = State::ARRIVED;
				DebugState();
			}
		}
	}

    void  MotionControl::Turn(float angle){
		controller->SetFeedrate(30);
		if (absolute) Move({cPosition.a, cPosition.b, angle });
		else Move({0, 0, angle});
		controller->SetFeedrate(100);
	}

    void  MotionControl::GoPolar(float heading, float length){
		PolarVec target;
		if(Settings::yellow()) target = PolarVec(-heading*DEG_TO_RAD, length);
		if(Settings::purple()) target = PolarVec(heading*DEG_TO_RAD, length);
		Go(target.toVec2());
	}

	void  MotionControl::Go(float x, float y){
		Vec2 target = {x, y};
		Go(target);
	}

	void  MotionControl::Go(Vec2 target){
		Console::info("MotionControl") << "Go :" << target << Console::endl;
		if (absolute) Move({target.a, target.b, cPosition.c*RAD_TO_DEG});
		else Move({target.a, target.b, 0});
	}

	void  MotionControl::GoTurn(float x, float y, float heading){
		GoTurn({x, y, heading});
	}
	void  MotionControl::GoTurn(Vec3 target){
		Go(Vec2(target.a, target.b));
		Turn(target.c);
	}
	
	
	void  MotionControl::ProbeBorder(Vec2 borderPos){
		boolean tAbsolute = isAbsolute();
		probing = true;
		Align(borderPos, 180);

		SetRelative();
		controller->SetFeedrate(FAST);
		Go(-borderPos.mag() - 80,.0);
		controller->SetFeedrate(SLOW);
		Go(-120,.0);

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
		
		Go(100,.0);

		controller->SetFeedrate(FAST);
		SetAbsolute(tAbsolute);
		probing = false;
	}

	bool  MotionControl::isProbing(){
		return probing;
	}


	void  MotionControl::Align(float angleTable, float orientation){
		boolean tAbsolute = isAbsolute();
		SetAbsolute();
		Turn(angleTable - orientation);
		SetAbsolute(tAbsolute);
	}

	void  MotionControl::Align(Vec2 coord, float orientation){
		boolean tAbsolute = isAbsolute();
		SetAbsolute();
		Turn(coord.heading()*RAD_TO_DEG - orientation);
		SetAbsolute(tAbsolute);
	}

	Vec3  MotionControl::SetTarget(Vec3 target){
		target.c *= DEG_TO_RAD;

		if(absolute){
			target.sub(cPosition); 		 //Convert Absolute target to relative
			target.rotateZ(cPosition.c);
		}
		
		while(target.c > PI) target.c -= 2.0f*PI;
		while(target.c < -PI) target.c += 2.0f*PI;

		cTarget = target;
		
		
		//Intercom::focus();

		return target;
	}

	void  MotionControl::UpdatePosition(){
		Vec3 currentStepperPos = controller->GetPosition();
		Vec3 relativePosition = fk(currentStepperPos);
		
		relativePosition.a /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;
		relativePosition.b /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;
		relativePosition.c /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;

		relativePosition.a /= calibration.a;
		relativePosition.b /= calibration.b;
		relativePosition.c /= calibration.c;

		Console::trace("MotionControl") << "Current position" << relativePosition << Console::endl;
		cPosition = cStartPosition.copy().add(relativePosition);
	}

	//Raw relative move request
	void  MotionControl::Move(Vec3 target){
		//Set new target
		target = SetTarget(target);
		cStartPosition = cPosition; //Save start position
		
		DebugState();
		cState = State::RUNNING; //robot is moving from now
		DebugState();

		target.mult(calibration.toMatrix()); //Apply cartesian calibration
		target = ik(target);				 //Apply inverse kinematics result in steps
		target.mult(Settings::Stepper::STEP_MODE * RAD_TO_DEG); //Apply stepping multiplier

		controller->Move(target, true); //Execute move
		while(Running()) Run(); //Wait for move to end and check for opponent
		
		//Need to take actual rotation when adding relative target
		cPosition = cStartPosition.add(cTarget.rotateZ(-cPosition.c)); 
		cStartPosition = cPosition;
		DebugState();
		cState = State::IDLE; //Ready for next move
		DebugState();
	}

	bool  MotionControl::Running(){
		return !(cState == State::ARRIVED || cState == State::CANCELLED);
	}




	// -------------------------------
	// ----- Setters and Getters -----
	// -------------------------------

	Vec3  MotionControl::GetPosition(){
		return Vec3(cPosition.a, cPosition.b, cPosition.c * RAD_TO_DEG);
	}

	Vec3  MotionControl::GetTarget(){
		return cTarget;
	}
	Vec3  MotionControl::GetAbsTarget(){
		return cStartPosition.copy().add(cTarget);
	}

	bool  MotionControl::isAbsolute(){
		return absolute;
	}

	bool  MotionControl::isRelative(){
		return !absolute;
	}

	bool  MotionControl::isProbed(){
		return probedX && probedY;
	}
	bool  MotionControl::isXProbed(){
		return probedX;
	}
	bool  MotionControl::isYProbed(){
		return probedY;
	}

	void  MotionControl::SetPosition(Vec2 newPos){
		cPosition = Vec3(newPos, cPosition.c*RAD_TO_DEG);
	}
	void  MotionControl::SetPosition(Vec3 newPos){
		cPosition = newPos.mult(Vec3(1.0,1.0,DEG_TO_RAD).toMatrix());
	}

    void  MotionControl::SetAbsolute(bool state){
		absolute = state;
	}

    void  MotionControl::SetRelative(bool state){
		SetAbsolute(!state);
	}

	
    void  MotionControl::SetAvoidance(bool state){
		if(Settings::avoidance())
			avoidance = state;
		else avoidance = false;
	}



}
