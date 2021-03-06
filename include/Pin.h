#pragma once
#include <Arduino.h>

namespace Pin{
    // Declaration des pins E/S
     
        //IHM    
        namespace Lcd{
            const int
                Cs = 10,
                Dc = 9 ,
                Rs = 8 ;
        }   

    const int
        tirette              =  37, //INPUT_PULLUP
        latchMux             =  38,
        clockMux             =  39, //OUTPUT
        dataMux              =  24, //INPUT
        checkButton          =  36,
        neoPixel             =  35,
        robotSelect          =  14;
    
    const int probeResistor  = A26;
    

    namespace Servo{
        const int 
            pinServo01  = 0,
            pinServo02  = 1,
            pinServo03  = 2,
            pinServo04  = 3,

            pinServo05  = 4,
            pinServo06  = 5,
            pinServo07  = 6,
            pinServo08  = 7,

            pinServo09  = 25,
            pinServo10  = 28,
            pinServo11  = 29,
            pinServo12  = 30;
    }

    namespace Pump{
        const int 
            pinPump01   = 0,
            pinEv01     = 1,

            pinPump02   = 2,
            pinEv02     = 3,

            pinPump03   = 4,
            pinEv03     = 5;
    }
    

    namespace Stepper{
        const int 
            enable  = 23,      //OUTPUT
            dirA    = 16,        //OUTPUT
            dirB    = 17,        //OUTPUT
            dirC    = 15,        //OUTPUT
            stepA   = 22,       //OUTPUT
            stepB   = 21,       //OUTPUT
            stepC   = 20;       //OUTPUT
    }
}   

