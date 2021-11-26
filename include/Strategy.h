#pragma once
#include "Actuators.h"

namespace Strategy{

	#ifdef DANSE
	extern bool danseMode;
    void danse();
	#endif

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
