#include "Twinsystem.h"
#include "Test.h"

using namespace Motion;
using namespace Strategy;
using namespace Actuators;

namespace Test{

    void calibrationRotation(){
		SetRelative();
        turn(360*2);
		IHM::waitCheck();
		turn(-360*2);
    }

    void calibrationX(){
		SetRelative();
		go(200,0);
		IHM::waitCheck();
		go(-200,0);
    }

    void calibrationY(){
		SetRelative();
		go(0,200);
		IHM::waitCheck();
		go(0,-200);
    }


	void testMotion(){
		//calibrationRotation();
		//IHM::waitCheck();
		calibrationX();
		IHM::waitCheck();
		calibrationY();
	}


    void testingFlip(){
		takeElement(BrasAU,GROUND);
		delay(1000);
		flipElement(BrasAU);
		delay(1000);
		releaseElement(BrasAU,GROUND);
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



} // namespace Test
