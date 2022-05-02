#pragma once 


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

#define LOADING_SOUND 0001
#define UP_SOUND 0002
#define DOWN_SOUND 0003
#define SWITCH_RIGHT_SOUND 0004
#define SWITCH_LEFT_SOUND 0005
#define START_SOUND 0006
#define MUSEUM_SOUND 0007
#define DROOP_SOUND 0008

namespace IHM{

    void init();
    void menu();

    void updateButtonIHM  ();
    void readButtonState  ();
    void freezeSettings   ();

    //------ Gestion du ring Neopixel ------
    void setColor();

    //------ Gestion des E/S ------
    bool getTirette       ();
    bool getRobot         ();
    bool getDetection     ();
    bool getStrategie     ();
    bool getCheck         ();
    bool waitCheck        ();
    bool getEquipe        ();
    bool getArrowUp       ();
    bool getArrowDown     ();
    bool getOption01      ();
    bool getOption02      ();
    bool getOption03      ();

    //----- Gestion des etats -----
    void setRecalage      ( bool state );
    bool getRecalage      ();

    namespace LCD{
        //------ Pages LCD ------
        void splashScreen     ();
        void baseMenu         ();
        void affichePage      ();
        void page01           ();
        void page02           ();
        void page03           ();
        void adjustActuators  ();
        void startMenu        ();
        void menuScreen       ();
        void initScreen       ();
        void checkListScreen  ();
        void goScreen         ();
        void matchScreen      ();
    }

    namespace Sound{
        //------ Gestion des sons -------
        void playSound      (int soundfile);
        void switchSound    (bool state);
    }
}