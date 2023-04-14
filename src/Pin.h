#pragma once
#include <Arduino.h>

namespace Pin{
    // Declaration des pins E/S
    // IHM Pinout

    namespace Inputs{
        const int 
            resetButton = 25,
            starter = 26,
            teamSwitch = 27,
            strategySwitch = 28,
            twinSwitch = 2;
 
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
        const int ServoC1 = 23; //P2
        const int ServoC2 = 21; //A4
        const int ServoC3 = 19; //D1

        const int ServoA1 = 17; //A2
        const int ServoA2 = 16; //A1
        const int ServoA3 = 18; //D2

        const int ServoB1 = 56; //D3
        const int ServoB2 = 20; //A3
        const int ServoB3 = 22; //P1
    }


}   

