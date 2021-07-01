#include "Strategie.h"


void setup(){
  Serial.begin(115200);     //Demarrage d'un liaison série pour le debug
  Wire.begin();             //Demarrage de la liaison I2C
  delay(500);               //Pause de demarrage avant lancement
  interface.splashScreen(); //Affichage du Logo des Karibous
  setupActionneur();        //Setup des actionneurs
  initActionneur();         //Initialisation des actionneurs
  delay(1000);              //Attente affichage logo
  waitLaunch();             //Attendre le début du match
}

void loop(){
  if (strategie == STRATEGIE_HOMOLOGATION) homologationPrimaire();
  else matchPrimaire(); // Match
}



void waitLaunch(){
  // Gestion tirette
  while (interface.getTirette())
  {
    interface.updateButtonIHM();
    equipe = interface.getEquipe();
    nav.setTeam(equipe);
    interfaceLink();
    if(!interface.getCheck())
    {
      interface.initScreen();
      sequenceRecalage();
      interface.setRecalage(true);
      //interface.checkListScreen();
    }
    interface.menuScreen();
  }
  while (!interface.getTirette())
  {
    interface.updateButtonIHM();
    equipe = interface.getEquipe();
    nav.setTeam(equipe);
    if(!interface.getCheck())
    {
      interface.initScreen();
      sequenceRecalage();
      interface.setRecalage(true);
      //interface.checkListScreen();
    }
    interface.menuScreen();
  }

  // Lancement du Match
  matchEnCours = true;
	timeInit = millis();
  interface.goScreen();
  // Enregistrement des paramètres de match
  typeRobot = ROBOT_PRIMAIRE;
  strategie = interface.getStrategie();
  detection = interface.getDetection();
  equipe = interface.getEquipe();
  // Envois des paramètres à la ComNavigation
  nav.setGlobalDetection(detection);
  nav.setTeam(equipe);
  // Démarrage de la balise
  digitalWrite(pinBeacon,HIGH);
  // Delai pour affichage
	delay (100);
}

//----------------INIT ACTIONNEUR-------------

void setupActionneur(){
  // Init des bras
  initBrasMancheAir();

    // Init du drapeau
  servoDrapeau.attach(pinServoDrapeau);
  servoDrapeau.write(0);

  // Initialisation des bras
  brasDroit.setPin(pinServoDroit,pinServoVentouseDroit,pinPompeDroit,pinEVDroit);
  brasGauche.setPin(pinServoGauche,pinServoVentouseGauche,pinPompeGauche,pinEVGauche);
  // Initialisation de la pin pinBalise
  pinMode(pinBeacon,OUTPUT);
  digitalWrite(pinBeacon,LOW);

  brasDroit.setLimit(35,140,10,90);
  brasGauche.setLimit(110,10,180,70);
  brasDroit.setPosition(0,0,LOW,LOW,0);
  brasGauche.setPosition(0,0,LOW,LOW,2000);

  servoDrapeau.detach();
  servoBrasDroit.detach();
  servoBrasGauche.detach();
}

void initActionneur(){

    brasDroit.setPosition(0,0,LOW,LOW,0);
    brasGauche.setPosition(0,0,LOW,LOW,2000);

    brasDroit.setPosition(100,100,HIGH,LOW,0);
    brasGauche.setPosition(100,100,HIGH,LOW,2000);

    brasDroit.setPosition(0,0,LOW,LOW,0);
    brasGauche.setPosition(0,0,LOW,LOW,2000);

    servoBrasDroit.attach(pinServoBrasDroit);
    servoBrasGauche.attach(pinServoBrasGauche);

    initBrasMancheAir();

    servoBrasDroit.detach();
    servoBrasGauche.detach();

}

//----------------STRATEGIES----------------

void sequenceRecalage(){
  //Recalage
  nav.setDetection(false);
  nav.setRecalibration(false);
  nav.slowMode(true);
  turnGo(0,-200);   //Toward +X (   ? ;  ?  )
  turnGo(0,-20);    //Toward +X ( 110 ;  ?  ) // X0 + BC
  turnGo(0,100);    //Toward +X ( 210 ;  ?  ) // X110 + X100
  turnGo(90,-1000); //Toward -Y ( 210 ;  ?  ) // 
  turnGo(0,-20);    //Toward -Y ( 210 ; 1890) //Y2000 - BC
  turnGo(0,920);    //Toward -Y ( 210 ; 970 ) //Y1890 - Y920
  turnGo(-90,-200); //Toward +X ( 110 ; 970 ) //X0 - BC
  turnGo(0,30);     //Toward +X ( 130 ; 970 ) //X0 + 30
  turnGo(-90,0);    //Toward +Y ( 130 ; 970 )
}

