#include "Strategy.h"
#include "Twinsystem.h"

using namespace Motion;
using namespace Match;
using namespace Actuators;
using namespace System;

//----------------STRATEGIES----------------
namespace Strategy{

	int nbrElBlue = 0, nbrElGreen = 0, nbrElRed = 0;

	void recalage(){
		if(Settings::primary()) recalagePrimary();
		else if(Settings::secondary()) recalageSecondary();
	}

	void homologation(){
		if(Settings::primary()) homologationPrimary();
		else if(Settings::secondary()) homologationSecondary();
	}

	void match(){
		if(Settings::primary()) matchPrimary();
		else if(Settings::secondary()) matchSecondary();
	}

	void recalagePrimary(){
		// Laisser passer le robot secondaire
		SetAvoidance(false);
		SetRelative();
		go(500,0);
		wait(12000);
		go(-500,0);
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
		SetAvoidance(false);
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
		goTurn(260,850,0);
		Controller::sleep();
	}

	void homologationPrimary(){
		matchPrimary();
	}
    
	void homologationSecondary(){
		matchSecondary();
	}

	void matchPrimary(){

		SetAvoidance(false);
		updateScore(Score::STATUETTE_DEPOSEE);
		updateScore(Score::VITRINE_DEPOSEE);

		takeGroundTrio();

		flipElement(BrasAU);
		flipElement(BrasInit);
		flipElement(BrasTirette);

		layOnGallery(BrasTirette,RED_ELEMENT);
		layOnGallery(BrasInit,GREEN_ELEMENT);
		layOnGallery(BrasAU,BLUE_ELEMENT);

		SetAvoidance(true);
		go(1350,290);
		takeDispenser(BrasAU,SECOND_DISPENSER);
		go(1350,275);
		takeDispenser(BrasInit,SECOND_DISPENSER);
		go(1350,260);
		takeDispenser(BrasTirette,SECOND_DISPENSER);

		//takeHorizontalDispenser(BrasAU);
		//go(800,300);
		//flipElement(BrasAU);
		//layOnGallery(BrasAU,GREEN_ELEMENT);

		goHome();
	}
	
	void matchSecondary(){
		SetAvoidance(false);
		takeAndPushUnder(BrasAU);
		takeStatuette(BrasAU);
		releaseCube(BrasTirette);
		layStatuette(BrasAU);
		go(400,1000);
		SetAvoidance(true);

		for(int square = 0; square <=2; square++) flipSquares(square) ;
		updateScore(Score::CARRE_NON_BASCULE);

		goHome();
	}

	void waitLaunch()
	{
    	while(!IHM::getTirette()){
			System::update();
			if(Debugger::lastCommand() == "start") break;
		}
    	while(IHM::getTirette()){
			System::update();
		}
		
		// Enregistrement des paramètres de match
		IHM::freezeSettings();
		Settings::setTeam(IHM::getEquipe());
	}

//-------- SOUS-STRATEGIES --------

	void takeGroundTrio(){
		SetAbsolute();
		go(673,675);
		takeElement(BrasInit,GROUND);
		BrasInit.updateElement(GREEN_ELEMENT);
		go(680,550);
		if(Settings::yellow()) align(0, BrasAU.GetAngle());
		if(Settings::purple()) align(0,-BrasAU.GetAngle());
		go(743,550);
		takeElement(BrasAU,GROUND);
		BrasAU.updateElement(BLUE_ELEMENT);
		if(Settings::yellow()) align(-60, BrasTirette.GetAngle());
		if(Settings::purple()) align(-60,-BrasTirette.GetAngle());
		go(840,668);
		takeElement(BrasTirette,GROUND);
		BrasTirette.updateElement(RED_ELEMENT);
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
		Debugger::println(colorElement);
		if(colorElement == BLUE_ELEMENT) {
			xPos = 570 ; 
			if (nbrElBlue>0) layOnTop = true ;
			nbrElBlue++;
		}
		else if(colorElement == GREEN_ELEMENT) {
			xPos = 725 ; 
			if (nbrElGreen==1) xPos = 875 ;
			else if (nbrElGreen>=2){ xPos = 810 ; layOnTop = true ;}
			nbrElGreen++;
		}
		else if(colorElement == RED_ELEMENT) {
			xPos = 1050 ; 
			if (nbrElRed>0) layOnTop = true ;
			nbrElRed++;
		}
		SetAbsolute();
		go(xPos,230);
		if(Settings::yellow()) align(90, robotArm.GetAngle());// +90 car Gallerie positionnée à 90° de l'origine
		if(Settings::purple()) align(90,-robotArm.GetAngle());
		go(xPos,160);

		SetPosition(Vec3(GetPosition().a,112.61+87,GetPosition().c));

		if(!layOnTop) releaseElement(robotArm,GALLERY);
		else releaseElement(robotArm,GALLERY_TOP);
		go(xPos,210);
		updateScore(Score::ECHANTILLON_GALERIE);
		updateScore(Score::ECHANTILLON_GALERIE_TRIE);
		robotArm.updateElement(NO_ELEMENT);
	}

