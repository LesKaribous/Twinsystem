#pragma once
#include "services/service.h"
#include "services/ihm/fields.h"
#include "services/ihm/button.h"
#include "settings.h"
#include "utils/geometry.h"
#include "pin.h"

#ifdef TEENSY35
#include <ILI9341_t3n.h>
typedef ILI9341_t3n TFTScreen;
#endif
#ifdef TEENSY41
#include <ILI9341_t3.h>
typedef ILI9341_t3 TFTScreen;
#endif


class IHM : public Service{
    IHM();
    SINGLETON(IHM)
public:
    enum class Page{
        BOOT,
        INIT,
        MATCH,
        RESET
    };

    void attach() override;
    void run() override;
   
    void setPage(Page p);
    void drawBootProgress(String msg);//%
    void addBootProgress(int v);//%
    void setRobotPosition(Vec3 p);

    void freezeSettings();
    void unfreezeSettings();

    #ifndef OLD_BOARD
    void playTone(const int freq, const int duration);
    void playMelody(const int* notes, const int* durations, const int len, const int tempo);
    void playStartupMelody();
    #endif

    bool hasStarter() const;
    bool buttonPressed() const;
    bool starterPulled() const;
    bool starterPlaced() const;
    bool buttonReleased() const;
    bool starterCancelled() const;

    bool isColor(Color) const;

    bool isPrimary() const;
    bool isSecondary() const;
    bool getRobotType() const;
    bool getStrategyState() const;
    void waitButtonRelease();
    void setIntercomState(bool);
    void setTime(int t);
    void addScorePoints(int t);

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

    TFTScreen screen;
    Page currentPage = Page::BOOT;

    unsigned long lastDraw = 0;
    unsigned long lastPosDraw = 0;
    bool needDraw = true;

    int bootProgress = 0;

public:
    Switch starter;
    Switch teamSwitch;
    Switch twinSwitch;
    Switch strategySwitch;
    Button resetButton;
    
};

SINGLETON_EXTERN(IHM, ihm)