void homologationPrimaire(){
  nav.setDetection(false);          //Toward +Y ( 130 ; 970 )
  nav.setRecalibration(false);      
  nav.slowMode(false);
  turnGo(0,1000);                   //Toward +Y ( 130 ; 1970 )
  .......................................................................................................
  turnGo(0,-70);                    //Toward +Y ( 130 ; 1900 )
  brasMancheAir(true);              //Ouverture bras
  nav.slowMode(true);
  turnGo(90,500);                   //Toward +X ( 630 ; 970 ) //old 90 700
  //turn ?
  brasMancheAir(false);             //Remonte bras
  nav.slowMode(false);
  turnGo(90,-50);     // Recalage   //Toward -Y ( 130 ; 1020 )
  turnGo(0,150);                    // Face distributeur
  turnGo(90,600);                   // Go distributeur //old 800
  brasVentouse(PRISE_ENSEMBLE);     // Prendre deux gobelets
  turnGo(0,-100);
  turnGo(-90,0);
  turnGo(0,800);
  brasVentouse(DEPOSE_ENSEMBLE);    // Dépose deux gobelets
  attente(100000);

  waitFinMatch();
}

void matchPrimaire(){

  waitFinMatch();
}

void brasVentouse(BrasVentouse stateBras)
{
  switch (stateBras)
  {
    case PRISE_DROITE:
      brasDroit.setPosition(100,100,HIGH,LOW,0);
      attente(200);
      brasDroit.setPosition(0,0,HIGH,LOW,0);
      attente(200);
    break;
    case PRISE_GAUCHE:
      brasGauche.setPosition(100,100,HIGH,LOW,0);
      attente(200);
      brasGauche.setPosition(0,0,HIGH,LOW,0);
      attente(200);
    break;
    case DEPOSE_DROITE:
      brasDroit.setPosition(100,100,HIGH,LOW,0);
      attente(200);
      brasDroit.setPosition(100,100,LOW,HIGH,0);
      attente(200);
      brasDroit.setPosition(0,0,LOW,LOW,0);
      attente(200);
    break;
    case DEPOSE_GAUCHE:
      brasGauche.setPosition(100,100,HIGH,LOW,0);
      attente(200);
      brasGauche.setPosition(100,100,LOW,HIGH,0);
      attente(200);
      brasGauche.setPosition(0,0,LOW,LOW,0);
      attente(200);
    break;
    case PRISE_ENSEMBLE:
      brasDroit.setPosition(100,100,HIGH,LOW,0);
      brasGauche.setPosition(100,100,HIGH,LOW,0);
      attente(200);
      brasDroit.setPosition(0,0,HIGH,LOW,0);
      brasGauche.setPosition(0,0,HIGH,LOW,0);
      attente(200);
    break;
    case DEPOSE_ENSEMBLE:
      brasDroit.setPosition(100,100,HIGH,LOW,0);
      brasGauche.setPosition(100,100,HIGH,LOW,0);
      attente(200);
      brasDroit.setPosition(100,100,LOW,HIGH,0);
      brasGauche.setPosition(100,100,LOW,HIGH,0);
      attente(200);
      brasDroit.setPosition(0,0,LOW,LOW,0);
      brasGauche.setPosition(0,0,LOW,LOW,0);
      attente(200);
    break;
    case REPOS:
      brasDroit.setPosition(0,0,LOW,LOW,0);
      brasGauche.setPosition(0,0,LOW,LOW,0);
    break;
  }
}

void initBrasMancheAir(){
  servoBrasDroit.attach(pinServoBrasDroit);
  servoBrasGauche.attach(pinServoBrasGauche);

  servoBrasDroit.write(POS_BRAS_D_BAS);
  servoBrasGauche.write(POS_BRAS_G_BAS);
 
  delay(800);

  servoBrasDroit.write(POS_BRAS_D_HAUT);
  servoBrasGauche.write(POS_BRAS_G_HAUT);

  delay(800);
  servoBrasDroit.detach();
  servoBrasGauche.detach();
}

