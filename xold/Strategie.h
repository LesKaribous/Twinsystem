#include <Arduino.h>
//#include <Servo.h>
#include "Bras.h"
#include "IHM.h"
#include "ComNavigation.h"

//                 (X-)
//                Back
//                  B
//           '''''''''''''''
//         '                 '
//        |                   |
// (Y-) R | |=|     -     |=| | L (Y+)
//        |         C         |
//        |                   |
//        '-------------------'
//                  F
//                Front
//                 (X+)

// BF : 170mm
// CF : 60mm
// CB : 110mm
// RC = LC : 110mm (+ 6 de plexi) 
// RL : ~240mm


//  _______________________________________
// |                                       |
// |                                       |
// |                o - - - > X            |
// |                '                      |
// |                '                      |
// |                '                      |
// |                V                      |
// |                Y                      |
// |_______________________________________|


// Adressage I2C pour les cartes esclaves
#define CARTE_DEPLACEMENT 60
//Etat de la position demandée
#define TERMINEE 0  // Position validée et terminée
#define RECU 1      // Position reçu
#define ERRONEE 2   // Position erronée. CRC nok.
#define BIZARRE 3   // Reponse étrange à ne pas prendre en compte

// Adversaire
const bool ADVERSAIRE_NON = 1 ;
const bool ADVERSAIRE_OUI = 0 ;
// Couleur Equipe
const bool EQUIPE_JAUNE = 1 ;
const bool EQUIPE_BLEU = 0 ;
// Strategie
const bool STRATEGIE_HOMOLOGATION = 1 ;
const bool STRATEGIE_MATCH = 0 ;
// type de Robot
const bool ROBOT_PRIMAIRE = 1 ;
const bool ROBOT_SECONDAIRE = 0 ;
// Autres
const int TEMPS_MATCH = 101000 ;

const int POS_BRAS_G_BAS = 38;
const int POS_BRAS_G_HAUT = 120;

const int POS_BRAS_D_BAS = 160;
const int POS_BRAS_D_HAUT = 70;

typedef enum {
  PRISE_DROITE,
  PRISE_GAUCHE,
  DEPOSE_DROITE,
  DEPOSE_GAUCHE,
  PRISE_ENSEMBLE,
  DEPOSE_ENSEMBLE,
  REPOS
} BrasVentouse ;

// Declaration des pins E/S
int pinPompeGauche=23,pinEVGauche=17,pinPompeDroit=27,pinEVDroit=26 ;
const int pinServoDroit=8,pinServoVentouseDroit=9,pinServoGauche=24,pinServoVentouseGauche=10 ;

const int pinServoDrapeau     = 15 ;
const int pinServoBrasDroit   = 16 ;      // Pin Servo bras droit
const int pinServoBrasGauche  = 25 ;      // Pin Servo bras gauche

const int pinBeacon = 28 ;

Bras brasDroit ;
Bras brasGauche ;

Servo servoDrapeau ;
Servo servoBrasDroit ;
Servo servoBrasGauche ;

int navError = 0   ; // Nombre d'erreur de com avec la navigation
/*
int nbrBizarre = 0  ; // Nombre de réponses bizarres

FastCRC8 CRC8;
byte bufNavRelatif[5]={0,0,0,0,0}; // Buffer d'envoi des ordres de navigation relatifs
byte crcNavRelatif = 0; // CRC de controle pour les ordres de navigation relatifs

byte bufNavAbsolu[6]={0,0,0,0,0,0}; // Buffer d'envoi des ordres de navigation absolus
byte crcNavAbsolu = 0 ; // CRC de controle pour les ordres de navigation absolus

byte optionNavigation = 0;
*/
ComNavigation nav ;

int score = 0;
bool matchEnCours = false;

double tempsRestant = TEMPS_MATCH;
double timeInit=0;

// Declaration des variables IHM
IHM interface ;
bool detection = false , strategie = false , typeRobot = ROBOT_PRIMAIRE , equipe = EQUIPE_BLEU ;

//----- FONCTIONS OBLIGATOIRES -----
void setup();
void loop();
//----- INITIALISATION ROBOT -----
void setupActionneur();
void initActionneur();
void sequenceRecalage();
void waitLaunch();
void interfaceLink();
//----- STRATEGIES -----
void homologationPrimaire();
void homologationSecondaire();
void matchPrimaire();
void matchSecondaire();
void testLancerGobelet();
//----- COMMANDES DES ACTIONNEURS -----
void initBrasMancheAir();
void brasMancheAir(bool state);
void brasVentouse(BrasVentouse stateBras);
//----- COMMANDES DE DEPLACEMENT -----
void turnGo(bool adversaire, bool recalage,bool ralentit,int turn, int go);
void turnGo(int turn, int go);
void goTo(bool adversaire, bool recalage,bool ralentit,int X, int Y, int rot);
void goTo(int X, int Y, int rot);
//----- FIN DE MATCH -----
void waitFinMatch();
void finMatch();
//----- AUTRES -----
bool majTemps();
void majScore();
void attente(int temps);

/*
//DEMANDE L'ETAT DU DEPLACEMENT----------------
int askNavigation();
//ENVOI UNE COMMANDE DE DEPLACEMENT ABSOLU----------------
void sendNavigation(byte fonction, int X, int Y, int rot);
//ENVOI UNE COMMANDE DE DEPLACEMENT RELATIF----------------
void sendNavigation(byte fonction, int rot, int dist);
//ENVOI UNE COMMANDE TURN GO----------------
*/
