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

    namespace Outputs{
        const int enTraco = 13; // Enable Traco regulator - Enable on true
    }

    namespace Led{
        const int neopixel = 57;
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
        dirB  = 11, 
        dirC  = 10, 
        dirA  = 12,
        stepB = 6, 
        stepC = 5, 
        stepA = 9;
    }
    
    namespace LocalisationSensors{
        const int SensorC = 40;
        const int SensorA = 41;
        const int SensorB = 42;
    }

    // Actuator Pinout
    namespace Servo{
        // Connector Act 1
        // Bloç Servo 1 - BC
        const int ServoA1 = 23; //P2
        const int ServoA2 = 21; //A4
        const int ServoA3 = 19; //D1
        // Bloç Servo 2 - AB
        const int ServoB1 = 17; //A2
        const int ServoB2 = 16; //A1
        const int ServoB3 = 18; //D2
        // Bloç Servo 3 - CA
        const int ServoC1 = 56; //D3
        const int ServoC2 = 20; //A3
        const int ServoC3 = 22; //P1
    }

    // Sensor Pinout
    namespace Sensor{
        // Connector Act 3
        // ---
        // Bloc Sensor 1 - BC
        const int SensorRight_BC = 36; //P2
        const int SensorLeft_BC  = 49; //A4
        //const int SensorA3 = 48; //D1
        // ---
        // Bloc Sensor 2 - AB
        const int SensorRight_AB = 31; //A2
        const int SensorLeft_AB  = 32; //A1
        //const int SensorB3 = 47; //D2
        // ---
        // Bloc Sensor 3 - CA
        const int SensorRight_CA = 52; //D3
        const int SensorLeft_CA  = 50; //A3
        //const int SensorC3 = 35; //P1
    }

    // Actuator Fork Pinout for second robot
    namespace ServoFork{
        // Connector Act 2
        // Bloc Servo 1 - AB
        const int ServoAB_Elevator = 38; //P2 - 10 - 1 -> Non utilisé
        const int ServoAB_Fork = 34; //A4 - 8 - 3 
        //const int ServoAB_3 = 39; //D1 - 4 - 5
        // Bloc Servo 2 - CA
        const int ServoCA_Elevator = 14; //A2 - 2 - 7
        const int ServoCA_Fork = 15; //A1 - 1 - 8
        //const int ServoCA_3 = 55; //D2 - 3 - 6
        // Bloc Servo 3 - BC
        const int ServoBC_Elevator = 53; //D3 - 5 - 9 -> Non utilisé 
        const int ServoBC_Fork = 33; //A3 - 7 - 4 -> Non utilisé 
        //const int ServoCA_3 = 37; //P1 - 9 - 2
    }
}   

