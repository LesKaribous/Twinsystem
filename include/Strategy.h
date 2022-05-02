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
}
