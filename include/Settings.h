#pragma once

namespace Setting{

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

// Pos Bras
const int POS_BRAS_G_BAS = 38;
const int POS_BRAS_G_HAUT = 120;

const int POS_BRAS_D_BAS = 160;
const int POS_BRAS_D_HAUT = 70;

}