	void takeStatuette(Bras &robotArm){
		SetAbsolute();
		go(400,1580);
		if(Settings::yellow()) align(-135, robotArm.GetAngle());
		if(Settings::purple()) align(-135,-robotArm.GetAngle());
		takeElement(robotArm,PEDESTAL);
		updateScore(Score::STATUETTE_ENLEVEE);
	}

	void layStatuette(Bras &robotArm){
		SetAbsolute();
		turn(0);
		go(200,200);
		//Home 
		SetPosition(Vec3(0,0,0));
		Vec2 borderXmin(-100, 0	 );
		Vec2 borderYmin(   0,-100);
		Vec2 borderXmax( 100, 0	 );
		Vec2 borderYmax(   0, 100);
		probeBorder(borderXmin);
		probeBorder(borderYmin);
		//
		if(Settings::yellow()) align(90, robotArm.GetAngle());
		if(Settings::purple()) align(90,-robotArm.GetAngle());
		//turn(360-robotArm.GetAngle()+90);
		go(230,200);
		releaseElement(robotArm,MUSEUM);
		updateScore(Score::STATUETTE_VITRINE);
		updateScore(Score::VITRINE_ACTIVEE);
	}

	void takeAndPushUnder(Bras &robotArm){
		int statePush = 0;
		while(statePush<2)
		{
			// Take an element on the Workshed and push it under
			SetAbsolute();
			if(statePush == 0){
				if(Settings::yellow()) go(230,1545);
				if(Settings::purple()) go(280,1545);
			}
			else if(statePush == 1){
				go(500,1700); //Decalage
				if(Settings::yellow()) go(400,1730);
				if(Settings::purple()) go(430,1750);
			}

			if(Settings::yellow()) align(-135, robotArm.GetAngle());
			if(Settings::purple()) align(-135,-robotArm.GetAngle());
			
			//turn(360-robotArm.GetAngle()-135);
			takeElement(robotArm,WORK_SHED);
			updateScore(Score::ECHANTILLON_ENLEVE);
			// Go To center
			SetAbsolute();
			//go(390,1580);
			if(Settings::yellow()) go(500,1500);
			if(Settings::purple()) go(510,1510);
			// Go Back to free the space before releasing
			SetRelative();
			//goPolar(robotArm.GetAngle(),-100);
			//	Release
			releaseElement(robotArm,GROUND);
			//	Push under
			goPolar(robotArm.GetAngle(),220);
			// Go Back
			goPolar(robotArm.GetAngle(),-100);
			//	Incremente l'état pour passer au suivant
			statePush ++;
		}
		updateScore(Score::ECHANTILLON_ABRI);
		SetAbsolute();
	}

	void takeDispenser(Bras &robotArm, int dispenser){
		boolean tAbsolute = isAbsolute(); // Stock le type de positionnement

		SetAbsolute();
		if(dispenser == SECOND_DISPENSER){
			if(Settings::yellow())align(90, robotArm.GetAngle());
			if(Settings::purple())align(90,-robotArm.GetAngle());
		}
		else if(dispenser == FIRST_DISPENSER){
			if(Settings::yellow())align(60, robotArm.GetAngle());
			if(Settings::purple())align(60,-robotArm.GetAngle());
		}

		takeElement(robotArm,DISPENSER);
		updateScore(Score::ECHANTILLON_ENLEVE);

		SetAbsolute(tAbsolute); // Restaure le type de positonnement 
	}

	void takeFirstDispenser(){
		SetAbsolute();
		go(400,1225);
		go(280,1225);
		// Take First Element
		turn(BrasAU.GetAngle()+60);
		takeElement(BrasAU,DISPENSER);
		// Take Second Element
		turn(BrasTirette.GetAngle()-60);
		takeElement(BrasTirette,DISPENSER);
		go(400,1000);
		flipElement(BrasAU);
		flipElement(BrasTirette);
	}

	void releaseCube(Bras &robotArm){
		SetAbsolute();
		go(390,1580);
		if(Settings::yellow()) align(-135, robotArm.GetAngle());
		if(Settings::purple()) align(-135,-robotArm.GetAngle());
		//turn(360-robotArm.GetAngle()-135);
		SetRelative();
		goPolar(robotArm.GetAngle(),100);
		robotArm.setElevator(100,500);
		robotArm.setElevator(0,500);
		goPolar(robotArm.GetAngle(),-100);
		updateScore(Score::REPLIQUE_DEPOSEE);
		SetAbsolute();
	}

