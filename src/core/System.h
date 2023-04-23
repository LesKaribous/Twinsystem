#pragma once

#include "ui/UI.h"
#include "core/Core.h"
#include "core/JobExecutor.h"

#include "event/ValueTracker.h"
#include "event/Event.h"
#include "core/Program.h"
#include "com/Intercom.h"
#include "debug/Console.h"
#include "actuators/Actuators.h"
#include "motion/MotionControl.h"

void OnDummyRequestResponse(String answer);

namespace TwinSystem{

	enum class RobotState{
		IDLE,
		ARMED,
		STARTING,
		STARTED,
		FINISHING,
		FINISHED
	};

	class System{
	public :
		virtual void Initialize();
		virtual void Update();

		void OnEvent(Event& e);
		void Execute(Program prgm);

		void Wait(int time);
		void WaitUntil(JobExecutor& obj);
		
		virtual void PollEvents();
		inline const RobotState& GetState(){return _state;}

	protected :
		System();
		RobotState _state;
		Intercom intercom;
		MotionControl motion;
		Actuators actuators;
		UI ui;
		
	};
} // namespace System