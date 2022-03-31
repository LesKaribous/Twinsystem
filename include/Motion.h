#pragma once

#include "Geometry.h"

namespace Motion{
    extern Vec3 position;

    void init();
    void sequenceRecalage();

    //----- COMMANDES DE DEPLACEMENT -----
    void go(Vec3);

    //Inverse Kinematics
    Vec3 ik(Vec3);
}