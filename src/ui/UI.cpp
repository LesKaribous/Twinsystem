#include "UI.h"
#include "Pin.h"
#include "core/System.h"

namespace TwinSystem{

    using namespace Pin;

    UI::UI() : screen(screen::CS, screen::DC, screen::RST, screen::MOSI, screen::SCK, screen::MISO){}
    
    void UI::Initialize(){
        inputs.Initialize();
        screen.begin();
        Clear();
        needDraw = true;
    }
    
    void UI::Update(){
        inputs.Update();
    }

    /*
    bool UI::pollEvents(std::function<void(Event&)> cb){
        inputs.pollEvents(cb);
    }
    */
    void UI::OnEvent(Event& e){
        if(e.IsInCategory(EventCategory::EventCategoryUI)){
            needDraw = true;
        }
    }
    
    void UI::Clear(){
        screen.fillScreen(ILI9341_BLACK);
        screen.setTextSize(2);
    }

    void UI::Draw(){
        switch (currentPage)
        {
        case Page::INIT :
            /* code */
            break;
        
        default:
            break;
        }
    }


    void UI::drawBackScreenStart() {
        screen.fillScreen(ILI9341_BLACK);
        screen.setTextColor(ILI9341_ORANGE);
        screen.setTextSize(2);
        // Texte "Etats"
        screen.setCursor(10,10);
        screen.println("Strat : ");
        screen.setCursor(10,30);
        screen.println("    X :        mm");
        screen.setCursor(10,50);
        screen.println("    Y :        mm");
        screen.setCursor(10,70);
        screen.println("Theta :        deg");
        screen.setCursor(10,90);
        screen.println("Lidar : ");
        screen.setCursor(10,110);
        screen.println(" Init : ");
        screen.setCursor(10,130);
        screen.println("Tirette ");
        screen.drawFastHLine(10, 155, 220, ILI9341_WHITE);
        screen.setTextSize(4);
        screen.setCursor(10,170);
        screen.println("Team");
        screen.setTextSize(1);
        screen.setCursor(10,300);
        screen.print("Upload : ");
        screen.println(__TIMESTAMP__);
    }

    void UI::updateAllStartVar() {
        if(firstUpdate){
            updateStrategyState(getStrategy());
            updateTeamColor(getTeamColor());
            updateLidarState(getLidarState());
            updateTiretteState(getTiretteState());
            updateInitState(getInitState());
            updatePosition(0.0, 0.0, 0.0);
            firstUpdate = false;
        } 
        else 
        {
            if(inputs.strategySwitch.hasChanged()) updateStrategyState(inputs.strategySwitch.GetState());
            if(inputs.teamSwitch.hasChanged()) updateTeamColor(inputs.teamSwitch.GetState());
            if(lidarHasChanged()) updateLidarState(getLidarState());
            if(tiretteHasChanged()) updateTiretteState(getTiretteState());
            if(initHasChanged()) updateInitState(getInitState());
            //
        }
    }

    void UI::updateTeamColor(bool team){
        screen.fillRect(10, 200, 220, 98, ILI9341_BLACK);
        screen.setTextSize(4);
        screen.setCursor(60,230);
        screen.setTextColor(ILI9341_WHITE);
        if(team)
        {
            screen.fillRoundRect(10, 213, 220, 67, 20, ILI9341_GREEN);
            screen.fillRoundRect(15, 218, 210, 57, 15, ILI9341_BLACK);
            screen.println("Green");
        }
        else
        {
            screen.fillRoundRect(10, 213, 220, 67, 20, ILI9341_BLUE);
            screen.fillRoundRect(15, 218, 210, 57, 15, ILI9341_BLACK);
            screen.println("Blue");
        }
        
    }

    void UI::updateInitState(int initState) {
        screen.fillRect(100, 110, 140, 15, ILI9341_BLACK);
        screen.setTextSize(2);
        screen.setCursor(100, 110);

        switch (initState) {
            case NO_INIT:
            screen.setTextColor(ILI9341_RED);
            screen.println("No init !");
            break;
            case PENDING_INIT:
            screen.setTextColor(ILI9341_BLUE);
            screen.println("Init ...");
            break;
            case DONE_INIT:
            screen.setTextColor(ILI9341_GREEN);
            screen.println("Init done !");
            break;
            default:
            screen.setTextColor(ILI9341_RED);
            screen.println("Unknow init");
        }
    }

