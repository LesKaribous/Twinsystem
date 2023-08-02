#pragma once
#include "system/core/service.h"
#include "services/terminal/console.h"

#include "button.h"
#include "localisationSensor.h"

class Inputs : public Service{
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
    LocalisationSensor localisationSensorA;
    LocalisationSensor localisationSensorB;
    LocalisationSensor localisationSensorC;
    

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

    bool hasTurbineStateChanged();

    bool isBlue() const;
    bool isCake() const;
    bool isGreen() const;
    bool isCherry() const;
    bool isBrown() const;
    bool isNoBrown() const;
    bool isPrimary() const;
    bool isSecondary() const;
    bool getRobotType() const;
    bool getStrategyState() const;
    bool getUpTrapState() const;
    bool getDownTrapState() const;
    bool getLowTurbineState() const;
    bool getHighTurbineState() const;

    void waitButtonRelease();

    float getDistanceSensorA() const;
    float getDistanceSensorB() const;
    float getDistanceSensorC() const;

    bool hasChanged() const;
    void update() override;
};



