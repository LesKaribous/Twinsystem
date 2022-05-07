#include "Match.h"
#include "Twinsystem.h"

namespace Match{

    double tempsRestant = Settings::TEMPS_MATCH;
    double timeInit = 0;
    int score = 0;
   	State state = State::INIT;

	void start()
	{
		timeInit = millis();
		state = State::RUNNING;
	}

	void updateScore(int points, int multiplicateur)
	{
		score = score + (points * multiplicateur);
	}

	int GetScore(){
		return score;
	}
    double GetTempsRestant(){
		return tempsRestant;
	}

	void update(){
		updateTime();
		IHM::LCD::matchScreen();
	}

	//----------------MISE A JOUR DU TEMPS DE MATCH----------------
	bool updateTime()
	{
		if(Match::hasStarted){ //Do not update time before match has started
			tempsRestant = (Settings::TEMPS_MATCH - (millis() - timeInit)) / 1000;

			if (tempsRestant <= 0)
			{
				Match::end();
				return true;
			}
			else if (tempsRestant <= 4)
			{
				//Last action ?
				return false;
			}
			else
				return false;
		}
	}

	//----------------PROCEDURE D'ATTENTE----------------
	void wait(int temps)
	{
		int initTemps = millis();
		while ((millis() - initTemps) <= temps)
		{
			if (state == State::RUNNING)
			{
				updateTime();
				IHM::LCD::matchScreen();
			}
		}
	}

	//----------------PROCEDURE DE FIN DE MATCH----------------
	void waitEnd()
	{
		//Stopper la balise
		while (!updateTime())
		{
			//digitalWrite(Pin::Beacon,LOW);
			IHM::LCD::matchScreen();
		}
	}

	

	void end()
	{
		IHM::LCD::matchScreen();
		// Stopper les moteurs
		Controller::stop();
		Controller::disengage();
		// Stopper les actionneurs
		Actuators::unsuck();
		Actuators::sleep();
		// Boucle infinie
		while(true){
			Debugger::checkSerial();
		}
	}
}