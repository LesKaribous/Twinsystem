#pragma once

#include "Bras.h"

namespace Actuators{

    enum class BrasVentouse {
        PRISE_DROITE,
        PRISE_GAUCHE,
        DEPOSE_DROITE,
        DEPOSE_GAUCHE,
        PRISE_ENSEMBLE,
        DEPOSE_ENSEMBLE,
        REPOS
    };

    void prepare();
    void init();

    void initBrasMancheAir();
    void brasMancheAir(bool state);
    void brasVentouse(BrasVentouse stateBras);


    Bras brasDroit ;
    Bras brasGauche ;

    Servo servoDrapeau ;
    Servo servoBrasDroit ;
    Servo servoBrasGauche ;

}







