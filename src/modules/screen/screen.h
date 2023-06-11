#pragma once
#include "modules/module.h"
#include "system/core/lib.h"
#include "modules/screen/fields.h"

enum class Page{
    BOOT,
    INIT,
    MATCH,
    RESET
};

class Screen;
typedef ILI9341_t3n TFTScreen;

class System;

class Screen : public Module{
public:
    friend class SystemApplication;

    Screen();
    ~Screen();

    void update() override;

    void clear();
    void draw();
    void setPage(Page p);

    
    void drawBootProgress(int p, String msg);//%

private:

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

    BooleanField
    starter,
    teamSwitch,
    twinSwitch,
    resetButton,
    strategySwitch;

    Page currentPage = Page::BOOT;

    unsigned long lastDraw = 0;
    unsigned long lastPosDraw = 0;
    bool needDraw = true;
};