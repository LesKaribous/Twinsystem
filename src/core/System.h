#pragma once

#include "core/Core.h"
#include "event/Event.h"
#include "core/Program.h"
#include "actuators/Actuators.h"
#include "ui/UI.h"
#include "debug/Console.h"
#include "motion/MotionControl.h"

namespace TwinSystem{
    class System{
    public :
      System();

      void Run();

      void OnEvent(Event& e);
      void Execute(Program prgm);

      void wait(int time);

      inline static System& GetInstance() { return *s_Instance; }

      Shared<Actuators> actuators;
      Shared<MotionControl> motion;
      Shared<StepperController> stepper;
      UI ui;

    protected:
      void Initialize(); //Should be called once in the constructor
      bool pollEvents();
		  static System* s_Instance;



    };
} // namespace System