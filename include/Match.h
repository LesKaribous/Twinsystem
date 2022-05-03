#pragma once
#include "IHM.h"
#include "Settings.h"

#define TEAM_BLUE IHM::getEquipe() == Setting::EQUIPE_BLEU
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

