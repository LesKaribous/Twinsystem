#include "motion/MotionControl.h"
#include "Twinsystem.h"
#include "debug/Console.h"

namespace TwinSystem{
	
	MotionControl::MotionControl(){
		_calibration = Settings::Match::PRIMARY ?  
		Settings::Calibration::Primary.Cartesian : Settings::Calibration::Secondary.Cartesian;
		
		_absolute = Settings::Motion::ABSOLUTE;
		steppers.SetFeedrate(100);
	}

	MotionControl::~MotionControl(){}

	void  MotionControl::Initialize(){
	   steppers.Initialize();
         

        State _state = State::IDLE;

        Vec3 _startPosition  = {0,0,0};
        Vec3 _position       = {-1,-1,0};
        Vec3 _target 	     = {0,0,0};

        Vec3 _calibration 	 = {1,1,1};
        Vec2 _controlPoint   = {0,0};

        bool _probedX = false, _probedY = false;
        bool _absolute = true;
        bool _probing = false;
	}

	void  MotionControl::DebugState(){
		switch(_state){
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
		steppers.OnEvent(e);
	}


	void  MotionControl::Pause(){
		DebugState();
		if(_state == State::RUNNING){
			DebugState();
			_state = State::PAUSED;
			DebugState();
			Console::info("Controller") << "stopped" << Console::endl;
			steppers.Stop();
		}
	}

    void  MotionControl::Resume(){
		DebugState();
		if(_state == State::PAUSED){
			DebugState();
			_state = State::RUNNING;
			DebugState();
			Console::info("Controller resumed");
			steppers.Resume();
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
		if(_state == State::RUNNING){
			if(steppers.isArrived()){
				DebugState();
				_state = State::ARRIVED;
				DebugState();
			}
		}
	}

    void  MotionControl::Turn(float angle){
		steppers.SetFeedrate(30);
		if (_absolute) Move({_position.a, _position.b, angle });
		else Move({0, 0, angle});
		steppers.SetFeedrate(100);
	}

	//All goPolar are relative
    void  MotionControl::GoPolar(float heading, float length){
		PolarVec target = PolarVec(heading*DEG_TO_RAD, length);
		bool abs = isAbsolute();
		SetRelative();
		Go(target.toVec2());
		SetAbsolute(abs);
	}

	void  MotionControl::Go(float x, float y){
		Vec2 target = {x, y};
		Go(target);
	}

	void  MotionControl::Go(Vec2 target){
		Console::info("MotionControl") << "Go :" << target << Console::endl;
		if (_absolute) Move({target.a, target.b, _position.c*RAD_TO_DEG});
		else Move({target.a, target.b, 0});
	}

	void  MotionControl::GoTurn(float x, float y, float heading){
		GoTurn({x, y, heading});
	}
	void  MotionControl::GoTurn(Vec3 target){
		Go(Vec2(target.a, target.b));
		Turn(target.c);
	}
	
	
	void  MotionControl::ProbeBorder(TableCompass tc, RobotCompass rc){
		boolean tAbsolute = isAbsolute();
		_probing = true;
		Align(rc, GetCompassOrientation(tc));

		SetRelative();
		steppers.SetFeedrate(SLOW);
		GoPolar(GetCompassOrientation(rc),200);
		steppers.SetFeedrate(10);
		GoPolar(GetCompassOrientation(rc),80);

		float _offset = GetOffsets(rc);

		if(tc == TableCompass::NORTH)
			_position.a = 3000.0 - _offset; //We hit Xmax
		if(tc == TableCompass::SOUTH)
			_position.a = 0.0 + _offset; //We hit Xmin
		if(tc == TableCompass::EAST)
			_position.b = 2000.0 - _offset; //We hit Ymax
		if(tc == TableCompass::WEST)
			_position.b = 0.0 + _offset; //We hit Ymin

		GoPolar(GetCompassOrientation(rc),-100);

		steppers.SetFeedrate(FAST);
		SetAbsolute(tAbsolute);
		_probing = false;
	}

	bool  MotionControl::isProbing(){
		return _probing;
	}


	void  MotionControl::Align(RobotCompass rc, float orientation){
		Turn(orientation - GetCompassOrientation(rc));
	}

	Vec3  MotionControl::SetTarget(Vec3 target){
		target.c *= DEG_TO_RAD;

		if(_absolute){
			target.sub(_position); 		 //Convert Absolute target to relative
			target.rotateZ(_position.c);
		}
		
		while(target.c > PI) target.c -= 2.0f*PI;
		while(target.c < -PI) target.c += 2.0f*PI;

		_target = target;
		//Intercom::focus();

		return target;
	}

	void  MotionControl::UpdatePosition(){
		Vec3 currentStepperPos = steppers.GetPosition();
		Vec3 relativePosition = fk(currentStepperPos);
		
		relativePosition.a /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;
		relativePosition.b /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;
		relativePosition.c /= Settings::Stepper::STEP_MODE * RAD_TO_DEG;

		relativePosition.a /= _calibration.a;
		relativePosition.b /= _calibration.b;
		relativePosition.c /= _calibration.c;

		Console::trace("MotionControl") << "Current position" << relativePosition << Console::endl;
		_position = _startPosition.copy().add(relativePosition);
	}

	//Raw relative move request
	void  MotionControl::Move(Vec3 target){
		//Set new target
		target = SetTarget(target);
		_startPosition = _position; //Save start position
		
		DebugState();
		_state = State::RUNNING; //robot is moving from now
		DebugState();

		target.mult(_calibration.toMatrix()); //Apply cartesian calibration
		target = ik(target);				 //Apply inverse kinematics result in steps
		target.mult(Settings::Stepper::STEP_MODE * RAD_TO_DEG); //Apply stepping multiplier

		steppers.Move(target, true); //Execute move
		while(Running()) Run(); //Wait for move to end and check for opponent
		
		//Need to take actual rotation when adding relative target
		_position = _startPosition.add(_target.rotateZ(-_position.c)); 
		_startPosition = _position;
		DebugState();
		_state = State::IDLE; //Ready for next move
		DebugState();
	}

	bool  MotionControl::Running(){
		return !(_state == State::ARRIVED || _state == State::CANCELLED);
	}




	// -------------------------------
	// ----- Setters and Getters -----
	// -------------------------------

	Vec3  MotionControl::GetPosition(){
		return Vec3(_position.a, _position.b, _position.c * RAD_TO_DEG);
	}

	Vec3  MotionControl::GetTarget(){
		return _target;
	}
	Vec3  MotionControl::GetAbsTarget(){
		return _startPosition.copy().add(_target);
	}

	bool  MotionControl::isAbsolute(){
		return _absolute;
	}

	bool  MotionControl::isRelative(){
		return !_absolute;
	}

	bool  MotionControl::isProbed(){
		return _probedX && _probedY;
	}
	bool  MotionControl::isXProbed(){
		return _probedX;
	}
	bool  MotionControl::isYProbed(){
		return _probedY;
	}

	void  MotionControl::SetPosition(Vec2 newPos){
		_position = Vec3(newPos, _position.c*RAD_TO_DEG);
	}
	void  MotionControl::SetPosition(Vec3 newPos){
		_position = newPos.mult(Vec3(1.0,1.0,DEG_TO_RAD).toMatrix());
	}

    void  MotionControl::SetAbsolute(bool state){
		_absolute = state;
	}

    void  MotionControl::SetRelative(bool state){
		SetAbsolute(!state);
	}





}
