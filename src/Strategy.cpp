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
		
	}

	void homologation(){
		SetRelative();
		go(100,0);
		go(-100,0);
		go(0,100);
		go(0,-100);

		SetAbsolute();
		go(100,0);
		go(0,0);
		go(0,100);
		go(0,0);

		SetAbsolute();
		go(100,0,60);
		go(0,0,0);
		go(0,100,-60);
		go(0,0);
	}

	void recalage(){
		
	}

	void testingFlip(){
		Actuators::takeElement(Actuators::BrasAU,FLOOR);
		delay(1000);
		Actuators::flipElement(Actuators::BrasAU);
		delay(1000);
		Actuators::releaseElement(Actuators::BrasAU,FLOOR);
	}

	void testingActuators(){
		Actuators::BrasInit.setPosition(0,100,50);
		Actuators::BrasAU.setPosition(0,100,50);
		Actuators::BrasTirette.setPosition(0,100,50);
		IHM::waitCheck();
		Actuators::BrasInit.setPosition(0,50,100);
		Actuators::BrasAU.setPosition(0,50,100);
		Actuators::BrasTirette.setPosition(0,50,100);
		IHM::waitCheck();
		Actuators::BrasInit.setPosition(0,50,0);
		Actuators::BrasAU.setPosition(0,50,0);
		Actuators::BrasTirette.setPosition(0,50,0);
		IHM::waitCheck();
		Actuators::BrasInit.setPosition(0,50,50);
		Actuators::BrasAU.setPosition(0,50,50);
		Actuators::BrasTirette.setPosition(0,50,50);
		IHM::waitCheck();
		Actuators::BrasInit.setPosition(0,100,50);
		Actuators::BrasAU.setPosition(0,100,50);
		Actuators::BrasTirette.setPosition(0,100,50);
		IHM::waitCheck();
		Actuators::BrasInit.setPosition(100,100,50);
		Actuators::BrasAU.setPosition(100,100,50);
		Actuators::BrasTirette.setPosition(100,100,50);
		IHM::waitCheck();
		Actuators::BrasInit.setPosition(0,100,50);
		Actuators::BrasAU.setPosition(0,100,50);
		Actuators::BrasTirette.setPosition(0,100,50);
		IHM::waitCheck();
		Actuators::BrasInit.setPosition(0,0,50);
		Actuators::BrasAU.setPosition(0,0,50);
		Actuators::BrasTirette.setPosition(0,0,50);
		IHM::waitCheck();
		Actuators::BrasInit.detachBras();
		Actuators::BrasAU.detachBras();
		Actuators::BrasTirette.detachBras();
		IHM::waitCheck();
	}

	void waitLaunch()
	{
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