#pragma once

#include "Geometry.h"

namespace Motion{
    extern Vec2 position;

    void init();
    void sequenceRecalage();

    //----- COMMANDES DE DEPLACEMENT -----
    void turnGo(int turn, int go);
    void goTo(int X, int Y, int rot);
}