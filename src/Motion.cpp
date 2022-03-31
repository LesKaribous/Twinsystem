#include "Motion.h"
#include "Controller.h"
#include "Match.h"

#include "Debugger.h"
#include "Geometry.h"

namespace Motion
{

	Vec3 position = {0,0,0};

	void go(Vec3 target){

		Debugger::log("----------------");
		Debugger::log("Target :");
		Debugger::log(target.a);
		Debugger::log(target.b);
		Debugger::log(target.c);

		Vec3 calibration = Settings::Calibration::Primary.Cartesian;
		target.mult(calibration.toMatrix());

		Vec3 targetHolo = ik(target);

		Debugger::log("Holonomic :");
		Debugger::log(targetHolo.a);
		Debugger::log(targetHolo.b);
		Debugger::log(targetHolo.c);

		Controller::move(targetHolo, true);
	}

	Vec3 ik(Vec3 target){
		float w = target.c,
			  a = PI/3.0f,
			  b = a - w,
			  c = a + w,
			  L = Settings::Robot::RADIUS;

		Debugger::log("Calculus :");
		Debugger::log(a);
		Debugger::log(b);
		Debugger::log(c);

		Matrix3x3 P = {
			-cosf(w), -sinf(w), L,
			 cosf(b) , sinf(b), L,
			 cosf(c) , sinf(c), L
		};

		return target.mult(P);
	}
}
