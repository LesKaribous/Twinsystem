#include "Strategy.h"
#include "Twinsystem.h"

using namespace Motion;
using namespace Match;
using namespace Actuators;

//----------------STRATEGIES----------------
namespace Strategy{

	int nbrElBlue = 0, nbrElGreen = 0, nbrElRed = 0;

	void recalage(){
		if(Settings::ROBOT == Settings::PRIMARY) recalagePrimary();
		else if(Settings::ROBOT == Settings::SECONDARY) recalageSecondary();
	}

	void homologation(){
		if(Settings::ROBOT == Settings::PRIMARY) homologationPrimary();
		else if(Settings::ROBOT == Settings::SECONDARY) homologationSecondary();
	}

	void match(){
		if(Settings::ROBOT == Settings::PRIMARY) matchPrimary();
		else if(Settings::ROBOT == Settings::SECONDARY) matchSecondary();
	}

	void recalagePrimary(){
		// Laisser passer le robot secondaire
		/*
		SetRelative();
		go(500,0);
		wait(12000);
		go(-500,0);
		*/
		// ------
		SetPosition(Vec3(0,0,0));
		Vec2 borderXmin(-100, 0	 );
		Vec2 borderYmin(   0,-100);
		Vec2 borderXmax( 100, 0	 );
		Vec2 borderYmax(   0, 100);
		probeBorder(borderXmin);
		probeBorder(borderYmin);
		SetAbsolute();
		goTurn(250,550,0);
		probeBorder(borderXmin);
		goTurn(250,550,0);
		Controller::sleep();
	}

    void recalageSecondary(){
		SetPosition(Vec3(0,0,0));
		Vec2 borderXmin(-100, 0	 );
		Vec2 borderYmin(   0,-400);
		Vec2 borderXmax( 100, 0	 );
		Vec2 borderYmax(   0, 100);
		probeBorder(borderXmin);
		probeBorder(borderYmin);
		SetAbsolute();
		goTurn(250,850,0);
		probeBorder(borderXmin);
		goTurn(250,850,0);
		Controller::sleep();
	}

	void homologationPrimary(){

	}
    void homologationSecondary(){
		
	}

	void matchPrimary(){
		takeGroundTrio();
		
		flipElement(BrasAU);
		flipElement(BrasInit);
		flipElement(BrasTirette);

		layOnGallery(BrasTirette,RED_ELEMENT);
		layOnGallery(BrasInit,GREEN_ELEMENT);
		layOnGallery(BrasAU,BLUE_ELEMENT);

		takeHorizontalDispenser(BrasAU);
		layOnGallery(BrasAU,GREEN_ELEMENT);

		goHome();
	}
	void matchSecondary(){
		takeStatuette(BrasAU);

		goHome();
	}

	void waitLaunch()
	{
    	while(!IHM::getTirette()){
			Controller::update();
			IHM::menu();
			Debugger::checkSerial();
			if(Debugger::lastCommand() == "start") break;
		}
    	while(IHM::getTirette()){
			Controller::update();
			Debugger::checkSerial();
			IHM::menu();
		}

		// Lancement du Match
		Match::start();
    	IHM::LCD::goScreen();
		// Enregistrement des paramètres de match
		IHM::freezeSettings();
		// Delais d'affichage
		delay(100);
		//Match::update();

	}

//-------- SOUS-STRATEGIES --------

	void takeGroundTrio(){
		SetAbsolute();
		go(673,675);
		takeElement(BrasInit,GROUND);
		BrasInit.updateElement(GREEN_ELEMENT);
		go(650,550);
		turn(120);
		go(743,550);
		takeElement(BrasAU,GROUND);
		BrasInit.updateElement(BLUE_ELEMENT);
		turn(180);
		go(820,668);
		takeElement(BrasTirette,GROUND);
		BrasInit.updateElement(RED_ELEMENT);
	}

	void takeHorizontalDispenser(Bras &robotArm){
		SetAbsolute();
		go(200,300);
		turn(0);
		// -----Probing-----
		SetPosition(Vec3(0,0,0));
		Vec2 borderXmin(-20, 0	 );
		Vec2 borderYmin(   0,-100);
		probeBorder(borderXmin);
		probeBorder(borderYmin);
		//-------
		go(200,300);
		turn(robotArm.GetAngle()+60);
		go(110,300);
		takeElement(robotArm,WORK_SHED);
		robotArm.updateElement(GREEN_ELEMENT);
	}

