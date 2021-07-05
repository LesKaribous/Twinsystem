#pragma once
#include "IHM.h"
#include "Settings.h"

#define TEAM_BLUE IHM::getEquipe() == Setting::EQUIPE_BLEU
#define TEAM_YELLOW IHM::getEquipe() == Setting::EQUIPE_JAUNE

 #define WAIT(x) Match::attente(x)

namespace Match{

    //----- FIN DE MATCH -----
    
    void start();
    void waitFinMatch();
    void finMatch();

    bool majTemps();
    void majScore();

    //----- AUTRES -----
    void attente(int temps);
}

