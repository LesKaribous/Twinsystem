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
            twinSwitch = 2,
            upTrapSwitch = 34,
            downTrapSwitch = 15,
            lowTurbineSwitch = 14,  // I
            highTurbineSwitch = 33;  // II
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

    namespace Turbine{
        const int 
        enable = 39,
        speed = 37;
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
        
        const int ServoTrap = 55;
    }

    namespace CherryPicker{
        // EnableRegulator 
        const int pinEnaTraco   = 39;   
        // MotorDriver 
        const int pinTurbine    = 37;   //P1 - 2
        const int pinMotor      = 38;   //P2 - 1
        // ServoMotors
        const int pinServoTrap      = 55; //D2 - 6
        const int pinServoExtended  = 53; //D3 - 9
        // IHM BallGrabber
        const int pinButtonGrabber01 = 34; //A1 - 8 - UP
        const int pinButtonGrabber02 = 15; //A2 - 7 - DOWN
        const int pinButtonGrabber03 = 14; //A3 - 4 - I
        const int pinButtonGrabber04 = 33; //A4 - 3 - II
    }


}   

