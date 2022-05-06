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
	}

	void recalage(){
		turn(30);
		Vec2 borderPos(0,-100);
		probeBorder(borderPos);
	}

	void testingFlip(){
		takeElement(BrasAU,FLOOR);
		delay(1000);
		flipElement(BrasAU);
		delay(1000);
		releaseElement(BrasAU,FLOOR);
	}

	void testingActuators(){
		BrasInit.setPosition(0,100,50);
		BrasAU.setPosition(0,100,50);
		BrasTirette.setPosition(0,100,50);
		IHM::waitCheck();
		BrasInit.setPosition(0,50,100);
		BrasAU.setPosition(0,50,100);
		BrasTirette.setPosition(0,50,100);
		IHM::waitCheck();
		BrasInit.setPosition(0,50,0);
		BrasAU.setPosition(0,50,0);
		BrasTirette.setPosition(0,50,0);
		IHM::waitCheck();
		BrasInit.setPosition(0,50,50);
		BrasAU.setPosition(0,50,50);
		BrasTirette.setPosition(0,50,50);
		IHM::waitCheck();
		BrasInit.setPosition(0,100,50);
		BrasAU.setPosition(0,100,50);
		BrasTirette.setPosition(0,100,50);
		IHM::waitCheck();
		BrasInit.setPosition(100,100,50);
		BrasAU.setPosition(100,100,50);
		BrasTirette.setPosition(100,100,50);
		IHM::waitCheck();
		BrasInit.setPosition(0,100,50);
		BrasAU.setPosition(0,100,50);
		BrasTirette.setPosition(0,100,50);
		IHM::waitCheck();
		BrasInit.setPosition(0,0,50);
		BrasAU.setPosition(0,0,50);
		BrasTirette.setPosition(0,0,50);
		IHM::waitCheck();
		BrasInit.detachBras();
		BrasAU.detachBras();
		BrasTirette.detachBras();
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

	
	void takeElement(Bras robotArm,int location)
	{
		// Arming the arm	
		switch(location){
			case FLOOR :
				
				// Arming the arm
				robotArm.grab();
				robotArm.setPosition(100,100,80,1000);
				// Take an element lay on the floor
				robotArm.setPosition(35,100,80,1000);
			break;
			case DISPENSER :
				// Arming the arm
				robotArm.grab();
				robotArm.setPosition(100,100,80,1000);
				// Take an element lay on a 60° dispenser
			break;
			case WORK_SHED :
				// Arming the arm
				robotArm.grab();
				robotArm.setPosition(100,100,80,1000);
				// Take an element on the work shed
				robotArm.setPosition(100,70,100,500);
				robotArm.setPosition(60,70,100,1000);
				robotArm.setPosition(100,70,100,500);
				robotArm.setPosition(100,70,80,500);
			break;
			case GALLERY :
				// Arming the arm
				robotArm.grab();
				robotArm.setPosition(100,100,80,1000);
				// Take an element from the Gallery
			break;
			case PEDESTAL :
				// Arming the arm
				robotArm.grab();
				robotArm.setPosition(100,50,50,1000);
				// Take the statuette from the pedestal
				robotArm.setPosition(100,50,100,1000);
				robotArm.setPosition(80,70,100,1000);
				robotArm.setPosition(100,50,100,1000);
			break;
		}
		robotArm .setPosition(0,0,80,500);
	}

	void releaseElement(Bras robotArm,int location){
			
		switch(location){
			case FLOOR :
				// Release an element on the floor
				robotArm.setPosition(35,100,80,400);
				robotArm.ungrab(500);
				robotArm.setPosition(60,100,80,400);
			break;
			case DISPENSER :
				// Release an element on a 60° dispenser
				robotArm.ungrab();
			break;
			case WORK_SHED :
				// Release an element on the work shed
				robotArm.ungrab();
			break;
			case GALLERY :
				// Relase an element on the Gallery
				robotArm.ungrab();
			break;
			case PEDESTAL :
				// Release the statuette on the museum
				robotArm .setPosition(0,0,80,200);
				robotArm .setPosition(100,50,50,1000);
				robotArm .setPosition(80,70,50,1000);
				robotArm.ungrab(500);
			break;
		}
		robotArm .setPosition(0,0,50,500);
	}

	void flipElement(Bras robotArm){
		robotArm .setPosition(0,100,20,800);
		robotArm.ungrab(300);

		//IHM::waitCheck(); // ToDo reculer Relativement en fonction de la position de robotArm
		robotArm .setPosition(0,100,0,500);
		robotArm .setPosition(100,100,0,1000);
		//IHM::waitCheck();// ToDo avancer Relativement en fonction de la position de robotArm

		takeElement(robotArm,FLOOR);
	}

	void flipChallenge(Bras robotArm){
		// Arming the arm
		robotArm.grab();
		robotArm.setPosition(100,100,80,1000);
		// Take an element lay on the floor
		robotArm.setPosition(35,100,80,1000);
		// Let it flip !
		robotArm.setPosition(35,40,0,200);
		robotArm.ungrab(300);
		robotArm.setPosition(0,0,50);
	}

}