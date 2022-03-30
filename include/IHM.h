#pragma once 

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

#include "Match.h"
#include "Settings.h"

/***********
 *      --Button--
 * Button 1 : Index 5 : Recalage
 * Button 2 : Index 6 : Void 
 * Button 3 : Index 4 : Void 
 * Button 4 : Index 7 : Void
 * 
 *      --Switch--
 * Switch 1 : Index 1 : Equipe 
 * Switch 2 : Index 2 : Strategy
 * Switch 3 : Index 0 : Balise
 * Switch 4 : Index 3 : Robot
 **********/


namespace IHM{

    /*
    void init();

    void updateButtonIHM  ();
    void readButtonState  ();
    void freezeSettings   ();

    //------ Gestion des E/S ------
    bool getTirette       ();
    bool getRobot         ();
    bool getDetection     ();
    bool getStrategie     ();
    bool getCheck         ();
    bool getEquipe        ();
    bool getTestBras      ();
    bool getOpponent      ();
    void setOpponent      (int);

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

    */
}

