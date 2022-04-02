#include "Motion.h"
#include "Controller.h"
#include "Match.h"

#include "Debugger.h"
#include "Geometry.h"

namespace Motion
{

	Vec3 position = {0,0,0};
	

	void go(Vec3 target){

		Vec3 calibration = Settings::ROBOT == Settings::PRIMARY ?  Settings::Calibration::Primary.Cartesian : Settings::Calibration::Secondary.Cartesian;
		target.mult(calibration.toMatrix()); //Apply calibration (X,Y,ROT)

		target = ik(target); //Apply inverse kinematics (X,Y,ROT) -> (Va, Vb, Vc)
		target.mult(Settings::Stepper::STEP_MODE*RAD_TO_DEG);

		Controller::move(target, false);
	}

	Vec3 ik(Vec3 target){
		float c60 = cosf(PI/3.0f),
			  s60 = sinf(PI/3.0f),
			  L = Settings::RADIUS,
			  R = Settings::WHEEL_RADIUS;

		target.c *= DEG_TO_RAD;

		Matrix3x3 P = {
			   0,   1 , L,
			-s60, -c60, L,
			 s60, -c60, L
		};
				
		return target.mult(P).mult(1/R);
	}
}
