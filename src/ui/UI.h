#pragma once
#include "core/Core.h"
#include "event/Event.h"
#include "ui/Screen.h"
#include "inputs/Inputs.h"

namespace TwinSystem{
    class UI{
    public:
        UI();
        void clear();
        void draw();

        //bool pollEvents(void(System::*cb)(&Event));
        void OnEvent(Event& e);


        Inputs inputs;
    private:
        Screen screen;
        
        long lastDraw = 0;
        bool needDraw = true;
    };
} // namespace TwinSystem
