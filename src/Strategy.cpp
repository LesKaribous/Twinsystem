#include "Strategy.h"
#include "Intercom.h"
#include "Motion.h"
#include "Actuators.h"
#include "Match.h"
#include "Pin.h"

using namespace Motion;
using namespace Match;
using namespace Actuators;

//----------------STRATEGIES----------------
namespace Strategy
{
	bool TestBrasDeployed = false;

	//Warning Backward -100
	void grab(Side side){
		unfold(side);
		WAIT(600);
		suck(side);
		close(side);
		WAIT(900);
		fold(side);
	}

	void release(Side side){
		unfold(side);
		WAIT(1000);
		unsuck(side);
		open(side);
		WAIT(600);
		//turnGo(0,-100);
		//fold(side);
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
		turnGo(0, 60);	   //Toward +X ( 130 ; 970 ) //X0 + 30
		turnGo(-90, 0);	   //Toward +Y ( 130 ; 970 )
		
	}

	void homologationPrimaire()
	{
		Intercom::setDetection(true); 	//Toward +Y ( 130 ; 970 )
		Intercom::setRecalibration(false);
		Intercom::slowMode(false);
		turnGo(0, 1000);				//Toward +Y ( 130 ; 1970 )
		turnGo(0, -65);					//Toward +Y ( 130 ; 1900 )
		deployArm(true); 				//Ouverture bras
		Intercom::slowMode(true);
		turnGo(90, -100); 				//Toward +X ( 630 ; 970 ) //old 90 700
		turnGo(0,600);
		Match::majScore(15,1);
		deployArm(false);	//Remonte bras
		Intercom::slowMode(false);
		turnGo(90, -60);				// Recalage   //Toward -Y ( 130 ; 1020 )
		turnGo(0, 170);					// Face distributeur
		turnGo(90, 630);				// Go distributeur //old 800
		turnGo(0, -100);
		turnGo(-90, 0);
		turnGo(0, 400);
		Match::majScore(3,1);
		//Actuators::release(Actuators::Side::BOTH); // Dépose deux gobelets
		waitFinMatch();
	}

