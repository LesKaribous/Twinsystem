#include "ihm.h"
#include "settings.h"
#include "os/console.h"

INSTANTIATE_SERVICE(IHM, ihm)

IHM::IHM() : Service(ServiceID::ID_IHM), 
    starter(Pin::Inputs::starter),
    teamSwitch(Pin::Inputs::teamSwitch),
    twinSwitch(Pin::Inputs::twinSwitch),
    strategySwitch(Pin::Inputs::strategySwitch),
    resetButton(Pin::Inputs::resetButton),
    screen(Pin::TFT::CS, Pin::TFT::DC, Pin::TFT::RST, Pin::TFT::MOSI, Pin::TFT::SCK, Pin::TFT::MISO)
{};


void IHM::onAttach(){
    Console::info() << "IHM activated" << Console::endl;

    screen.begin();
    starter.setInverted(true);
    teamSwitch.setInverted(true);
    twinSwitch.setInverted(true);
    resetButton.setInverted(true);
    strategySwitch.setInverted(true);
    starter.init();
    teamSwitch.init();
    twinSwitch.init();
    resetButton.init();
    strategySwitch.init();
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
    bootProgress = 0;
    drawBootProgress("SystemBoot");
}

void IHM::onUpdate(){
    //update inputs
    starter.read();
    teamSwitch.read();
    twinSwitch.read();
    resetButton.read();
    strategySwitch.read();

    //update fields
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
    draw();
}


void IHM::clear(){
    screen.fillScreen(ILI9341_BLACK);
    screen.setTextSize(2);
}

void IHM::setPage(Page p){
    currentPage = p;
    clear();
    needDraw = true;
    lastDraw = 0;
}

void IHM::addBootProgress(int v){
    if (currentPage == Page::BOOT){
        bootProgress += v;
    }
}

void IHM::setRobotPosition(Vec3 pos){
    x.setValue(pos.x);
    y.setValue(pos.y);
    z.setValue(pos.z);
}

void IHM::drawBootProgress(String msg){
    if (currentPage == Page::BOOT){
        if(needDraw){
            needDraw = false;
            
            screen.setTextSize(4);
            screen.setCursor(0,50);
            screen.println("TwinSystem");
            
            //screen.drawBitmap(60,50, logo, 32, 32, ILI9341_WHITE);
        }
        
        //screen.fillRect(9, 219, 231, 11, ILI9341_WHITE);
        screen.fillRect(10, 200, 230, 118, ILI9341_BLACK);

        screen.setTextSize(2);
        screen.setCursor(30,200);

        screen.setTextColor(ILI9341_WHITE);
        screen.println(msg.c_str());

        
        int xmax = constrain(int(map(bootProgress, 0,100, 10,220)), 10, 220);
        screen.fillRect(10, 220, xmax, 10 , ILI9341_GREEN);

        if(bootProgress > 100){
            xmax = constrain(int(map(bootProgress-100, 0,100, 10,220)), 10, 220);
            screen.fillRect(10, 240, xmax, 10 , ILI9341_GREEN);
        }

        
    }
}


