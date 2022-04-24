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
		go({100,0,0});
		goTo({0,0,0});
	}

	void homologation(){
		Actuators::takeElement(Actuators::BrasAU,WORK_SHED);
		go({100,0,0});
		go({0,0,-60});
		Actuators::takeElement(Actuators::BrasTirette,WORK_SHED);

	}

	void recalage(){

	}


	void waitLaunch()
	{
		IHM::menu();

    	while(!IHM::getTirette())IHM::menu();
    	while( IHM::getTirette())IHM::menu();

		// Lancement du Match
		Match::start();
    	IHM::LCD::goScreen();
		// Enregistrement des paramètres de match
		IHM::freezeSettings();
		// Delais d'affichage
		delay(100);
		//Match::update();

	}
}