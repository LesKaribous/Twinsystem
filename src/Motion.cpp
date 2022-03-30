#include "Motion.h"
#include "Controller.h"
#include "Match.h"

#include "Debugger.h"
#include "Geometry.h"

namespace Motion
{

	Vec3 position = {0,0,0};

	//----------------ENVOI UNE COMMANDE TURN GO----------------
	void goTo(int X, int Y){

		Debugger::log("Target:");
		Debugger::log(X);
		Debugger::log(Y);

		Vec3 target = ik({X, Y, position.c});

		Debugger::log("Result:");
		Debugger::log(target.a);
		Debugger::log(target.b);
		Debugger::log(target.c);

		Controller::move(target, true);
	}

	Vec3 ik(Vec3 target){
		float w = target.c,
			  a = PI/3.0f,
			  b = a - w,
			  c = a + w,
			  L = Settings::Robot::RADIUS;

		Debugger::log("Calculus:");
		Debugger::log(a);
		Debugger::log(b);
		Debugger::log(c);
		Debugger::log(L);

		Matrix3x3 P = {
			-cosf(w), -sinf(w), L,
			 cosf(b) , sinf(b), L,
			 cosf(c) , sinf(c), L
		};

		Debugger::log(P.trace());

		return target.mult(P);
	}
}
