#pragma once
#include "Actuators.h"

namespace Strategy{

    extern int score;
    extern bool matchEnCours;

    //----- STRATEGIES -----
    void recalage();
    void homologation();
    void match();

    void recalagePrimary();
    void homologationPrimary();
    void matchPrimary();

    void recalageSecondary();
    void homologationSecondary();
	void matchSecondary();

    void waitLaunch();
    //-------- SOUS-STRATEGIES --------
    void takeGroundTrio();
    void takeHorizontalDispenser(Actuators::Bras robotArm);
    void layOnGallery(Actuators::Bras robotArm, int Color);
    //----- ACTUATORS STRATEGIES -----
    void takeElement(Actuators::Bras robotArm,int location);
    void releaseElement(Actuators::Bras robotArm,int location);
    void flipElement(Actuators::Bras robotArm);
    void flipChallenge(Actuators::Bras robotArm);
    void probeElement();
}
