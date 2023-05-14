#include "modules/inputs/inputs.h"
#include "settings.h"


Inputs::Inputs() : 
    Module(INPUTS), 
    starter(Pin::Inputs::starter),
    teamSwitch(Pin::Inputs::teamSwitch),
    twinSwitch(Pin::Inputs::twinSwitch),
    resetButton(Pin::Inputs::resetButton),
    strategySwitch(Pin::Inputs::strategySwitch)
{
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
}

Inputs::~Inputs(){
}

void Inputs::update(){
    starter.read();
    teamSwitch.read();
    twinSwitch.read();
    resetButton.read();
    strategySwitch.read();
}

bool Inputs::hasChanged() const{
    return resetButton.hasChanged() || starter.hasChanged() || teamSwitch.hasChanged() || strategySwitch.hasChanged() || twinSwitch.hasChanged();
}
 
void Inputs::waitButtonRelease(){
    long time = millis();
    while (buttonPressed()){
        resetButton.read();
        if(millis() - time  > 2000) return;
    }
}

void Inputs::freezeSettings(){
	teamSwitch.disable();
	strategySwitch.disable();
	//avoidanceSwitch.disable();
}
void Inputs::unfreezeSettings(){
	teamSwitch.enable();
	strategySwitch.enable();
	//avoidanceSwitch.enable();
}

bool Inputs::buttonPressed() const{
	return resetButton.getState();
}

bool Inputs::buttonReleased() const{
	return resetButton.hasChanged() && !resetButton.getState();
}

bool Inputs::hasStarter() const{
	return starter.getState();
}

bool Inputs::starterPulled() const{
	return !starter.getState() && !resetButton.getState();
}

bool Inputs::starterPlaced() const{
	return starter.getState() && starter.hasChanged();
}

bool Inputs::starterCancelled() const{
	return !starter.getState() && resetButton.getState();
}

bool Inputs::isPrimary() const{
	return twinSwitch.getState() == Settings::Match::PRIMARY;
}
bool Inputs::isSecondary() const{
	return twinSwitch.getState() == Settings::Match::SECONDARY;
}
bool Inputs::isBlue() const{
	return teamSwitch.getState() == Settings::Match::BLUE;
}
bool Inputs::isGreen() const{
	return teamSwitch.getState() == Settings::Match::GREEN;
}
bool Inputs::isCake() const{
	return strategySwitch.getState() == Settings::Match::CAKE;
}
bool Inputs::isCherry() const{
	return strategySwitch.getState() == Settings::Match::CHERRY;
}

bool Inputs::getStrategyState() const{
	return strategySwitch.getState();
}

bool Inputs::getRobotType() const{
	return twinSwitch.getState();
}