void IHM::draw(){
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

void IHM::updateAllStartVar() {
    bool forceDraw = millis() - lastDraw > 2000;
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

void IHM::updateAllMatchVar() {
    bool forceDraw = millis() - lastDraw > 2000;
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


void IHM::drawBackScreenStart() {
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
    screen.println(String(__DATE__) + String(" ") + String(__TIME__));
}

void IHM::updateTeamColor(bool team){
    screen.fillRect(10, 200, 220, 98, ILI9341_BLACK);
    screen.setTextSize(4);
    screen.setCursor(60,230);
    screen.setTextColor(ILI9341_WHITE);
    if(team == Settings::Match::YELLOW)
    {
        screen.fillRoundRect(10, 213, 220, 67, 20, ILI9341_YELLOW);
        screen.fillRoundRect(15, 218, 210, 57, 15, ILI9341_BLACK);
        screen.println("Yellow");
    }
    else
    {
        screen.fillRoundRect(10, 213, 220, 67, 20, ILI9341_BLUE);
        screen.fillRoundRect(15, 218, 210, 57, 15, ILI9341_BLACK);
        screen.println("Blue");
    }
    
}

void IHM::updateInitState() {
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

void IHM::drawBackScreenMatch() {
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

void IHM::updateStrategyState(bool stratState) {
    screen.fillRect(100, 10, 140, 15, ILI9341_BLACK);
    screen.setTextColor(ILI9341_WHITE);
    screen.setTextSize(2);
    screen.setCursor(100, 10);
    if(!twinSwitch.getState()){
        if(stratState==Settings::Match::CAKE) screen.println("CAKE");
        else screen.println("CHERRY");
    }
    else{
        if(stratState==Settings::Match::BROWN) screen.println("BROWN");
        else screen.println("NOBROWN"); 
    }

    
}

void IHM::updateLidarState(bool lidarState) {
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

void IHM::updateTiretteState(bool tiretteState) {
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

void IHM::updatePosition(float Xpos, float Ypos, float Tpos) {
    screen.fillRect(100, 30, 82, 55, ILI9341_BLACK);
    screen.setTextColor(ILI9341_WHITE);
    screen.setTextSize(2);
    screen.setCursor(100, 30);
    if(Xpos < -0.1 || probing.getState()) screen.println("?");
    else screen.println(int(Xpos));

    screen.setCursor(100, 50);

    if(Ypos < -0.1 || probing.getState()) screen.println("?");
    else screen.println(int(Ypos));

    screen.setCursor(100, 70);
    screen.println(int(Tpos));
}

void IHM::updateMatchTime(int tMatch) {
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

void IHM::updateScore(int uScore) {
    screen.fillRect(10, 210, 220, 85, ILI9341_BLACK);
    screen.setCursor(10, 210);
    screen.setTextColor(ILI9341_WHITE);
    screen.setTextSize(12);
    if (uScore < 100) screen.print("0");
    if (uScore < 10) screen.print("0");
    screen.println(uScore);
}

void IHM::waitButtonRelease(){
    long time = millis();
    while (buttonPressed()){
        resetButton.read();
        if(millis() - time  > 2000) return;
    }
}

void IHM::setIntercomState(bool value){
    intercom.setValue(value);
}

void IHM::setTime(int t){
    time.SetValue(t);
}

void IHM::addScorePoints(int p){
    score.SetValue(score.getValue() + p);
}

void IHM::freezeSettings(){
	teamSwitch.disable();
	strategySwitch.disable();
}
void IHM::unfreezeSettings(){
	teamSwitch.enable();
	strategySwitch.enable();
}

bool IHM::buttonPressed() const{
	return resetButton.getState();
}

bool IHM::buttonReleased() const{
	return resetButton.hasChanged() && !resetButton.getState();
}

bool IHM::hasStarter() const{
	return starter.getState();
}

bool IHM::starterPulled() const{
	return !starter.getState() && !resetButton.getState();
}

bool IHM::starterPlaced() const{
	return starter.getState() && starter.hasChanged();
}

bool IHM::starterCancelled() const{
	return !starter.getState() && resetButton.getState();
}

bool IHM::isPrimary() const{
	return twinSwitch.getState() == Settings::Match::PRIMARY;
}
bool IHM::isSecondary() const{
	return twinSwitch.getState() == Settings::Match::SECONDARY;
}
bool IHM::isColorBlue() const{
	return teamSwitch.getState() == Settings::Match::BLUE;
}
bool IHM::isColorYellow() const{
	return teamSwitch.getState() == Settings::Match::YELLOW;
}

bool IHM::getStrategyState() const{
	return strategySwitch.getState();
}

bool IHM::getRobotType() const{
	return twinSwitch.getState();
}