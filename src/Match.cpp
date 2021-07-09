#include "Match.h"
#include "Actuators.h"
#include "Pin.h"
#include "Intercom.h"

namespace Match{

    double tempsRestant = Setting::TEMPS_MATCH;
    double timeInit = 0;
    int score = 0;
   	State state = State::INIT;
	bool pavillonDeployed = false;

	void start()
	{
		timeInit = millis();
		state = State::RUNNING;
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
		else if (tempsRestant <= 4)
		{
			if(!pavillonDeployed){
				Actuators::servoDrapeau.attach(Pin::ServoDrapeau);
				Actuators::servoDrapeau.write(90);
				majScore(10,1);
				pavillonDeployed = true;
			}
			IHM::LCD::matchScreen(score, tempsRestant, Intercom::getNbrBadCRC());
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
			if (state == State::RUNNING)
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
		IHM::LCD::matchScreen(score, tempsRestant, Intercom::getNbrBadCRC());
		// Stopper les moteurs
		Intercom::sendNavigation(255, 0, 0);
		// Stopper la Balise
		digitalWrite(Pin::Beacon, LOW);
		Actuators::unsuck();
		Actuators::sleep();
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