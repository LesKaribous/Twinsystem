#pragma once
#include "IHM.h"
#include "Settings.h"

#define TEAM_PURPLE IHM::getEquipe() == Setting::EQUIPE_VIOLET
#define TEAM_YELLOW IHM::getEquipe() == Setting::EQUIPE_JAUNE

 #define WAIT(x) \
    if(Match::hasStarted()) \
        Match::attente(x); \
    else \
        delay(x);               
//END WAIT

    //Match::attente(x*int(Setting::SPEED / 100.0));

namespace Match{

    enum State{
        INIT,
        READY,
        ARMED,
        RUNNING,
        HOMING, 
        STOPPED,
        FINISHED
    };

    extern State state;

    void start();
    void waitEnd();
    void end();

    int GetScore();
    double GetTempsRestant();

    void update();
    bool updateTime();
    void updateScore(int points, int multiplicateur = 1);

    inline bool hasStarted(){return state >= State::RUNNING;}

    //----- AUTRES -----
    void wait(int temps);
}

namespace Score{

    //---------SCORES / POINTS POSSIBLES---------
    // Voir page 37 du rgèlement
    const int CARRE_BASCULE                 = 5,    // pour chaque carré de fouille basculé à la couleur de l’équipe
              CARRE_NON_BASCULE             = 5,    // au moins un carré de fouille à la couleur de l’équipe est basculé
              ECHANTILLON_ENLEVE            = 1,    // chaque échantillon enlevé d’un distributeur 
              ECHANTILLON_GALERIE           = 3,    // par échantillon sur la galerie
              ECHANTILLON_GALERIE_TRIE      = 3,    // par échantillon face trésor et trié dans sa période historique;
              ECHANTILLON_CAMPEMENT         = 1,    // par échantillon dans le campement
              ECHANTILLON_CAMPEMENT_TRIE    = 1,    // par échantillon face trésor et trié dans le campement
              ECHANTILLON_ABRI              = 5,    // par échantillon dans l’abri
              ROBOTS_CAMPEMENT              = 20,   // si tous les robots de l’équipe sont dans le campement ou la zone de fouille
              STATUETTE_DEPOSEE             = 2,    // pour la dépose de la statuette sur le piédestal pendant le temps de préparation
              STATUETTE_ENLEVEE             = 5,    // si la statuette n’est plus présente en fin de match sur le piédestal
              REPLIQUE_DEPOSEE              = 10,   // si la réplique est présente en fin de match sur le piédestal
              STATUETTE_VITRINE             = 15,   // si la statuette est présente en fin de match dans la vitrine
              VITRINE_DEPOSEE               = 2,    // si l’équipe dépose une vitrine pendant le temps de préparation
              VITRINE_ACTIVEE               = 5;    // si la vitrine est activée

}

