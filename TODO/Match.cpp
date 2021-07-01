#include "Match.h"

#include "Match.h"



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
