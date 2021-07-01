#pragma once

namespace Strategy{

    int score = 0;
    bool matchEnCours = false;

    //----- INITIALISATION ROBOT -----

    void waitLaunch();

    //----- STRATEGIES -----
    void homologationPrimaire();
    void homologationSecondaire();
    void matchPrimaire();
    void matchSecondaire();
    void testLancerGobelet();



    //----- AUTRES -----

    void attente(int temps);

}
