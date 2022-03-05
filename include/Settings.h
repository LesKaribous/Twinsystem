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


    namespace Robot{  
        const uint32_t ACCEL = 2500; // Old : 5000
        const uint32_t SPEED = 5000; // Old : 5000
    }

    namespace Stepper{
        const float 
            FACTOR_A  = 8.71f,//17.42f
            FACTOR_B  = 8.71f,
            FACTOR_C  = 8.71f,
            FACTOR_R  = 18.545f,//37.09f
            FACTOR_X  = 0.85f,
            FACTOR_Y  = 1.50f;
    }
    
    namespace Util{
        const float 
            sqrt3o2 = 1.0f * sqrt(3.0f) / 2.0f,
            rad120 = 120.0f * (PI / 180.0f),
            rad240 = 2.0f * rad120 ;
    }
}
