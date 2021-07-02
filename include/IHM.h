#pragma once 

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

namespace IHM{
    void init();

    void updateButtonIHM  ();
    void readButtonState  ();

    //------ Gestion des E/S ------
    bool getTirette       ();
    bool getDetection     ();
    bool getStrategie     ();
    bool getCheck         ();
    bool getEquipe        ();

    //----- Gestion des etats -----
    void setRecalage      ( bool state );
    bool getRecalage      ();

    namespace LCD{
        //------ Pages LCD ------
        void splashScreen     ();
        void menuScreen       ();
        void initScreen       ();
        void checkListScreen  ();
        void goScreen         ();
        void matchScreen      ( int score,int tempsRestant,int nbrBadCRC );
    }
}