	void flipSquares(int squareNumber){
		SetAbsolute();
		
		if(Settings::yellow()) align(-90, BrasTirette.GetAngle());
		if(Settings::purple()) align(-90,-BrasTirette.GetAngle());

		go(667+(squareNumber*185) , 1800);
		if (squareNumber != 1) probeElement();
		else {
			SetRelative();
			goPolar(BrasTirette.GetAngle(),150);

			SetAbsolute();
			SetPosition(Vec3(GetPosition().a,1887,GetPosition().c)); //Recalage
			SetRelative();

			BrasTirette.setElevator(100,500);
			BrasTirette.setElevator(0);
			goPolar(BrasTirette.GetAngle(),-100);
			SetAbsolute();
		}
		updateScore(Score::CARRE_BASCULE);
		SetAbsolute();
	}

	void goHome(){
		SetAbsolute();
		if(Settings::primary()) go(200,400);
		else if(Settings::secondary()) go(200,950);
		
		updateScore(Score::ROBOTS_CAMPEMENT/2); // Approximation : les deux robots doivent être au campement à la fin
	}

//----- ACTUATORS STRATEGIES -----

	void takeElement(Bras robotArm,int location)
	{
		boolean tAbsolute = isAbsolute(); // Stock le type de positionnement
		switch(location){
			case GROUND :
				// Arming the arm
				robotArm.grab();
				robotArm.setPosition(80,100,80);
				// Go To the element
				SetRelative();
				goPolar(robotArm.GetAngle(),20);
				// Take an element lay on the floor
				robotArm.setPosition(40,100,80,800);
				// Go Up
				robotArm.setPosition(80,100,80,200);
				goPolar(robotArm.GetAngle(),-30);
				robotArm.setPosition(80,50,80);
			break;
			case DISPENSER :
				// Arming the arm
				robotArm.grab();
				robotArm.setPosition(0,65,70,1000);
				// Take an element lay on a 60° dispenser
				SetRelative();
				goPolar(robotArm.GetAngle(),30);
				robotArm.setPosition(0,20,80,1000);
				goPolar(robotArm.GetAngle(),-30);
			break;
			case WORK_SHED :
				// Arming the arm
				robotArm.grab();
				robotArm.setPosition(100,70,100,1000);
				// Go To the element
				SetRelative();
				goPolar(robotArm.GetAngle(),20);
				// Take an element on the work shed
				robotArm.setPosition(60,70,100,1000);
				robotArm.setPosition(100,70,100,500);
				goPolar(robotArm.GetAngle(),-80);
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
				robotArm.setPosition(100,50,100,1000);
				// Go To the element
				SetRelative();
				goPolar(robotArm.GetAngle(),100);
				// Take the statuette from the pedestal
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
			case PEDESTAL :
				// Relase the CUBE on the pedestal
				// Warning only use it on the secondary
				robotArm.setElevator(100,500);
				robotArm.setElevator(0,500);

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
				robotArm .setPosition(90,60,50);
				SetRelative();
				goPolar(robotArm.GetAngle(),20);
				robotArm.ungrab(500);
				IHM::Sound::playSound(MUSEUM_SOUND);
				goPolar(robotArm.GetAngle(),-80);
			break;
		}
		robotArm .setPosition(0,0,50);

		SetAbsolute(tAbsolute); // Restaure le type de positonnement 
	}

	void flipElement(Bras robotArm){

		boolean tAbsolute = isAbsolute(); // Stock le type de positionnement
		robotArm.setPosition(0,100,20,600);
		robotArm.ungrab();
		SetRelative();
		robotArm .setPosition(80,100,0);
		goPolar(robotArm.GetAngle(),-70); // reculer relativement en fonction de robotArm
		goPolar(robotArm.GetAngle(),120); //avancer relativement en fonction de robotArm
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
		boolean tAbsolute = isAbsolute(); // Stock le type de positionnement
		// Avancer / recalage
		SetRelative();
		goPolar(BrasTirette.GetAngle(),150);

		SetAbsolute();
		SetPosition(Vec3(GetPosition().a,1887,GetPosition().c)); //Recalage
		SetRelative();

		BrasTirette.setTool(100);
		BrasTirette.setTool2(100,500);
		int square = Actuators::getProbingValue();
		Debugger::println(square);
		BrasTirette.setTool(0);
		BrasTirette.setTool2(0,500);
		if(square == IHM::getEquipe()) {
			BrasTirette.setElevator(100,500);
			updateScore(5);
			BrasTirette.setElevator(0,500);
		}
		goPolar(BrasTirette.GetAngle(),-100);

		SetAbsolute(tAbsolute); // Restaure le type de positonnement 
	}

//----- ALIGNMENT STRATEGIES -----
// TODO: add and modify to motion ? @Jules ?

	void alignTurn(float angleFrom, float angleTo){
		boolean tAbsolute = isAbsolute(); // Stock le type de positionnement

			SetAbsolute();
			
			if(Settings::yellow()) turn(-angleFrom+angleTo);
			else if(Settings::purple()) turn(+angleFrom-angleTo);

		SetAbsolute(tAbsolute); // Restaure le type de positonnement */
	}

}