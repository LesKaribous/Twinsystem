#pragma once

//TODO RESERVE
namespace Pin{
    // Declaration des pins E/S
    const int 
        PompeGauche          =  23, //OUTPUT
        EVGauche             =  17, //OUTPUT
        PompeDroit           =  27, //OUTPUT
        EVDroit              =  26, //OUTPUT

        //Bras ventouse
        ServoDroit           =   8, //OUTPUT
        ServoVentouseDroit   =   9, //OUTPUT
        ServoGauche          =  24, //OUTPUT
        ServoVentouseGauche  =  10, //OUTPUT

        //Drapeau
        ServoDrapeau         =  15, //OUTPUT

        //Bras manche à air
        ServoBrasDroit       =  16, //OUTPUT
        ServoBrasGauche      =  25, //OUTPUT

        //Balise
        Beacon               =  28, //OUTPUT

        //IHM       
        Tirette              = A22, //INPUT_PULLUP
        latchMux             =  37, //OUTPUT
        clockMux             =  38, //OUTPUT
        clockInhMux          = A21, //OUTPUT
        dataMux              =  39; //INPUT
}   