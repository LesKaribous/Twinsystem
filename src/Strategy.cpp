#include "Strategy.h"
#include "Intercom.h"
#include "Motion.h"
#include "Actuators.h"
#include "Match.h"
#include "Controller.h"
#include "Pin.h"

using namespace Motion;
using namespace Match;
using namespace Actuators;

//----------------STRATEGIES----------------
namespace Strategy{


	void match(){
		Motion::goTo(100,0);
	}

	void Homologation(){

	}


	void waitLaunch()
	{
		//Tirette and Screen routine
		/*Match::state = Match::State::READY;
		while(Match::state != Match::State::RUNNING){
			
			IHM::updateButtonIHM();
			Intercom::setTeam(IHM::getEquipe());
		
			if(!IHM::getTirette()){
				Match::state = Match::State::ARMED;
			}
			if (Match::state == Match::State::ARMED){
				if(IHM::getTirette()){
					Match::state = Match::State::RUNNING;
				}
			}

			//interfaceLink();
			if (!IHM::getCheck())
			{
				IHM::LCD::initScreen();
				Match::state = Match::State::READY;
				sequenceRecalage();
				IHM::setRecalage(true);
			}

			IHM::LCD::menuScreen();
		}
		*/



		// Lancement du Match
		Match::start();
		//IHM::LCD::goScreen();
		// Enregistrement des paramètres de match
		//IHM::freezeInputs();
		// Démarrage de la balise

		// Delai pour affichage
		delay(100);
	}
}