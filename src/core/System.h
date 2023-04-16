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



namespace TwinSystem{

	enum class RobotState{
		IDLE,
		ARMED,
		STARTING,
		MOVING,
		ROTATING,
		STOPPED,
		STUCK,
		ERROR

	};

	class System{
	public :
		System();

		void Initialize();
		void Update();

		void OnEvent(Event& e);
		void Execute(Program prgm);

		void Wait(int time);
		void WaitUntil(JobExecutor& obj);

        void Turn(float);
		void GoTurn(Vec3);
        void Go(float x, float y);
        void GoTurn(float x, float y, float w);
        void GoPolar(float heading, float length);
        void Align(RobotCompass, float orientation);

		virtual void PollEvents();
		inline const RobotState& GetState(){return _state;}

	protected :
		RobotState _state;
		Intercom intercom;
		MotionControl motion;
		Actuators actuators;
		UI ui;
		
	};
} // namespace System