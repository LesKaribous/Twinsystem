#include "Strategy.h"

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

void Strategy::waitLaunch(){
    // Gestion tirette
    while (interface.getTirette())
    {
        interface.updateButtonIHM();
        equipe = interface.getEquipe();
        nav.setTeam(equipe);
        interfaceLink();
        if (!interface.getCheck())
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
        if (!interface.getCheck())
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
    digitalWrite(pinBeacon, HIGH);
    // Delai pour affichage
    delay(100);
}