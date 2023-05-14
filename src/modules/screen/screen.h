#pragma once
#include "core/lib.h"
#include "system.h"
#include "core/module.h"
#include "modules/screen/fields.h"

enum class Page{
    START,
    INIT,
    MATCH,
    RESET
};

#pragma once


typedef ILI9341_t3n TFTScreen;

class Screen : public Module{
public:
    Screen(System&);
    ~Screen();

    void update() override;

    void clear();
    void draw();
    void setPage(Page p);

    friend class System;

private:

    System& system;

    // Fonctions d'affichage
    void updateAllMatchVar();
    void updateAllStartVar();

    void drawBackScreenStart();
    void drawBackScreenMatch();
    void updateScore(int uScore);
    void updateMatchTime(int tMatch);
    void updatePosition(float Xpos, float Ypos, float Tpos);
    void updateLidarState(bool lidarState);
    void updateTiretteState(bool tiretteState);
    void updateStrategyState(bool stratState);
    void updateInitState();
    void updateTeamColor(bool team);
    
    
    TFTScreen screen;

    //Fields
    FloatingField
    x, y, z;

    DigitalField
    score,
    time;

    BooleanField
    intercom,
    probing,
    probed,
    armed,
    started;

    Page currentPage = Page::INIT;

    unsigned long lastDraw = 0;
    unsigned long lastPosDraw = 0;
    bool needDraw = true;
};