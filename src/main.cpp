#include "TwinSystem.h"

using namespace TwinSystem;

class Robot : public System{
public :
    Robot() : System(){}

    void Recalage(){
		// Laisser passer le robot secondaire
		//SetAvoidance(false);
		//SetRelative();
		stepper->Engage();
		motion->Go(500,0);
		wait(12000);
		motion->Go(-500,0);
		// ------
		motion->SetPosition(Vec3(0,0,0));
		Vec2 borderXmin(-100, 0	 );
		Vec2 borderYmin(   0,-100);
		Vec2 borderXmax( 100, 0	 );
		Vec2 borderYmax(   0, 100);
		motion->ProbeBorder(borderXmin);
		motion->ProbeBorder(borderYmin);
		motion->SetAbsolute();
		motion->GoTurn(250,550,0);
		motion->ProbeBorder(borderXmin);
		motion->GoTurn(250,550,0);
		stepper->Sleep();
    }
};

void setup(){}

void loop(){
    Shared<Robot> robot = CreateShared<Robot>(); //Start the robot
    robot->Recalage(); //Execute instructions

    //Console::println("COUCOU");

    while(true);//End of the program
}