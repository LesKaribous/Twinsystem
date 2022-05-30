#include "Match.h"
#include "Twinsystem.h"

namespace Match{

    double tempsRestant = Settings::TEMPS_MATCH;
    double timeInit = 0;
    int score = 0;
   	State state = State::INIT;

	void start(){
		IHM::start();
		timeInit = millis();
		state = State::RUNNING;
	}

	void setReady()
	{
		state = State::READY;
	}

	void updateScore(int points, int multiplicateur = 1)
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
	}

	//----------------MISE A JOUR DU TEMPS DE MATCH----------------
	bool updateTime(){
		if(Match::hasStarted()){ //Do not update time before match has started
			tempsRestant = (Settings::TEMPS_MATCH - (millis() - timeInit)) / 1000;

			if (tempsRestant <= 0){
				tempsRestant = 0;
				Match::end();
				return true;
			}
			else if (tempsRestant <= 5 && state != State::HOMING){
				state = State::HOMING;
				Strategy::goHome();
				Match::end();
				return false;
			}
			else
				return false;
		}
		return false;
	}

	//----------------PROCEDURE DE FIN DE MATCH----------------
	void waitEnd(){
		while (!updateTime())
		{
			//digitalWrite(Pin::Beacon,LOW);
			IHM::LCD::matchScreen();
		}
	}

	

	void end()
	{
		Debugger::log("Fin de match !");
		IHM::LCD::matchScreen();
		// Stopper les moteurs
		Controller::stop();
		Controller::disengage();
		// Stopper les actionneurs
		Actuators::unsuck();
		Actuators::sleep();
		IHM::Sound::playSound(BATON_SOUND);
		// Boucle infinie
		while(true){
			Debugger::checkSerial();
		}
	}
}