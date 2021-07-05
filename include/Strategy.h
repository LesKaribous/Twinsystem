#pragma once

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


}
