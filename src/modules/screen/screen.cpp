#include "pin.h"
#include "modules/screen/screen.h"

Screen::Screen(const System& sys) : Module(SCREEN), system(sys), screen(Pin::TFT::CS, Pin::TFT::DC, Pin::TFT::RST, Pin::TFT::MOSI, Pin::TFT::SCK, Pin::TFT::MISO) {
    screen.begin();
    clear();
    needDraw = true;
    draw();
    update();
}

Screen::~Screen(){
}

void Screen::update(){
    x.read();
    y.read();
    z.read();
    probed.read();
    probing.read();
    started.read();
    armed.read();
    intercom.read();
    score.read();
    time.read();
    draw();
}

void Screen::clear(){
    screen.fillScreen(ILI9341_BLACK);
    screen.setTextSize(2);
}

void Screen::setPage(Page p){
    currentPage = p;
    clear();
    needDraw = true;
}

void Screen::draw(){
    switch (currentPage){
    case Page::INIT :
        if(needDraw) drawBackScreenStart();
        updateAllStartVar();
        break;
    case Page::MATCH :
        if(needDraw) drawBackScreenMatch();
        updateAllMatchVar();
        break;
    case Page::RESET :
        
        break;
    default:
        break;
    }
}

void Screen::updateAllStartVar() {
    if(system.inputs->strategySwitch.HasChanged()) updateStrategyState(inputs.strategySwitch.GetState());
    if(inputs.teamSwitch.HasChanged()) updateTeamColor(inputs.teamSwitch.GetState());
    if(inputs.starter.HasChanged()) updateTiretteState(inputs.starter.GetState());
    if(fields.intercom.HasChanged()) updateLidarState(fields.intercom.GetState());
    if(fields.probed.HasChanged() || fields.probing.HasChanged()) updateInitState();
    if(fields.x.HasChanged() || fields.y.HasChanged() || fields.z.HasChanged()){
        if( millis() - lastPosDraw >= 50){
            updatePosition(fields.x.GetValue(), fields.y.GetValue(), fields.z.GetValue()*RAD_TO_DEG);
            lastPosDraw = millis();
        }
    }    
}

void Screen::updateAllMatchVar() {
    if(fields.time.HasChanged()) updateMatchTime(fields.time.GetValue());
    if(fields.score.HasChanged()) updateScore(fields.score.GetValue());
    
    if(fields.x.HasChanged() || fields.y.HasChanged() || fields.z.HasChanged()){
        if( millis() - lastPosDraw >= 50){
            updatePosition(fields.x.GetValue(), fields.y.GetValue(), fields.z.GetValue()*RAD_TO_DEG);
            lastPosDraw = millis();
        }
    } 
    if(inputs.strategySwitch.HasChanged()) updateStrategyState(inputs.strategySwitch.GetState());
    if(fields.intercom.HasChanged()) updateLidarState(fields.intercom.GetState());
}


void Screen::drawBackScreenStart() {
    needDraw = false;
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

void Screen::updateTeamColor(bool team){
    screen.fillRect(10, 200, 220, 98, ILI9341_BLACK);
    screen.setTextSize(4);
    screen.setCursor(60,230);
    screen.setTextColor(ILI9341_WHITE);
    if(team == Settings::Match::GREEN)
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

void Screen::updateInitState() {
    screen.fillRect(100, 110, 140, 15, ILI9341_BLACK);
    screen.setTextSize(2);
    screen.setCursor(100, 110);

    if(!fields.probed.GetState() && !fields.probing.GetState()){
        screen.setTextColor(ILI9341_RED);
        screen.println("Not probed");
    }else if(fields.probing.GetState()){
        screen.setTextColor(ILI9341_BLUE);
        screen.println("Probing ...");
    }else if(fields.probed.GetState()){
        screen.setTextColor(ILI9341_GREEN);
        screen.println("Probed.");
    }else{
        screen.setTextColor(ILI9341_RED);
        screen.println("?");
    }
}

void Screen::drawBackScreenMatch() {
    needDraw = false;
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

void Screen::updateStrategyState(bool stratState) {
    screen.fillRect(100, 10, 140, 15, ILI9341_BLACK);
    screen.setTextColor(ILI9341_WHITE);
    screen.setTextSize(2);
    screen.setCursor(100, 10);
    if(stratState==Settings::Match::CAKE) screen.println("CAKE");
    else screen.println("CHERRY");
}

void Screen::updateLidarState(bool lidarState) {
    screen.fillRect(100, 90, 140, 15, ILI9341_BLACK);
    screen.setTextSize(2);
    screen.setCursor(100, 90);

    if (!lidarState) {
        screen.setTextColor(ILI9341_RED);
        screen.println("Waiting...");
    } else {
        screen.setTextColor(ILI9341_GREEN);
        screen.println("Connected");
    }
}

void Screen::updateTiretteState(bool tiretteState) {
    screen.fillRect(100, 130, 140, 15, ILI9341_BLACK);
    screen.setTextSize(2);
    screen.setCursor(100, 130);

    if(tiretteState){
        screen.setTextColor(ILI9341_GREEN);
        screen.println("armed !");
    }else{
        screen.setTextColor(ILI9341_RED);
        screen.println("unarmed !");
    }
}

void Screen::updatePosition(float Xpos, float Ypos, float Tpos) {
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

void Screen::updateMatchTime(int tMatch) {
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

void Screen::updateScore(int uScore) {
    screen.fillRect(10, 210, 220, 85, ILI9341_BLACK);
    screen.setCursor(10, 210);
    screen.setTextColor(ILI9341_WHITE);
    screen.setTextSize(12);
    if (uScore < 100) screen.print("0");
    if (uScore < 10) screen.print("0");
    screen.println(uScore);
}