    void UI::drawBackScreenMatch() {
        screen.fillScreen(ILI9341_BLACK);
        screen.setTextColor(ILI9341_ORANGE);
        screen.setTextSize(2);
        // Texte "Etats"
        screen.setCursor(10, 10);
        screen.println("Strat : ");
        screen.setCursor(10, 30);
        screen.println("    X :        mm");
        screen.setCursor(10, 50);
        screen.println("    Y :        mm");
        screen.setCursor(10, 70);
        screen.println("Theta :        deg");
        screen.setCursor(10, 90);
        screen.println("Lidar : ");
        screen.drawFastHLine(10, 112, 220, ILI9341_WHITE);
        // Texte "Temps"
        screen.setCursor(10, 120);
        screen.println("Temps Restant : ");
        screen.setCursor(125, 172);
        screen.println("Sec.");
        // Texte "Score"
        screen.setCursor(10, 190);
        screen.println("Score :");
        // Texte "Points"
        screen.setCursor(150, 300);
        screen.println("points");
    }

    void UI::updateAllMatchVar() {
        updateMatchTime(100);
        updateScore(9);
        updatePosition(1780.0, 1250.0, 180.0);
        updateLidarState(LIDAR_STATE);
        updateStrategyState(STRATEGY);
    }

    void UI::updateStrategyState(bool stratState) {
        screen.fillRect(100, 10, 140, 15, ILI9341_BLACK);
        screen.setTextColor(ILI9341_WHITE);
        screen.setTextSize(2);
        screen.setCursor(100, 10);
        if(stratState == CAKE_FIRST) screen.println("CAKE");
        else if(stratState == CHERRY_FIRST) screen.println("CHERRY");
    }

    void UI::updateLidarState(bool lidarState) {
        screen.fillRect(100, 90, 140, 15, ILI9341_BLACK);
        screen.setTextSize(2);
        screen.setCursor(100, 90);

        if (lidarState == LIDAR_ERROR) {
            screen.setTextColor(ILI9341_RED);
            screen.println("Waiting...");
        } else {
            screen.setTextColor(ILI9341_GREEN);
            screen.println("Connected");
        }
    }

    void UI::updateTiretteState(int tiretteState) {
        screen.fillRect(100, 130, 140, 15, ILI9341_BLACK);
        screen.setTextSize(2);
        screen.setCursor(100, 130);

        switch (tiretteState)
        {
        case TIRETTE_UNARMED:
            screen.setTextColor(ILI9341_RED);
            screen.println("unarmed !");
            break;
        case TIRETTE_ARMED:
            screen.setTextColor(ILI9341_GREEN);
            screen.println("armed !");
            break;
        case TIRETTE_GO:
            screen.setTextColor(ILI9341_BLUE);
            screen.println("Go !");
            break;
        default:
            break;
        }
    }

    void UI::updatePosition(float Xpos, float Ypos, float Tpos) {
        screen.fillRect(100, 30, 82, 55, ILI9341_BLACK);
        screen.setTextColor(ILI9341_WHITE);
        screen.setTextSize(2);
        screen.setCursor(100, 30);
        screen.println(Xpos);
        screen.setCursor(100, 50);
        screen.println(Ypos);
        screen.setCursor(100, 70);
        screen.println(Tpos);
    }

    void UI::updateMatchTime(int tMatch) {
        screen.fillRect(10, 140, 110, 45, ILI9341_BLACK);
        screen.setCursor(10, 142);
        screen.setTextSize(6);
        // Change color of the timer
        if (tMatch < 10) {
            screen.setTextColor(ILI9341_RED);
        } else if (tMatch < 50) {
            screen.setTextColor(ILI9341_ORANGE);
        } else {
            screen.setTextColor(ILI9341_GREEN);
        }
        // Add "0"
        if (tMatch < 100) screen.print("0");
        if (tMatch < 10) screen.print("0");
        // Print time
        screen.println(tMatch);
    }

    void UI::updateScore(int uScore) {
        screen.fillRect(10, 210, 220, 85, ILI9341_BLACK);
        screen.setCursor(10, 210);
        screen.setTextColor(ILI9341_WHITE);
        screen.setTextSize(12);
        if (uScore < 100) screen.print("0");
        if (uScore < 10) screen.print("0");
        screen.println(uScore);
    }



} // namespace TwinSystem
