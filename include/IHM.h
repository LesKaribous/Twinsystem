#pragma once 

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

namespace IHM{
    void init();
    bool majTemps();
    void majScore();

    void updateButtonIHM  ();
    void readButtonState  ();
    //------ Gestion des E/S ------
    bool getTirette       ();
    bool getDetection     ();
    bool getStrategie     ();
    bool getCheck         ();
    bool getEquipe        ();
    //----- Gestion des etats -----
    void setRecalage      (bool state);
    bool getRecalage      ();
    //------ Pages LCD ------
    void splashScreen     ();
    void menuScreen       ();
    void initScreen       ();
    void checkListScreen  ();
    void goScreen         ();
    void matchScreen      (int score,int tempsRestant,int nbrBadCRC);

    namespace {
        // Declaration de l'ecran
        U8G2_ST7920_128X64_F_SW_SPI _u8g2(U8G2_R3,13,11,12,U8X8_PIN_NONE);
        // Declaration des variables IHM

        bool    detection = false, 
                strategie = false, 
                typeRobot = ROBOT_PRIMAIRE, 
                equipe = EQUIPE_BLEU ;
        
        byte _LOGO_KARIBOUS_bits[];

        // Adversaire
        const bool _ADVERSAIRE_NON = 0 ;
        const bool _ADVERSAIRE_OUI = 1 ;

        // Couleur Equipe
        const bool _EQUIPE_JAUNE = 1 ;
        const bool _EQUIPE_BLEU  = 0 ;

        // Strategie
        const bool _STRATEGIE_HOMOLOGATION = 1 ;
        const bool _STRATEGIE_MATCH = 0 ;

        // type de Robot
        const bool _ROBOT_PRIMAIRE   = 1 ;
        const bool _ROBOT_SECONDAIRE = 0 ;

        // Autres
        const int _TEMPS_MATCH = 101000 ;

        // Declaration des pins IHM
        const int _pinTirette   = A22 ;         // Pin tirette

        const int _latchMux     = 37 ;
        const int _clockMux     = 38 ;
        const int _clockInhMux  = A21;
        const int _dataMux      = 39 ;

        // Logo Karibous
        const byte _LOGO_KARIBOUS_width = 64 ;
        const byte _LOGO_KARIBOUS_height = 64 ;


        // Declaration des variables IHM
        int _buttonState[8];
        bool _tirette = false ,
        _detection = false ,
        _strategie = false ,
        _check = false ,
        _typeRobot = _ROBOT_PRIMAIRE ,
        _equipe = _EQUIPE_BLEU ,
        _recalage = false  ;
        // Liste d'action de la check list:
        char* _titreList[6] = {
                        "Robot",
                        "Experi.",
                        "Palets",
                        "Table",
                        "Balise",
                        "GO !"};
        // Liste d'action de la check list:
        char* _list[6] = {
            "Installer le robot secondaire ",
            "Installer l'experience        ",
            "Verifier les palets           ",
            "Nettoyer la table             ",
            "Poser la balise adversaire    ",
            "Bon match !                   "
        };
    }
}

