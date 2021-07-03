#pragma once

namespace Strategy{

    extern int score;
    extern bool matchEnCours;

    //----- STRATEGIES -----
    void homologationPrimaire();
    void homologationSecondaire();
    void matchPrimaire();
    void matchSecondaire();
    void testLancerGobelet();

    void sequenceRecalage();
    void waitLaunch();


}
