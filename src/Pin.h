#pragma once
#include <Arduino.h>

namespace Pin{
    // Declaration des pins E/S
    // IHM Pinout

    namespace Inputs{
        const int 
            init = 25,
            starter = 26,
            team = 27,
            strategy = 28; 
 
    }

    namespace Led{
        const int equipe = 57;
    }
    
    // TFT pinout
    namespace TFT{
        const int
            CS = 43,
            DC = 54,
            RST = 255,
            MOSI = 44,
            SCK = 46,
            MISO = 45;

        const int T_CS_PIN = 51, T_IRQ_PIN = 52;
    }
    
    // Stepper Pinout
    namespace Stepper{
        const int
        enable = 24,
        dirB  = 10, 
        dirC  = 11, 
        dirA  = 12,
        stepB = 5, 
        stepC = 6, 
        stepA = 9;
    }
    

    // Actuator Pinout
    namespace Servo{
        const int ServoA1 = 23; //P2
        const int ServoA2 = 21; //A4
        const int ServoA3 = 19; //D1

        const int ServoB1 = 17; //A2
        const int ServoB2 = 16; //A1
        const int ServoB3 = 18; //D2

        const int ServoC1 = 56; //D3
        const int ServoC2 = 20; //A3
        const int ServoC3 = 22; //P1
    }


}   

