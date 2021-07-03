#include "Match.h"
#include "Actuators.h"
#include "Pin.h"
#include "Intercom.h"

namespace Match{

    double tempsRestant = Setting::TEMPS_MATCH;
    double timeInit = 0;
    int score = 0;
    bool matchEnCours = false;

	void start()
	{
		timeInit = millis();
		matchEnCours = true;
	}


	void majScore(int points, int multiplicateur)
	{
		score = score + (points * multiplicateur);
	}

	//----------------MISE A JOUR DU TEMPS DE MATCH----------------
	bool majTemps()
	{
		tempsRestant = (Setting::TEMPS_MATCH - (millis() - timeInit)) / 1000;

		if (tempsRestant <= 0)
		{
			finMatch();
			return true;
		}
		else if (tempsRestant <= 5)
		{
			Actuators::servoDrapeau.attach(Pin::ServoDrapeau);
			Actuators::servoDrapeau.write(90);
			return false;
		}
		else
			return false;
	}

	//----------------PROCEDURE D'ATTENTE----------------
	void attente(int temps)
	{

		int initTemps = millis();

		while ((millis() - initTemps) <= temps)
		{
			if (matchEnCours)
			{
				majTemps();
				IHM::LCD::matchScreen(score, tempsRestant, Intercom::getNbrBadCRC());
			}
		}
	}

	//----------------PROCEDURE DE FIN DE MATCH----------------
	void waitFinMatch()
	{
		digitalWrite(Pin::Beacon, LOW);
		while (!majTemps())
		{
			//digitalWrite(Pin::Beacon,LOW);
			IHM::LCD::matchScreen(score, tempsRestant, Intercom::getNbrBadCRC());
		}
	}

	void finMatch()
	{
		// Stopper les moteurs
		Intercom::sendNavigation(255, 0, 0);
		// Stopper la Balise
		digitalWrite(Pin::Beacon, LOW);
		// Boucle infinie
		while (1)
		{
			// Stopper les moteurs
			Intercom::sendNavigation(255, 0, 0);
			// Stoppe la Balise
			digitalWrite(Pin::Beacon,LOW);
		}
	}
}