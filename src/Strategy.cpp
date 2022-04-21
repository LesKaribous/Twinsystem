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

	void Homologation(){

	}


	void waitLaunch()
	{
		IHM::menu();

    	while(!IHM::getTirette())   {IHM::menu();}
    	while( IHM::getTirette())   {IHM::menu();}
		// Lancement du Match
		Match::start();
    	IHM::LCD::goScreen();
		// Enregistrement des paramètres de match
		IHM::freezeSettings();
		// Delais d'affichage
		delay(100);

	}
}