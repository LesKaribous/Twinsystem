#pragma once

#include "ui/UI.h"
#include "core/Core.h"
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
		void UpdateUI();

		void OnEvent(Event& e);
		void Execute(Program prgm);

		void Wait(int time);

		inline const RobotState& GetState(){return _state;}

	protected:
		Actuators actuators;
		MotionControl motion;
		Intercom intercom;
		UI ui;

		RobotState _state;
		bool pollEvents();

	private:
		bool _initialized = false;
	};
} // namespace System