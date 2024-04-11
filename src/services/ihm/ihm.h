#pragma once
#include "services/service.h"
#include "services/ihm/fields.h"
#include "services/ihm/button.h"
#include "utils/geometry.h"
#include "pin.h"
#include <ILI9341_t3n.h>

typedef ILI9341_t3n TFTScreen;

class IHM : public Service{
    IHM();
    SERVICE(IHM)
public:
    enum class Page{
        BOOT,
        INIT,
        MATCH,
        RESET
    };

    void onAttach()override;
    void onUpdate()override;
   
    void setPage(Page p);
    void drawBootProgress(String msg);//%
    void addBootProgress(int v);//%
    void setRobotPosition(Vec3 p);

    void freezeSettings();
    void unfreezeSettings();

    bool hasStarter() const;
    bool buttonPressed() const;
    bool starterPulled() const;
    bool starterPlaced() const;
    bool buttonReleased() const;
    bool starterCancelled() const;
    bool isColorBlue() const;
    bool isColorYellow() const;
    bool isPrimary() const;
    bool isSecondary() const;
    bool getRobotType() const;
    bool getStrategyState() const;
    void waitButtonRelease();
    void setIntercomState(bool);
    
private:
    void clear();
    void draw();

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
            

private:
    //Public fields
    FloatingField
    x, y, z;

    DigitalField score;
    DigitalField time;

    BooleanField intercom;
    BooleanField probing;
    BooleanField probed;
    BooleanField armed;
    BooleanField started;

    Switch starter;
    Switch teamSwitch;
    Switch twinSwitch;
    Switch strategySwitch;
    Button resetButton;
    
    TFTScreen screen;
    Page currentPage = Page::BOOT;

    unsigned long lastDraw = 0;
    unsigned long lastPosDraw = 0;
    bool needDraw = true;

    int bootProgress = 0;
};

EXTERN_DECLARATION(IHM, ihm)