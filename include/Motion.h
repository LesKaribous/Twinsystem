#pragma once

#include "Geometry.h"

namespace Motion{
    extern Vec3 position;

    void init();
    void sequenceRecalage();

    //----- COMMANDES DE DEPLACEMENT -----
    void go(Vec3);
    void goTo(Vec3);


    //Getters
    Vec3 GetPosition();
	Vec3 GetTarget();

    //Inverse Kinematics
    Vec3 ik(Vec3);
}