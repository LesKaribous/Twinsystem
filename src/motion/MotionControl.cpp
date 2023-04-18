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
        _state = JobState::IDLE;

        _startPosition  = {0,0,0};
        _position       = {-1,-1,0};
        _target 	     = {0,0,0};

        _calibration 	= Settings::Calibration::Primary.Cartesian;
        _controlPoint   = {0,0};
		_absolute = true;
	}

	void MotionControl::OnEvent(Event& e){
		//EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		//dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));
		steppers.OnEvent(e);
	}

    void MotionControl::Pause(){
		JobExecutor::Pause();
		steppers.Pause();
	}

    void MotionControl::Resume(){
		JobExecutor::Resume();
		steppers.Resume();
	}

    void MotionControl::Cancel() {
		JobExecutor::Cancel();
		steppers.Cancel();
	}

    void MotionControl::Finish() {
		JobExecutor::Finish();
		
		//Need to take actual rotation when adding relative target
		//_position = _startPosition.add(_target.rotateZ(-_position.c));
		_position = _target;
		_startPosition = _position;
	}

	
	void  MotionControl::Run(){
		Console::trace("MotionControl") << JobExecutor::ToString() << Console::endl;
		steppers.Run();
		if(_state == JobState::RUNNING){
			if(!steppers.IsBusy()){
				_state = JobState::FINISHED;

				Finish();
			}
		}
		Console::trace("MotionControl") << JobExecutor::ToString() << Console::endl;
	}

	void  MotionControl::GoAwait(float x, float y){
		Vec2 target = {x, y};
		GoAwait(target);
	}

	void  MotionControl::GoAsync(float x, float y){
		Vec2 target = {x, y};
		GoAsync(target);
	}

    void  MotionControl::TurnAwait(float angle){
		Console::info("MotionControl") << "Turn :" << angle << Console::endl;

		steppers.SetFeedrate(50);
		if (_absolute) MoveAwait({_position.a, _position.b, angle });
		else MoveAwait({0, 0, angle});
	}

	void  MotionControl::TurnAsync(float angle){
		Console::info("MotionControl") << "Turn :" << angle << Console::endl;

		steppers.SetFeedrate(50);
		if (_absolute) MoveAsync({_position.a, _position.b, angle });
		else MoveAsync({0, 0, angle});
	}

	void  MotionControl::GoAwait(Vec2 target){
		steppers.SetFeedrate(100);
		Console::info("MotionControl") << "Go :" << target << Console::endl;
		if (_absolute) MoveAwait({target.a, target.b, _position.c*RAD_TO_DEG});
		else MoveAwait({target.a, target.b, 0});
	}

	void  MotionControl::GoAsync(Vec2 target){
		steppers.SetFeedrate(100);
		Console::info("MotionControl") << "Go :" << target << Console::endl;
		if (_absolute) MoveAsync({target.a, target.b, _position.c*RAD_TO_DEG});
		else MoveAsync({target.a, target.b, 0});
	}

	void  MotionControl::AlignAwait(RobotCompass rc, float orientation){
		Console::info("MotionControl") << "Align :" << Console::endl;
		TurnAwait((orientation - GetCompassOrientation(rc)));
	}
	void  MotionControl::AlignAsync(RobotCompass rc, float orientation){
		Console::info("MotionControl") << "Align :" << Console::endl;
		TurnAsync( orientation - GetCompassOrientation(rc));
	}


	//Raw relative move request
	void  MotionControl::MoveAwait(Vec3 target){
		//Set new target
		target.c *= DEG_TO_RAD;
		
		if(!_absolute){
			_target = ToAbsoluteTarget(target);
		}else{
			_target = target;
		}

		OptmizeTarget();
		Vec3 relativeTarget = ToRelativeTarget(target);
		
		Console::info("MotionControl") << "Current position : " << GetAbsPosition() << Console::endl;
		Console::info("MotionControl") << "Current target   : " << GetAbsTarget() << Console::endl;
		Console::info("MotionControl") << "Current relative target : " << relativeTarget << Console::endl;
		
		_state = JobState::RUNNING; //robot is moving from now

		steppers.DisableAsync();
		steppers.Move(TargetToSteps(relativeTarget)); //Execute move await
		Finish();
	}

	//Raw relative move request
	void  MotionControl::MoveAsync(Vec3 target){
		//Set new target
		target.c *= DEG_TO_RAD;
		if(!_absolute){
			_target = ToAbsoluteTarget(target);
		}else{
			_target = target;
		}

		OptmizeTarget();
		_relTarget  = ToRelativeTarget(_target);
		
		Console::info("MotionControl") << "Current position : " << GetAbsPosition() << Console::endl;
		Console::info("MotionControl") << "Current target   : " << GetAbsTarget() << Console::endl;
		Console::info("MotionControl") << "Current relative target : " << _relTarget << Console::endl;
		
		_state = JobState::RUNNING; //robot is moving from now

		steppers.EnableAsync();
		steppers.Move(TargetToSteps(_relTarget)); //Execute move await
	}

	void MotionControl::OptmizeTarget(){
		while(_target.c > PI) _target.c -= 2.0f*PI;
		while(_target.c <= -PI) _target.c += 2.0f*PI;
	}

	Vec3 MotionControl::TargetToSteps(Vec3 relTarget){
		relTarget.mult(_calibration.toMatrix()); 					//Apply cartesian calibration
		relTarget = ik(relTarget);				  					//Apply inverse kinematics result in steps
		relTarget.mult(Settings::Stepper::STEP_MODE * RAD_TO_DEG); 	//Apply stepping multiplier
		return relTarget;
	}
	
	Vec3 MotionControl::ToRelativeTarget(Vec3 absTarget){
		absTarget.sub(_position); 		 //Convert Absolute target to relative
		absTarget.rotateZ(_position.c);
		return absTarget;
	}

	Vec3 MotionControl::ToAbsoluteTarget(Vec3 relTarget){
		relTarget.rotateZ(-_position.c);
		relTarget.add(_position); 		 //Convert  target relativeto Absolute
		return relTarget;
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


		_position = _startPosition.copy().add(relativePosition);
		Console::trace("MotionControl") << "Current position" << _position << Console::endl;
	}




	// -------------------------------
	// ----- Setters and Getters -----
	// -------------------------------

	Vec3 MotionControl::GetAbsPosition() const{
		return _position;
	}

	Vec3  MotionControl::GetAbsTarget() const{
		return _target;
	}

	bool  MotionControl::IsAbsolute() const{
		return _absolute;
	}

	bool  MotionControl::IsRelative() const{
		return !_absolute;
	}

	void  MotionControl::SetAbsPosition(Vec3 newPos){
		_position = newPos; 
	}

	void  MotionControl::SetAbsTarget(Vec3 newTarget){
		_target = newTarget;
	}


    void  MotionControl::SetAbsolute(){
		_absolute = true;
	}

    void  MotionControl::SetRelative(){
		_absolute = false;
	}





}
