#pragma once
#include "Actuators.h"

namespace Strategy{

    extern int score;
    extern bool matchEnCours;

    //----- STRATEGIES -----
    void homologation();
    void match();
    void recalage();
    void testingFlip();
    void testingActuators();

    void waitLaunch();
    //----- ACTUATORS STRATEGIES -----
    void takeElement(Actuators::Bras robotArm,int location);
    void releaseElement(Actuators::Bras robotArm,int location);
    void flipElement(Actuators::Bras robotArm);
    void flipChallenge(Actuators::Bras robotArm);
}