	void layOnGallery(Bras &robotArm, int colorElement){
		float xPos = 0 ;
		bool layOnTop = false;
		Debugger::log(colorElement);
		if(colorElement == BLUE_ELEMENT) {
			xPos = 570 ; 
			if (nbrElBlue>0) layOnTop = true ;
			nbrElBlue++;
		}
		else if(colorElement == GREEN_ELEMENT) {
			xPos = 810 ; 
			if (nbrElGreen>0) layOnTop = true ;
			nbrElGreen++;
		}
		else if(colorElement == RED_ELEMENT) {
			xPos = 1050 ; 
			if (nbrElRed>0) layOnTop = true ;
			nbrElRed++;
		}
		SetAbsolute();
		go(xPos,240);
		turn(360-robotArm.GetAngle()+90); // +90 car Gallerie positionnée à 90° de l'origine
		go(xPos,160);
		// ToDo Recalage Bordure
		if(!layOnTop) releaseElement(robotArm,GALLERY);
		else releaseElement(robotArm,GALLERY_TOP);
		go(xPos,240);
		robotArm.updateElement(NO_ELEMENT);
	}

	void takeStatuette(Bras &robotArm){
		SetAbsolute();
		go(390,1580);
		turn(-15); //TODO: A modifier avec l'angle de robotArm
		takeElement(robotArm,PEDESTAL);
		turn(0);
		go(200,200);
		turn(360-robotArm.GetAngle()+90);
		go(200,200);
		releaseElement(robotArm,MUSEUM);
	}

	void goHome(){
		SetAbsolute();
		go(200,400);
	}

//----- ACTUATORS STRATEGIES -----

	void takeElement(Bras robotArm,int location)
	{
		boolean tAbsolute = isAbsolute(); // Stock le type de positionnement
		switch(location){
			case GROUND :
				// Arming the arm
				robotArm.grab();
				robotArm.setPosition(100,100,80,400);
				// Go To the element
				SetRelative();
				goPolar(robotArm.GetAngle(),20);
				// Take an element lay on the floor
				robotArm.setPosition(40,100,80,800);
				robotArm.setPosition(80,100,80,400);
				goPolar(robotArm.GetAngle(),-30);
				robotArm.setPosition(80,50,80);
			break;
			case DISPENSER :
				// Arming the arm
				robotArm.grab();
				robotArm.setPosition(0,60,70,1000);
				// Take an element lay on a 60° dispenser
				robotArm.setPosition(0,20,80,1000);
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
				// Go To the element
				SetRelative();
				goPolar(robotArm.GetAngle(),80);
				robotArm.setPosition(100,70,100,1000);
				robotArm.setPosition(100,50,100,1000);
				goPolar(robotArm.GetAngle(),-100);
			break;
		}
		robotArm .setPosition(0,0,80,500);

		SetAbsolute(tAbsolute); // Restaure le type de positonnement 
	}

	void releaseElement(Bras robotArm,int location){

		boolean tAbsolute = isAbsolute(); // Stock le type de positionnement

		switch(location){
			case GROUND :
				// Release an element on the floor
				robotArm.setPosition(40,100,80,400);
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
				robotArm.setPosition(0,30,100,200);
				robotArm.ungrab();

			break;
			case GALLERY_TOP :
				// Relase an element on the Gallery
				robotArm.setPosition(0,30,100,400);
				robotArm.setPosition(100,30,100,400);
				robotArm.setPosition(100,40,80,200);
				robotArm.ungrab();

			break;
			case MUSEUM :
				// Release the statuette on the museum
				robotArm .setPosition(0,0,80,200);
				robotArm .setPosition(100,50,50,1000);
				SetRelative();
				goPolar(robotArm.GetAngle(),60);
				robotArm .setPosition(80,70,50,1000);
				robotArm.ungrab(500);
				IHM::Sound::playSound(MUSEUM_SOUND);
				goPolar(robotArm.GetAngle(),-80);
			break;
		}
		robotArm .setPosition(0,0,50,0);

		SetAbsolute(tAbsolute); // Restaure le type de positonnement 
	}

	void flipElement(Bras robotArm){

		boolean tAbsolute = isAbsolute(); // Stock le type de positionnement

		robotArm .setPosition(0,100,0,800);
		robotArm.ungrab();
		SetRelative();
		goPolar(robotArm.GetAngle(),-70); // reculer relativement en fonction de robotArm
		//robotArm .setPosition(0,100,0,500);
		robotArm .setPosition(100,100,0,200);
		goPolar(robotArm.GetAngle(),100); //avancer relativement en fonction de robotArm
		takeElement(robotArm,GROUND);

		SetAbsolute(tAbsolute); // Restaure le type de positonnement 
	}

	void flipChallenge(Bras robotArm){
		// Arming the arm
		robotArm.grab();
		robotArm.setPosition(100,100,80,1000);
		// Take an element lay on the floor
		robotArm.setPosition(35,100,80,500);
		// Let it flip !
		robotArm.setPosition(35,40,0,200);
		robotArm.ungrab(300);
		robotArm.setPosition(0,0,50);
	}

	void probeElement(){
		BrasTirette.setTool(100);
		BrasTirette.setTool2(100,500);
		int square = Actuators::getProbingValue();
		Debugger::log(square);
		BrasTirette.setTool(0);
		BrasTirette.setTool2(0,500);
		if(square == IHM::getEquipe()) {
			BrasTirette.setElevator(100,500);
			updateScore(5);
			BrasTirette.setElevator(0,500);
		}
	}
}