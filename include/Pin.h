#pragma once

namespace Pin{
    // Declaration des pins E/S
    const int 
        //Balise
        Beacon               =  28, //OUTPUT

        //IHM       
        Tirette              = A22, //INPUT_PULLUP

        latchMux             =  37, //OUTPUT
        clockMux             =  38, //OUTPUT
        dataMux              =  39; //INPUT

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
    }

    namespace Stepper{
        const int 
            enable = 23,       //OUTPUT
            dirA = 16,        //OUTPUT
            dirB = 17,        //OUTPUT
            dirC = 15,        //OUTPUT
            stepA = 22,       //OUTPUT
            stepB = 21,       //OUTPUT
            stepC = 20;       //OUTPUT
    }
}   

