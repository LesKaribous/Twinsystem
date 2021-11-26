#pragma once

//#define TESTARM
//#define DANSE

namespace Setting{

    const float SPEED = 100; //%

    //Adresse I2C pour les cartes esclaves
    const int PILOT_ADRESS = 0x3C;

    // Adversaire
    const bool ADVERSAIRE_NON = 1 ;
    const bool ADVERSAIRE_OUI = 0 ;

    // Couleur Equipe
    const bool EQUIPE_JAUNE = 1 ;
    const bool EQUIPE_BLEU  = 0 ;

    // Strategie
    const bool STRATEGIE_HOMOLOGATION = 1 ;
    const bool STRATEGIE_MATCH        = 0 ;

    // type de Robot
    const bool ROBOT_PRIMAIRE = 1 ;
    const bool ROBOT_SECONDAIRE = 0 ;

    // Autres
    const int TEMPS_MATCH = 101000 ;


    //********** Actuators ***********
    //********** Pos Bras ************
    const int POS_BRAS_G_PUSH   =  20;
    const int POS_BRAS_G_BAS    =  38; //38
    const int POS_BRAS_G_HAUT   = 119;

    const int POS_BRAS_D_PUSH   = 179;
    const int POS_BRAS_D_BAS    = 165; //160
    const int POS_BRAS_D_HAUT   =  75; 


    //********* Pos Pincer **********

    //Right
    const int PINCER_R_FOLDED   = 40;
    const int PINCER_R_ARMED    = 145;
    const int PINCER_R_UNFOLDED = 155;
    const int PINCER_R_CLOSED   = 10;
    const int PINCER_R_OPEN     = 130;

    //LEFT
    const int PINCER_L_FOLDED   = 140;
    const int PINCER_L_ARMED    = 35;
    const int PINCER_L_UNFOLDED = 20;
    const int PINCER_L_CLOSED   = 155;
    const int PINCER_L_OPEN     = 30;

    //Vacuum
    const bool PINCER_SUCk      = true;
    const bool PINCER_EV_ON     = true;
}
