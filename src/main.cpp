#include "TwinSystem.h"
#include "math/Geometry.h"

using namespace TwinSystem;

class Robot : public System{
public :
    Robot() : System(){}

	bool amorce = false;
	bool waitForLaunch(){
		
		Run();
		if (!amorce && !ui.inputs.starter.GetState())
			amorce = true;

		if(!ui.inputs.init.GetState()) //Init pressed
			Recalage(); //Execute instructions

		delay(10);
		return !(amorce && ui.inputs.starter.GetState());
	}


	void identifySteppers(){
		//motion->Go(d, 0);

		stepper->Engage();

		stepper->Move(Vec3(1000,0,0));
		delay(1000);	
		stepper->Move(Vec3(0,1000,0));
		delay(1000);	
		stepper->Move(Vec3(0,0,1000));
		delay(1000);	

		stepper->Disengage();
	}

	void X(int d){
		stepper->Engage();
		motion->Go(d, 0);
		stepper->Disengage();
	}

	void Y(int d){
		stepper->Engage();
		motion->Go(0, d);
		stepper->Disengage();
	}

	void R(int d){
		stepper->Engage();
		motion->Turn(d);
		stepper->Disengage();
	}

	void test(){
		motion->GoPolar(GetCompassOrientation(RobotCompass::A), 100);

		motion->GoPolar(GetCompassOrientation(RobotCompass::C), 100);

	}

    void Recalage(){
		// Laisser passer le robot secondaire
		//SetAvoidance(false);
		//SetRelative();
		actuators->close(RobotCompass::AB);
		actuators->close(RobotCompass::BC);
		actuators->close(RobotCompass::CA);
		// ------
		motion->SetPosition(Vec3(-1,-1,0));
		motion->ProbeBorder(TableCompass::SOUTH, RobotCompass::BC);
		motion->ProbeBorder(TableCompass::EAST, RobotCompass::CA);
		motion->SetAbsolute();
		motion->Turn(-60);
		actuators->ungrab(RobotCompass::AB);
		actuators->ungrab(RobotCompass::BC);
		actuators->ungrab(RobotCompass::CA);

		stepper->Disengage();
    }


	void Match(){
		stepper->Engage();
		motion->SetAbsolute();
		Vec2 cakeRose1(575,1775);
		Vec2 cakeJaune1(775,1775);
		Vec2 cakeMarron1(1125,1275);

		Vec2 b1(225,1775);

		Vec2 ballBlueBegin(300,1125);
		Vec2 ballBlueEnd(150,1125);
		Vec2 ballBlueBasket(200,1675);

		Vec2 b1safe(450,1550);
		Vec2 b1safe2(300,1300);
		Vec2 b5safe(900,400);

		Vec2 depose1(200,1800);
		Vec2 depose2(350,1800);
		Vec2 depose3(450,1800);

		Vec2 retrait1(200,1650);
		Vec2 retrait2(350,1650);
		Vec2 retrait3(450,1650);
		
		
		actuators->ungrab(RobotCompass::AB);
		motion->Go(cakeRose1);
		actuators->grab(RobotCompass::AB);
		motion->Align(RobotCompass::BC, GetCompassOrientation(TableCompass::NORTH));

		actuators->ungrab(RobotCompass::BC);
		motion->Go(cakeJaune1);
		actuators->grab(RobotCompass::BC);
		motion->Align(RobotCompass::CA, 45);

		actuators->ungrab(RobotCompass::CA);
		motion->Go(cakeMarron1);
		actuators->grab(RobotCompass::CA);

		motion->Go(b1safe);
		//Dépose du premier Gateau
		motion->Align(RobotCompass::BC, -120);
		motion->Go(depose1);
		actuators->ungrab(RobotCompass::BC);
		actuators->unlock(RobotCompass::BC);
		delay(500);
		motion->Go(retrait1);
		motion->Go(retrait2);
		actuators->close(RobotCompass::BC);
		//Dépose du second Gateau
		motion->Align(RobotCompass::AB, -120);
		motion->Go(depose2);
		actuators->ungrab(RobotCompass::AB);
		actuators->unlock(RobotCompass::AB);
		delay(500);
		motion->Go(retrait2);
		motion->Go(retrait3);
		actuators->close(RobotCompass::AB);
		//Dépose du troisieme Gateau
		motion->Align(RobotCompass::CA, -120);
		motion->Go(depose3);
		actuators->ungrab(RobotCompass::CA);
		actuators->unlock(RobotCompass::CA);
		delay(500);
		motion->Go(retrait3);
		actuators->close(RobotCompass::CA);
		
		
		//recalage
		motion->ProbeBorder(TableCompass::SOUTH, RobotCompass::C);
		motion->SetAbsolute();
		//Va chercher le balles
		motion->Align(RobotCompass::A, GetCompassOrientation(TableCompass::WEST));
		motion->Go(ballBlueBegin);
		motion->Go(ballBlueEnd);
		// Va en dépose des balles
		motion->Go(ballBlueBasket);
		motion->Align(RobotCompass::A, GetCompassOrientation(TableCompass::SOUTH));
		//recalage
		motion->ProbeBorder(TableCompass::SOUTH, RobotCompass::A);
		motion->SetAbsolute();
		// Dépose

		// Fin de match
		motion->Go(b5safe);
		stepper->Disengage();

    }
};

void setup(){}

void loop(){
    Robot robot; //Start the robot
	robot.motion->SetAbsolute();
	while(robot.waitForLaunch()){}

	//robot.actuators->grab(RobotCompass::AB);	
	//robot.actuators->applause(RobotCompass::AB);
	//robot.actuators->applause(RobotCompass::BC);	
	//robot.actuators->applause(RobotCompass::CA);	
	robot.Match();



    Console::println("Fin du programme");
    while(true);//End of the program
}