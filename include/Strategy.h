#pragma once
#include "Actuators.h"

namespace Strategy{

    extern int score;
    extern bool matchEnCours;

    //----- STRATEGIES -----
    void homologationPrimaire();
    void homologationSecondaire();
    void matchPrimaire();
    void matchSecondaire();

    void testPincer();

    void sequenceRecalage();
    void waitLaunch();

    void grab(Actuators::Side side = Actuators::Side::BOTH);
    void release(Actuators::Side side = Actuators::Side::BOTH);

}
