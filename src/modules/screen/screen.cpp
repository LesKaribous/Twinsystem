#include "pin.h"
#include "settings.h"
#include "font.h"
#include "modules/screen/screen.h"

const uint8_t logo[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0x00, 0x00, 0x7f, 0xff, 0x00, 
  0x00, 0xff, 0xff, 0x80, 0x00, 0xff, 0xff, 0xe0, 
  0x07, 0xff, 0xff, 0x80, 0x00, 0xff, 0xff, 0xc0, 
  0x07, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x80, 
  0x07, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x00, 
  0x07, 0xff, 0xff, 0x80, 0x03, 0xff, 0xfe, 0x00, 
  0x07, 0xff, 0xff, 0x80, 0x07, 0xff, 0xfc, 0x00, 
  0x07, 0xff, 0xff, 0x80, 0x07, 0xff, 0xfc, 0x00, 
  0x03, 0xff, 0xff, 0x80, 0x0f, 0xff, 0xf8, 0x00, 
  0x03, 0xff, 0xff, 0x80, 0x0f, 0xff, 0xf0, 0x00, 
  0x03, 0xff, 0xff, 0x80, 0x1f, 0xff, 0xe0, 0x00, 
  0x03, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xc0, 0x00, 
  0x03, 0xff, 0xff, 0x80, 0x3f, 0xff, 0x80, 0x00, 
  0x03, 0xff, 0xff, 0x80, 0x7f, 0xff, 0x00, 0x00, 
  0x03, 0xff, 0xff, 0x80, 0xff, 0xff, 0x00, 0x00, 
  0x03, 0xff, 0xff, 0x81, 0xff, 0xfe, 0x00, 0x00, 
  0x03, 0xff, 0xff, 0x81, 0xff, 0xfc, 0x00, 0x00, 
  0x03, 0xff, 0xff, 0x83, 0xff, 0xf8, 0x00, 0x00, 
  0x03, 0xff, 0xff, 0x87, 0xff, 0xf0, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0x8f, 0xff, 0xe0, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0x8f, 0xff, 0xc0, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0x9f, 0xff, 0x80, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0xbf, 0xff, 0x00, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 
  0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
  0x00, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 
  0x00, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 
  0x00, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 
  0x00, 0xff, 0xff, 0xf7, 0xff, 0xe0, 0x00, 0x00, 
  0x00, 0xff, 0xff, 0xe3, 0xff, 0xf0, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0xc3, 0xff, 0xf8, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0x81, 0xff, 0xf8, 0x00, 0x00, 
  0x00, 0x7f, 0xfe, 0x01, 0xff, 0xfc, 0x00, 0x00, 
  0x00, 0x7f, 0xfc, 0x01, 0xff, 0xfc, 0x00, 0x00, 
  0x00, 0x7f, 0xf8, 0x00, 0xff, 0xfe, 0x00, 0x00, 
  0x00, 0x7f, 0xf0, 0x00, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0x7f, 0xe0, 0x00, 0x7f, 0xff, 0x80, 0x00, 
  0x00, 0x3f, 0xc0, 0x00, 0x7f, 0xff, 0xc0, 0x00, 
  0x00, 0x3f, 0xc0, 0x00, 0x3f, 0xff, 0xc0, 0x00, 
  0x00, 0x3f, 0xc0, 0x00, 0x3f, 0xff, 0xe0, 0x00, 
  0x00, 0x3f, 0xc0, 0x00, 0x1f, 0xff, 0xf0, 0x00, 
  0x00, 0x3f, 0xc0, 0x00, 0x1f, 0xff, 0xf8, 0x00, 
  0x00, 0x3f, 0xc0, 0x00, 0x0f, 0xff, 0xf8, 0x00, 
  0x00, 0x3f, 0xe0, 0x00, 0x0f, 0xff, 0xfc, 0x00, 
  0x00, 0x1f, 0xe0, 0x00, 0x07, 0xff, 0xfe, 0x00, 
  0x00, 0x1f, 0xe0, 0x00, 0x07, 0xff, 0xff, 0x00, 
  0x00, 0x1f, 0xe0, 0x00, 0x03, 0xff, 0xff, 0x80, 
  0x00, 0x1f, 0xe0, 0x00, 0x01, 0xff, 0xfc, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

Screen::Screen() : Module(SCREEN), screen(Pin::TFT::CS, Pin::TFT::DC, Pin::TFT::RST, Pin::TFT::MOSI, Pin::TFT::SCK, Pin::TFT::MISO) {
    screen.begin();

    x.init();
    y.init();
    z.init();
    probed.init();
    probing.init();
    started.init();
    armed.init();
    intercom.init();
    score.init();
    time.init();

    clear();
    needDraw = true;
    lastDraw = 0;
    drawBootProgress(0, "SystemBoot");
}

Screen::~Screen(){
}

void Screen::update(){
    x.read();
    y.read();
    z.read();
    score.read();
    time.read();
    intercom.read();
    probing.read();
    probed.read();
    armed.read();
    started.read();
    starter.read();
    teamSwitch.read();
    twinSwitch.read();
    resetButton.read();
    strategySwitch.read();
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

void Screen::drawBootProgress(int p, String msg){
    if (currentPage == Page::BOOT){
        if(needDraw){
            needDraw = false;
            
            screen.setTextSize(4);
            screen.setCursor(0,50);
            screen.println("TwinSystem");
            
            //screen.drawBitmap(60,50, logo, 32, 32, ILI9341_WHITE);
        }
        
        screen.fillRect(9, 219, 231, 11, ILI9341_WHITE);
        screen.fillRect(10, 200, 230, 118, ILI9341_BLACK);


        screen.setTextSize(2);
        screen.setCursor(30,200);

        screen.setTextColor(ILI9341_WHITE);
        screen.println(msg.c_str());
        int xmax = constrain(int(map(p, 0,100, 10,220)), 10, 220);
        screen.fillRect(10, 220, xmax, 10 , ILI9341_WHITE);
    }
}


void Screen::draw(){
    switch (currentPage){
    case Page::BOOT :
        break;
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
    bool forceDraw = millis() - lastDraw > 800;
    if(forceDraw) lastDraw = millis();

    bool drawStrategy = strategySwitch.hasChanged() || forceDraw;
    bool drawTeam = teamSwitch.hasChanged() || forceDraw;
    bool drawStarter = starter.hasChanged() || forceDraw;
    bool drawIntercom = intercom.hasChanged() || forceDraw;
    bool drawProbing = probed.hasChanged() || probing.hasChanged() || forceDraw;
    bool drawPosition = x.hasChanged() || y.hasChanged() || z.hasChanged() || forceDraw;

    if(drawStrategy) updateStrategyState(strategySwitch.getState());
    if(drawTeam) updateTeamColor(teamSwitch.getState());
    if(drawStarter) updateTiretteState(starter.getState());
    if(drawIntercom) updateLidarState(intercom.getState());
    if(drawProbing) updateInitState();
    if(drawPosition){
        if( millis() - lastPosDraw >= 50){
            updatePosition(x.getValue(), y.getValue(), z.getValue()*RAD_TO_DEG);
            lastPosDraw = millis();
        }
    }    
}

void Screen::updateAllMatchVar() {
    bool forceDraw = millis() - lastDraw > 800;
    if(forceDraw) lastDraw = millis();

    bool drawTime = time.hasChanged() || forceDraw;
    bool drawScore = score.hasChanged() || forceDraw;
    bool drawIntercom = intercom.hasChanged() || forceDraw;
    bool drawPos = x.hasChanged() || y.hasChanged() || z.hasChanged() || forceDraw;

    if(drawTime) updateMatchTime(time.getValue());
    if(drawScore) updateScore(score.getValue());
    
    if(drawPos){
        if( millis() - lastPosDraw >= 50){
            updatePosition(x.getValue(), y.getValue(), z.getValue()*RAD_TO_DEG);
            lastPosDraw = millis();
        }
    } 

    if(forceDraw) updateStrategyState(strategySwitch.getState());
    if(drawIntercom) updateLidarState(intercom.getState());
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

    if(!probed.getState() && !probing.getState()){
        screen.setTextColor(ILI9341_RED);
        screen.println("Not probed");
    }else if(probing.getState()){
        screen.setTextColor(ILI9341_BLUE);
        screen.println("Probing ...");
    }else if(probed.getState()){
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
    screen.println(Xpos < 0 ? '?' : Xpos);
    screen.setCursor(100, 50);
    screen.println(Ypos< 0 ? '?' : Ypos);
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
