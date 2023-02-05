#pragma once

#include "core/Core.h"
#include "core/Event.h"
#include "core/Program.h"
#include "debug/Console.h"
#include "ui/UI.h"

namespace TwinSystem{
    class System{
    public :
      System();

      void Run();
      void Initialize();


      void OnEvent(Event& e);
      void Execute(Program prgm);

      void wait(int time);

      inline static System& GetInstance() { return *s_Instance; }

    private:
      bool pollEvents();
		  static System* s_Instance;

      UI ui;

    };
} // namespace System