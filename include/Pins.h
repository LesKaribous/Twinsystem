#pragma once

namespace Pin{
    // Declaration des pins E/S
    const int 
    PompeGauche          = 23,
    EVGauche             = 17,
    PompeDroit           = 27,
    EVDroit              = 26,

    //Bras ventouse
    ServoDroit           =  8,
    ServoVentouseDroit   =  9,
    ServoGauche          = 24,
    pServoVentouseGauche  = 10,

    //Drapeau
    ServoDrapeau         = 15 ,

    //Bras manche à air
    ServoBrasDroit       = 16 ,      // Pin Servo bras droit
    ServoBrasGauche      = 25 ,      // Pin Servo bras gauche

    //Balise
    Beacon               = 28 ;
}