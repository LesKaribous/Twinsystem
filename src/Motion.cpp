#include "Motion.h"
#include "Controller.h"
#include "Match.h"

#include "Geometry.h"

namespace Motion
{

	Vec2 position = {0,0};

	//----------------ENVOI UNE COMMANDE TURN GO----------------
	void goTo(int X, int Y, int rot){
		Vec3 target = fk({X, Y, rot});
		Controller::move(target);
	}

	Vec3 fk(Vec3 pos){
		float θ = pos.c,
			  a = PI/3,
			  b = a - θ,
			  c = a + θ,
			  L = Settings::Robot::RADIUS;

		Matrix3x3 P = {
			-cosf(θ), -sinf(θ), L,
			 cosf(b) , sinf(b), L,
			 cosf(c) , sinf(c), L
		};

		return pos.mult(P);
	}
}
