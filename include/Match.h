#pragma once
#include "IHM.h"
#include "Settings.h"

#define TEAM_BLUE IHM::getEquipe() == Setting::EQUIPE_BLEU
#define TEAM_YELLOW IHM::getEquipe() == Setting::EQUIPE_JAUNE

namespace Match{
    double tempsRestant = TEMPS_MATCH;
    double timeInit = 0;
    int score = 0;

    bool matchEnCours = false;

    //----- FIN DE MATCH -----
    inline void waitFinMatch();
    inline void finMatch();

    inline bool majTemps();
    inline void majScore();


    
    //----- AUTRES -----
    void attente(int temps);
}

