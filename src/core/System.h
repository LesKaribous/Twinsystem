#pragma once

#include "core/Core.h"
#include "event/Event.h"
#include "core/Program.h"
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

    protected:
      void Initialize(); //Should be called once in the constructor
      bool pollEvents();
		  static System* s_Instance;

      Shared<MotionControl> motion;
      Shared<StepperController> stepper;
      Shared<UI> ui;

    };
} // namespace System