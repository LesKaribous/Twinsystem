#pragma once
#include "core/Core.h"
#include "core/Event.h"
#include "ui/ProgressBar.h"

namespace TwinSystem{
    class UI{
    public:
        UI();

        void OnEvent(Event& e);

    private:
        ProgressBar prgBar;

    };
} // namespace TwinSystem
