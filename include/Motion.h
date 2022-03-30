#pragma once

#include "Geometry.h"

namespace Motion{
    extern Vec3 position;

    void init();
    void sequenceRecalage();

    //----- COMMANDES DE DEPLACEMENT -----
    void goTo(int X, int Y);

    //Inverse Kinematics
    Vec3 ik(Vec3);
}