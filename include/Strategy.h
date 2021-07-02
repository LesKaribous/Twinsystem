#pragma once

namespace Strategy{

    int score = 0;
    bool matchEnCours = false;

    //----- STRATEGIES -----
    void homologationPrimaire();
    void homologationSecondaire();
    void matchPrimaire();
    void matchSecondaire();
    void testLancerGobelet();

    void sequenceRecalage();
    void waitLaunch();


}
