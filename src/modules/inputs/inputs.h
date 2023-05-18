#pragma once
#include "core/module.h"
#include "modules/inputs/button.h"

class Inputs : public Module{
private:
    Switch starter;
    Switch teamSwitch;
    Switch twinSwitch;
    Button resetButton;
    Switch strategySwitch;
    Button upTrapSwitch;
    Button downTrapSwith;
    Switch lowTurbineSwitch;
    Switch highTurbineSwitch;

public:
    Inputs();
    ~Inputs();

    void freezeSettings();
    void unfreezeSettings();

    bool hasStarter() const;
    bool buttonPressed() const;
    bool starterPulled() const;
    bool starterPlaced() const;
    bool buttonReleased() const;
    bool starterCancelled() const;

    bool isBlue() const;
    bool isCake() const;
    bool isGreen() const;
    bool isCherry() const;
    bool isPrimary() const;
    bool isSecondary() const;
    bool getRobotType() const;
    bool getStrategyState() const;
    bool getUpTrapState() const;
    bool getDownTrapState() const;
    bool getLowTurbineSate() const;
    bool getHighTurbineState() const;

    void waitButtonRelease();

    bool hasChanged() const;
    void update() override;
};



