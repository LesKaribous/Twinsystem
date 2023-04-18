#pragma once
#include "core/Core.h"
#include "event/Event.h"
#include "ui/Screen.h"
#include "inputs/Inputs.h"
#include "inputs/Fields.h"

namespace TwinSystem{

    enum class Page{
        START,
        INIT,
        MATCH,
        RESET
    };

    class UI{
    public:
        UI();

        void Initialize();
        void Clear();
        void Update();
        void Draw();

        void SetPage(Page p);
        //bool pollEvents(void(System::*cb)(&Event));
        void OnEvent(Event& e);

    private:
        // Fonctions d'affichage
        void drawBackScreenStart();
        void drawBackScreenMatch();
        void updateScore(int uScore);
        void updateMatchTime(int tMatch);
        void updatePosition(float Xpos, float Ypos, float Tpos);
        void updateLidarState(bool lidarState);
        void updateTiretteState(bool tiretteState);
        void updateStrategyState(bool stratState);
        void updateAllMatchVar();
        void updateInitState(int initState);
        void updateTeamColor(bool team);
        void updateAllStartVar();


    public:
        Inputs inputs;
        Fields fields;
        Screen screen;

    private:
        Page currentPage = Page::INIT;

        unsigned long lastDraw = 0;
        unsigned long lastPosDraw = 0;

        bool needDraw = true;
    };
} // namespace TwinSystem
