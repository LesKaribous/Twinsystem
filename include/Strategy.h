#pragma once
#include "Actuators.h"

#define FIRST_DISPENSER 1
#define SECOND_DISPENSER 2 

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
    void takeHorizontalDispenser(Actuators::Bras &robotArm);
    void layOnGallery(Actuators::Bras &robotArm,int colorElement);
    void takeStatuette(Actuators::Bras &robotArm);
    void layStatuette(Actuators::Bras &robotArm);
    void takeAndPushUnder(Actuators::Bras &robotArm);
    void takeDispenser(Actuators::Bras &robotArm, int dispenser);
    void takeFirstDispenser();
    void goHome();
    //----- ACTUATORS STRATEGIES -----
    void takeElement(Actuators::Bras robotArm,int location);
    void releaseElement(Actuators::Bras robotArm,int location);
    void flipElement(Actuators::Bras robotArm);
    void flipChallenge(Actuators::Bras robotArm);
    void probeElement();
}
