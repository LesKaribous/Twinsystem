#pragma once
#include "core/Core.h"
#include "core/Event.h"
#include "ui/ProgressBar.h"


namespace TwinSystem{



    class UI{
    public:
        UI();

        void init();
        void draw();

        void OnEvent(Event& e);

        inline ProgressBar& GetProgressBar(){return prgBar;};

    private:
        Shared<Screen> screen;
        ProgressBar prgBar;

        bool needDraw;
    };
} // namespace TwinSystem
