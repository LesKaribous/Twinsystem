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

		//stepper->Sleep();
    }


	void Match(){
		motion->SetAbsolute();
		Vec2 cakeRose1(575,1775);
		Vec2 cakeJaune1(775,1775);
		Vec2 cakeMarron1(1125,1275);

		Vec2 b1(225,1775);
		
		actuators->ungrab(RobotCompass::AB);
		motion->Go(cakeRose1);
		actuators->grab(RobotCompass::AB);
		motion->Align(RobotCompass::BC, GetCompassOrientation(TableCompass::NORTH));
		delay(1000);
		actuators->ungrab(RobotCompass::BC);
		motion->Go(cakeJaune1);
		actuators->grab(RobotCompass::BC);
		motion->Align(RobotCompass::CA, 45);
		delay(1000);
		actuators->ungrab(RobotCompass::CA);
		motion->Go(cakeMarron1);
		actuators->grab(RobotCompass::CA);
		delay(1000);
		motion->Go(b1);


		actuators->unlock(RobotCompass::AB);
		actuators->unlock(RobotCompass::BC);
		actuators->unlock(RobotCompass::CA);
		delay(10000);


		//motion->Turn(-60);

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