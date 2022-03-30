#pragma once

namespace Pin{
    // Declaration des pins E/S
    const int 
        //IHM       
        Tirette              =  37, //INPUT_PULLUP
        clockMux             =  38, //OUTPUT
        dataMux              =  38, //INPUT
        initButton           =  36,
        neoPixel             =  35,
        robotSelect          =  14;
/*
    namespace Servo{
        const int 
            pinPump01   = 0,
            pinServo02  = 1,
            pinServo03  = 2,
            pinServo04  = 3,

            pinPump02   = 4,
            pinServo06  = 5,
            pinServo07  = 6,
            pinServo08  = 7,

            pinPump03   = 25,
            pinServo10  = 28,
            pinServo11  = 29,
            pinServo12  = 30;
    }*/

    namespace Stepper{
        const int 
            enable = 23,      //OUTPUT
            dirA = 17,        //OUTPUT
            dirB = 16,        //OUTPUT
            dirC = 15,        //OUTPUT
            stepA = 22,       //OUTPUT
            stepB = 21,       //OUTPUT
            stepC = 20;       //OUTPUT
    }
}   