	void matchPrimaire()
	{
		Intercom::setDetection(true); 	//Toward +Y ( 130 ; 970 )
		Intercom::setRecalibration(false);
		Intercom::slowMode(false);
		turnGo(0, 1000);				//Recalage Y+
		turnGo(0, -65); //65
		//Manche à air
		turnGo(-90, 150);
		deployArm(true,false,!IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0,-690);
		turnGo(270, -110);				// Recalage  -Y ( 130 ; 1020 )
		deployArm(false,false,!IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		//Fin Manche à air
		Match::majScore(15,1);
		

		Intercom::slowMode(false);
		turnGo(0, 180);
		turnGo(90, 700);
		turnGo(0, -80);
		Intercom::slowMode(true);
		arming();
		turnGo(0,60);
		grab();
		turnGo(0,-250);
		Intercom::slowMode(false);
		turnGo(-90, 0);
		turnGo(0, 690);//bof
		turnGo(0, -200);
		Match::majScore(2,1); //Gobelet dans le port
		Match::majScore(1,1); //Gobelet bonne couleur
		Match::majScore(1,2); //Pair
		release(); // Dépose deux gobelets
		turnGo(0,-100);
		fold();
		
		turnGo(0,-180);
		turnGo(90, 300);
		turnGo(0, -90);
		Intercom::slowMode(true);
		arming();
		turnGo(0,60);
		grab();
		turnGo(0, -80);
		turnGo(-90, 0);
		turnGo(0, 350);
		release(!IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0, -30);
		fold(!IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0, -50);
		release(IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0, -50);
		fold(IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0, -50);


		Match::majScore(2,1); //Gobelet dans le port
		Match::majScore(1,1); //Gobelet bonne couleur
		Match::majScore(1,2); //Pair

		turnGo(0,-250);
		turnGo(90, 200);
		turnGo(0, -80);
		Intercom::slowMode(true);
		arming(!IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0,60);
		grab(!IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		fold(!IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0, -80);
		turnGo(-90, 0);
		turnGo(0, 310);
		unfold(!IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0, 50);
		release(!IHM::getEquipe() ? Side::LEFT : Side::RIGHT); // Dépose le gobelets rouge
		turnGo(0,-40);
		fold(!IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		Match::majScore(2,1); //Gobelet dans le port
		Match::majScore(1,1); //Gobelet couleur
		//Match::majScore(1,2); //Pair

		waitFinMatch();
	}

/*
		Intercom::setDetection(true); 	//Toward +Y ( 130 ; 970 )
		Intercom::setRecalibration(false);
		Intercom::slowMode(false);
		turnGo(0, 1000);				//Toward +Y ( 130 ; 1970 )
		turnGo(0, -65);					//Toward +Y ( 130 ; 1900 )
		deployArm(true); 				//Ouverture bras
		Intercom::slowMode(true);
		turnGo(90, -100); 				//Toward +X ( 630 ; 970 ) //old 90 700
		turnGo(0,600);
		Match::majScore(15,1);
		deployArm(false);	//Remonte bras
		Intercom::slowMode(false);
		turnGo(90, -60);				// Recalage   //Toward -Y ( 130 ; 1020 )
		turnGo(0, 170);					// Face distributeur
		turnGo(90, 630);				// Go distributeur //old 800
		turnGo(0, -100);
		turnGo(-90, 0);
		turnGo(0, 400);
		Match::majScore(3,1);
		//Actuators::release(Actuators::Side::BOTH); // Dépose deux gobelets
		waitFinMatch();
	}

	void matchPrimaire()
	{
		Intercom::setDetection(true); 	//Toward +Y ( 130 ; 970 )
		Intercom::setRecalibration(false);
		Intercom::slowMode(false);
		turnGo(0, 1000);				//Recalage Y+
		turnGo(0, -70); //65
		//Manche à air
		deployArm(true);
		turnGo(90, -100); 
		turnGo(0,630);
		deployArm(false);
		//Fin Manche à air
		Match::majScore(15,1);
		

		Intercom::slowMode(false);
		turnGo(90, -80);				// Recalage  -Y ( 130 ; 1020 )
		turnGo(0, 180);
		turnGo(90, 700);
		turnGo(0, -80);
		Intercom::slowMode(true);
		arming();
		turnGo(0,60);
		grab();
		fold();
		turnGo(0,-250);
		Intercom::slowMode(false);
		turnGo(-90, 0);
		turnGo(0, 710);//bof
		turnGo(0, -200);
		Match::majScore(2,1); //Gobelet dans le port
		Match::majScore(1,1); //Gobelet bonne couleur
		Match::majScore(1,2); //Pair
		release(); // Dépose deux gobelets
		turnGo(0,-100);
		fold();

		turnGo(0,-180);
		turnGo(90, 300);
		turnGo(0, -80);
		Intercom::slowMode(true);
		arming();
		turnGo(0,60);
		grab();
		turnGo(0, -80);
		turnGo(-90, 0);

		//turnGo(0, 270);
		//release(); // Dépose deux gobelets
		//turnGo(0,-100);
		//fold();
		turnGo(0, 350);
		release(IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0, -30);
		fold(IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0, -50);
		release(IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0, -50);
		fold(IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0, -50);


		Match::majScore(2,1); //Gobelet dans le port
		Match::majScore(1,1); //Gobelet bonne couleur
		Match::majScore(1,2); //Pair

		turnGo(0,-250);
		turnGo(90, 200);
		turnGo(0, -80);
		Intercom::slowMode(true);
		arming(IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0,60);
		grab(IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		fold(IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0, -80);
		turnGo(-90, 0);
		turnGo(0, 310);
		unfold(IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		turnGo(0, 50);
		release(IHM::getEquipe() ? Side::LEFT : Side::RIGHT); // Dépose le gobelets rouge
		turnGo(0,-40);
		fold(IHM::getEquipe() ? Side::LEFT : Side::RIGHT);
		Match::majScore(2,1); //Gobelet dans le port
		Match::majScore(1,1); //Gobelet couleur
		//Match::majScore(1,2); //Pair

		waitFinMatch();


*/

	void waitLaunch()
	{
		Match::state = Match::State::READY;
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
			
			#ifdef TESTARM
				if(!IHM::getTestBras() && !TestBrasDeployed){
					Actuators::deployArm(true, true);
					TestBrasDeployed = true;
				}
				if(IHM::getTestBras() && TestBrasDeployed){
					Actuators::deployArm(false);
					TestBrasDeployed = false;
				}
			#endif

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