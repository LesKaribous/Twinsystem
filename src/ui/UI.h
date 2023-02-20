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

        bool pollEvents(std::function<void(Event&)>);
        void OnEvent(Event& e);

    private:
        Screen screen;
        Inputs inputs;
        
        bool needDraw;
    };
} // namespace TwinSystem