void brasMancheAir(bool state)
{
  if(equipe==EQUIPE_BLEU) servoBrasDroit.attach(pinServoBrasDroit);
  else servoBrasGauche.attach(pinServoBrasGauche);

  if(state)
  {
    if(equipe==EQUIPE_BLEU) servoBrasDroit.write(POS_BRAS_D_BAS);
    else servoBrasGauche.write(POS_BRAS_G_BAS);
  }
  else
  {
    if(equipe==EQUIPE_BLEU) servoBrasDroit.write(POS_BRAS_D_HAUT);
    else servoBrasGauche.write(POS_BRAS_G_HAUT);
  }
  delay(800);
  if(equipe==EQUIPE_BLEU) servoBrasDroit.detach();
  if(equipe==EQUIPE_BLEU) servoBrasGauche.detach();
}

//----------------PROCEDURE D'ATTENTE----------------
void attente(int temps){

	int initTemps = millis();

	while( (millis()-initTemps) <= temps)
	{
		if(matchEnCours)
    {
      majTemps();
		  interface.matchScreen(score,tempsRestant,navError);
    }
	}
}

void majScore(int points, int multiplicateur){
	score = score + (points*multiplicateur);
}

//----------------ENVOI UNE COMMANDE TURN GO----------------
void turnGo(int turn, int go)
{
  nav.turnGo(turn,go);
  attente(100);
  int reponseNavigation = nav.askNavigation();
	while(reponseNavigation!=TERMINEE)
	{
    if (reponseNavigation==ERRONEE || reponseNavigation==BIZARRE)
    {
      nav.turnGo(turn,go);
      navError++;
    }
    attente(200);
    reponseNavigation = nav.askNavigation();
	}

}

void turnGo(bool adversaire, bool recalage,bool ralentit,int turn, int go)
{
	nav.turnGo(adversaire,recalage,ralentit,turn,go);
  attente(100);
  int reponseNavigation = nav.askNavigation();
	while(reponseNavigation!=TERMINEE)
	{
    if (reponseNavigation==ERRONEE || reponseNavigation==BIZARRE)
    {
      nav.turnGo(adversaire,recalage,ralentit,turn,go);
      navError++;
    }
    attente(200);
    reponseNavigation = nav.askNavigation();
	}

}

void goTo  (int X, int Y, int rot)
{
  nav.goTo(X,Y,rot);
  int reponseNavigation = nav.askNavigation();
	while(reponseNavigation!=TERMINEE)
	{
    if (reponseNavigation==ERRONEE)
    {
      nav.goTo(X,Y,rot);
      navError++;
    }
    attente(200);
    reponseNavigation = nav.askNavigation();
	}
}

void turnGo(bool adversaire, bool recalage,bool ralentit,int X, int Y, int rot)
{
  nav.goTo(adversaire,recalage,ralentit,X,Y,rot);
  int reponseNavigation = nav.askNavigation();
  while(reponseNavigation!=TERMINEE)
  {
    if (reponseNavigation==ERRONEE)
    {
      nav.goTo(adversaire,recalage,ralentit,X,Y,rot);
      navError++;
    }
    attente(200);
    reponseNavigation = nav.askNavigation();
  }
}

//----------------MISE A JOUR DU TEMPS DE MATCH----------------
bool majTemps(){
  tempsRestant = ( TEMPS_MATCH - (millis() - timeInit) ) / 1000;

  if ( tempsRestant <= 0 )
  {
    finMatch();
    return true;
  }
  else if ( tempsRestant <= 5 )
  {
    servoDrapeau.attach(pinServoDrapeau);
    servoDrapeau.write(90);
    return false;
  }
  else return false;
}

//----------------PROCEDURE DE FIN DE MATCH----------------
void waitFinMatch()
{
  while (!majTemps())
  {
    interface.matchScreen(score,tempsRestant,navError);
  }
}

void finMatch(){
	// Stopper les moteurs
	nav.sendNavigation(255, 0, 0);
  // Stopper la Balise
  digitalWrite(pinBeacon,LOW);
	// Boucle infinie
	while(1)
	{
		// Stopper les moteurs
		nav.sendNavigation(255, 0, 0);
    // Stoppe la Balise
    //digitalWrite(pinBalise,LOW);
	}
}
