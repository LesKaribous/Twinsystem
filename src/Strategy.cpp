#include "Strategy.h"
#include "Intercom.h"
#include "Motion.h"
#include "Actuators.h"
#include "Match.h"
#include "Pin.h"

using namespace Motion;
using namespace Match;

//----------------STRATEGIES----------------
namespace Strategy
{
	void grab()
	{
		turnGo(0,-80);
		Actuators::unfold();
		Actuators::open();
		WAIT(500);
		turnGo(0,40);
		Actuators::close();
		Actuators::suck();
		WAIT(500);
		Actuators::fold();
		WAIT(500);
	}

	void sequenceRecalage()
	{
		//Recalage
		Intercom::setDetection(false);
		Intercom::setRecalibration(false);
		Intercom::slowMode(true);
		
		turnGo(0, -200);   //Toward +X (   ? ;  ?  )
		turnGo(0, -20);	   //Toward +X ( 110 ;  ?  ) // X0 + BC
		turnGo(0, 100);	   //Toward +X ( 210 ;  ?  ) // X110 + X100
		turnGo(90, -1000); //Toward -Y ( 210 ;  ?  ) //
		turnGo(0, -20);	   //Toward -Y ( 210 ; 1890) //Y2000 - BC
		turnGo(0, 920);	   //Toward -Y ( 210 ; 970 ) //Y1890 - Y920
		turnGo(-90, -200); //Toward +X ( 110 ; 970 ) //X0 - BC
		turnGo(0, 30);	   //Toward +X ( 130 ; 970 ) //X0 + 30
		turnGo(-90, 0);	   //Toward +Y ( 130 ; 970 )
		
	}

	void homologationPrimaire()
	{
		Intercom::setDetection(false); 	//Toward +Y ( 130 ; 970 )
		Intercom::setRecalibration(false);
		Intercom::slowMode(false);
		turnGo(0, 1000);				//Toward +Y ( 130 ; 1970 )
		turnGo(0, -70);					//Toward +Y ( 130 ; 1900 )
		Actuators::deployArm(true); 				//Ouverture bras
		Intercom::slowMode(true);
		turnGo(90, 500); 				//Toward +X ( 630 ; 970 ) //old 90 700
		//turn ?
		Actuators::deployArm(false);	//Remonte bras
		Intercom::slowMode(false);
		turnGo(90, -50);				// Recalage   //Toward -Y ( 130 ; 1020 )
		turnGo(0, 150);					// Face distributeur
		turnGo(90, 600);				// Go distributeur //old 800
		turnGo(0, -40);					//step backward before grab
		grab();							// Prendre deux gobelets
		turnGo(0, -100);
		turnGo(-90, 0);
		turnGo(0, 800);
		Actuators::release(Actuators::Side::BOTH); // Dépose deux gobelets
		attente(100000);

		waitFinMatch();
	}

	void matchPrimaire()
	{

		waitFinMatch();
	}

	void waitLaunch()
	{
		// Gestion tirette
		while (IHM::getTirette())
		{
			IHM::updateButtonIHM();
			Intercom::setTeam(IHM::getEquipe());
			//interfaceLink();
			if (!IHM::getCheck())
			{
				IHM::LCD::initScreen();
				sequenceRecalage();
				IHM::setRecalage(true);
			}
			IHM::LCD::menuScreen();
		}
		while (!IHM::getTirette())
		{
			IHM::updateButtonIHM();
			Intercom::setTeam(IHM::getEquipe());
			if (!IHM::getCheck())
			{
				IHM::LCD::initScreen();
				sequenceRecalage();
				IHM::setRecalage(true);
				//IHM::checkListScreen();
			}
			IHM::LCD::menuScreen();
		}

		// Lancement du Match
		Match::start();
		IHM::LCD::goScreen();
		// Enregistrement des paramètres de match
		IHM::freezeSettings();
		// Envois des paramètres à la ComNavigation
		Intercom::setGlobalDetection(IHM::getDetection());
		Intercom::setTeam(IHM::getEquipe());
		// Démarrage de la balise
		digitalWrite(Pin::Beacon, HIGH);
		// Delai pour affichage
		delay(100);
		
	}
}