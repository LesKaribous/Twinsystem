#pragma once
#include <Arduino.h>


#ifdef OLD_BOARD
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
    }

#else

    namespace Pin{
        // Declaration des pins E/S
        // IHM Pinout

        namespace Inputs{
            const int 
                resetButton = 38,
                starter = 32,
                teamSwitch = 31,
                strategySwitch = 36,
                twinSwitch = 2;
        }

        namespace Outputs{
            const int 
                enTraco = 28, // Enable Traco regulator - Enable on true
                buzzer = 3; // Buzzer pin
        }
        
        namespace Led{
            const int neopixel = 37;
        }
        
        // TFT pinout
        namespace TFT{
            const int
                CS = 10,
                DC = 9,
                RST = 255,
                MOSI = 11,
                SCK = 13,
                MISO = 12;
        }
        
        // Stepper Pinout
        namespace Stepper{
            const int
            enable = 24,
            dirA  = 25,
            dirB  = 4, 
            dirC  = 5, 
            stepA = 27,
            stepB = 6, 
            stepC = 26;
        }
        
        // Actuator Pinout
        namespace Servo{
            // Connector Act 1
            // Bloç Servo 1 - BC
            const int ServoA1 = 21; //P2
            const int ServoA2 = 22; //A4
            const int ServoA3 = 20; //D1
            // Bloç Servo 2 - AB
            const int ServoB1 = 23; //A2
            const int ServoB2 = 16; //A1
            const int ServoB3 = 39; //D2
            // Bloç Servo 3 - CA
            const int ServoC1 = 15; //D3
            const int ServoC2 = 14; //A3
            const int ServoC3 = 17; //P1
        }
    }
#endif
